#!/usr/bin/pythonx

import sys
import itertools
from policy import *

def logGoodiesConflicts(policies):
    # check that the two policies to consider are both the same module
    # and that they are include v exclude
    policies = [p[0] for p in policies if p[1] == True]
    possible_conflicts = list(itertools.combinations(policies, 2))
    conflicts = []
    for (p0, p1) in possible_conflicts:
        if p0.outcome.include_module == p1.outcome.exclude_module or p0.outcome.exclude_module == p1.outcome.include_module:
            if p0.role != p1.role:
                conflicts.append((p0, p1))
    return conflicts

def logConflicts(policies):
    # is this a user-user conflict, or a user-app conflict? Only will be user-app?
    # tuple for every pair of policies that conflict
    policies = [p[0] for p in policies if p[1] == True]
    conflicts = list(itertools.combinations(policies, 2))
    conflicts = [(c0,c1) for (c0,c1) in conflicts if c0.role != c1.role]
    return conflicts

def sortUserAndAppPolicies(user_policies, app_policies):
    if app_policies:
        return user_policies[:-1] + app_policies[:-1] + [user_policies[-1]] + [app_policies[-1]]
    else:
        return user_policies

def checkPredicate(policy, context):
    #check flow predicate
    result = policy.flow_predicate.test(context)

    #check context prediates
    for p in policy.context_predicates:
        result &= p.test(context)
    return result

def GoodiesClass(policies, context, modules):
    relevant_policies = []
    relevant_modules = set([ModuleName.ENCRYPTION, ModuleName.COMPRESSION, ModuleName.PII_LEAK_DETECTION, ModuleName.TRAFFIC_SHAPING])
    for policy in policies:
        if relevant_modules.intersection(set([policy.outcome.include_module, policy.outcome.exclude_module])):
            if checkPredicate(policy, context):
                relevant_policies.append((policy, True))
        elif policy.outcome.priority != None:
            if checkPredicate(policy, context):
                relevant_policies.append((policy, False))
    conflicts = logGoodiesConflicts(relevant_policies) 

    # go through all policies and add the modules:
    new_modules = []
    relevant_policies = [p[0] for p in relevant_policies]
    for policy in relevant_policies:
        if policy.outcome.include_module:
            new_modules.append(policy.outcome.include_module)

        elif policy.outcome.priority:
            if policy.outcome.priority[0] == GeneralConcern.SPEED:
                pass
            elif policy.outcome.priority[0] == GeneralConcern.ENERGY:
                pass
            elif policy.outcome.priority[0] == GeneralConcern.COST:
                new_modules += [ModuleName.COMPRESSION, ModuleName.TRAFFIC_SHAPING]
    new_modules = list(set(new_modules))
    return (new_modules, conflicts)

def TransportClass(policies, context, modules):
    relevant_policies = []
    relevant_modules = set([ModuleName.TCP, ModuleName.UDP, ModuleName.MPTCP])
    for policy in policies:
        if relevant_modules.intersection(set([policy.outcome.include_module, policy.outcome.exclude_module])):
            if checkPredicate(policy, context):
                relevant_policies.append((policy, True))
        elif policy.outcome.priority != None:
            if checkPredicate(policy, context):
                relevant_policies.append((policy, False))
    conflicts = logConflicts(relevant_policies)
    used_policy = relevant_policies[0][0]

    # Always return MPTCP if using WIFI and LTE
    new_modules = []
    if ModuleName.WIFI_AND_LTE in modules: 
        new_modules.append(ModuleName.MPTCP)
    else:
        if used_policy.outcome.include_module:
            new_modules.append(used_policy.outcome.include_module)
        elif used_policy.outcome.exclude_module == ModuleName.TCP:
            new_modules.append(ModuleName.UDP)
        elif used_policy.outcome.exclude_module == ModuleName.UDP:
            new_modules.append(ModuleName.TCP)
        elif used_policy.outcome.exclude_module == ModuleName.MPTCP:
            new_modules.append(ModuleName.UDP)
        elif used_policy.outcome.priority[0] == GeneralConcern.SPEED:
            #UDP if type is low-latency, else TCP
            if ContextVar.FLOW_TYPE in context and context[ContextVar.FLOW_TYPE] == FlowType.LOW_LATENCY:
                new_modules.append(ModuleName.UDP)
            else: new_modules.append(ModuleName.TCP)
        elif used_policy.outcome.priority[0] == GeneralConcern.ENERGY:
            new_modules.append(ModuleName.TCP)
        elif used_policy.outcome.priority[0] == GeneralConcern.COST:
            new_modules.append(ModuleName.TCP)
    return (new_modules, conflicts)

def NetworkClass(policies, context, modules):
    return ([ModuleName.IPV4], [])

def NICClass(policies, context, modules):
    relevant_policies = []
    relevant_modules = set([ModuleName.WIFI, ModuleName.LTE, ModuleName.WIFI_AND_LTE])
    for policy in policies:
        #print policy
        if relevant_modules.intersection(set([policy.outcome.include_module, policy.outcome.exclude_module])):
            if checkPredicate(policy, context):
                relevant_policies.append((policy, True))
        elif policy.outcome.priority != None:
            if checkPredicate(policy, context):
                relevant_policies.append((policy, False))
    conflicts = logConflicts(relevant_policies)
    used_policy = relevant_policies[0][0]

    new_modules = []
    if used_policy.outcome.include_module:
        new_modules.append(used_policy.outcome.include_module)
    elif used_policy.outcome.exclude_module == ModuleName.WIFI:
        new_modules.append(ModuleName.LTE)
    elif used_policy.outcome.exclude_module == ModuleName.LTE:
        new_modules.append(ModuleName.WIFI)
    elif used_policy.outcome.exclude_module == ModuleName.WIFI_AND_LTE:
        # This is actually an error...
        new_modules.append(ModuleName.LTE)
    elif used_policy.outcome.priority[0] == GeneralConcern.SPEED:
        #wifi if type is low-latency, bulk is WIFI+LTE, else LTE
        if ContextVar.FLOW_TYPE in context and context[ContextVar.FLOW_TYPE] == FlowType.LOW_LATENCY:
            new_modules.append(ModuleName.WIFI)
        elif ContextVar.FLOW_TYPE in context and context[ContextVar.FLOW_TYPE] == FlowType.BULK:
            new_modules.append(ModuleName.WIFI_AND_LTE)
        else: new_modules.append(ModuleName.LTE)
    elif used_policy.outcome.priority[0] == GeneralConcern.ENERGY:
        new_modules.append(ModuleName.WIFI)
    elif used_policy.outcome.priority[0] == GeneralConcern.COST:
        new_modules.append(ModuleName.WIFI)

    return (new_modules, conflicts)

def returner(user_policies, app_policies, context):
    policies = sortUserAndAppPolicies(user_policies, app_policies) 
    modules = []
    conflicts = []

    NICModules, NICConflicts = NICClass(policies, context, modules)
    modules += NICModules
    conflicts += NICConflicts

    NetworkModules, NetworkConflicts = NetworkClass(policies, context, modules)
    modules += NetworkModules
    conflicts += NetworkConflicts

    TransportModules, TransportConflicts = TransportClass(policies, context, modules)
    modules += TransportModules
    conflicts += TransportConflicts

    GoodiesModules, GoodiesConflicts= GoodiesClass(policies, context, modules)
    modules += GoodiesModules
    conflicts += GoodiesConflicts

    return modules, conflicts
