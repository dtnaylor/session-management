#!/usr/bin/python

import sys
import itertools
from generator import generator
from policy import *

def logConflicts(policies):
    # is this a user-user conflict, or a user-app conflict?
    # tuple for every pair of policies that conflict
    policies = [p[0] for p in policies if p[1] == True]
    return list(itertools.combinations(policies, 2))

def GoodiesClass(policies, context, modules):
    relevant_policies = []
    relevant_modules = set([ModuleName.ENCRYPTION, ModuleName.COMPRESSION, ModuleName.PII_LEAK_DETECTION, ModuleName.TRAFFIC_SHAPING])
    for policy in policies:
        if relevant_modules.intersection(set([policy.outcome.include_module, policy.outcome.exclude_module])):
            relevant_policies.append((policy, True))
        if policy.outcome.priority != None:
            relevant_policies.append((policy, False))
    #I don't think these can conflict in our setup
    #conflicts = logConflicts(relevant_policies) 

    # go through all policies and add the modules:
    new_modules = []
    relevant_policies = [p[0] for p in relevant_policies]
    for policy in relevant_policies:
        if policy.outcome.include_module:
            new_modules.append(policy.outcome.include_module)

        if policy.outcome.priority:
            if policy.outcome.priority[0] == GeneralConcern.SPEED:
                pass
            elif policy.outcome.priority[0] == GeneralConcern.ENERGY:
                pass
            elif policy.outcome.priority[0] == GeneralConcern.COST:
                new_modules.append([ModuleName.COMPRESSION, ModuleName.TRAFFIC_SHAPING])
    return (new_modules, [])

def TransportClass(policies, context, modules):
    relevant_policies = []
    relevant_modules = set([ModuleName.TCP, ModuleName.UDP, ModuleName.MPTCP])
    for policy in policies:
        if relevant_modules.intersection(set([policy.outcome.include_module, policy.outcome.exclude_module])):
            relevant_policies.append((policy, True))
        if policy.outcome.priority != None:
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
            if context[ContextVar.FLOW_TYPE] == FlowType.LOW_LATENCY:
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
            relevant_policies.append((policy, True))
        if policy.outcome.priority != None:
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
    elif used_policy.outcome.priority[0] == GeneralConcern.SPEED:
        #wifi if type is low-latency, bulk is WIFI+LTE, else LTE
        if context[ContextVar.FLOW_TYPE] == FlowType.LOW_LATENCY:
            new_modules.append(ModuleName.WIFI)
        elif context[ContextVar.FLOW_TYPE] == FlowType.BULK:
            new_modules.append(ModuleName.WIFI_AND_LTE)
        else: new_modules.append(ModuleName.LTE)
    elif used_policy.outcome.priority[0] == GeneralConcern.ENERGY:
        new_modules.append(ModuleName.WIFI)
    elif used_policy.outcome.priority[0] == GeneralConcern.COST:
        new_modules.append(ModuleName.WIFI)

    return (new_modules, conflicts)

def returner(policies, context):
    #policies = sortpolicies(policies) #these are already in order
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
