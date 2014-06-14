#!/usr/bin/python

import sys
import itertools
import random
from policy import *

random.seed()
general_concerns = [Outcome(priority=c) for c in itertools.permutations((GeneralConcern.ENERGY, GeneralConcern.SPEED, GeneralConcern.COST))]

EXPERIMENT = "CONFLICTS"
RANDOM_USER_POLICY = False
NUMBER_OF_APP_POLICIES = 1000
NUMBER_OF_RANDOM_USER_POLICIES = 10
ALLOW_NO_PREFERENCE_GENERATION = True

def generateContextSet():
    if EXPERIMENT == "USER_CONTROL":
        return [{}]

    if EXPERIMENT == "CONTEXT":
        context = {}
        context[ContextVar.NETWORK_TYPE] = [NetworkType.WIFI, NetworkType.LTE, NetworkType.WIFI_AND_LTE]
        context[ContextVar.LTE_DATA_USAGE] = [.1, .2, .3, .4, .5, .6, .7, .8, .9]
        context[ContextVar.BATTERY] = [.1, .2, .3, .4, .5, .6, .7, .8, .9]
        context[ContextVar.APPLICATION] = [Application.SPOTIFY, Application.SKYPE, Application.CHROME]
        context[ContextVar.FLOW_TYPE] = [FlowType.MULTIMEDIA, FlowType.LOW_LATENCY, FlowType.BULK]
        context[ContextVar.BW] = [100, 10000] #kbps
        context[ContextVar.LATENCY] = [30, 1000] #ms

        (keys,values) = zip(*context.items())
        context_list = list(itertools.product(*values))
        context = []
        for c in context_list:
            context.append(dict(zip(keys,c)))
        return context

    if EXPERIMENT == "CONFLICTS":
        context = {}
        context[ContextVar.NETWORK_TYPE] = [NetworkType.WIFI, NetworkType.LTE, NetworkType.WIFI_AND_LTE]
        context[ContextVar.LTE_DATA_USAGE] = [.1, .9]
        context[ContextVar.BATTERY] = [.1, .9]
        context[ContextVar.APPLICATION] = [Application.SPOTIFY, Application.SKYPE, Application.CHROME]
        context[ContextVar.FLOW_TYPE] = [FlowType.MULTIMEDIA, FlowType.LOW_LATENCY, FlowType.BULK]
        context[ContextVar.BW] = [100, 10000] #kbps
        context[ContextVar.LATENCY] = [30, 1000] #ms

        (keys,values) = zip(*context.items())
        context_list = list(itertools.product(*values))
        context = []
        for c in context_list:
            context.append(dict(zip(keys,c)))
        return context


def generateRandomOutcome():
    # pick a module
    modules = ModuleName.module_list()
    modules.remove('IPv6')
    modules.remove('IPv4')
    module = random.choice(modules)

    r = random.random()
    if ALLOW_NO_PREFERENCE_GENERATION:
        if r <= 0.33: # going to include module
            return Outcome(include_module = module)
        elif r <= 0.66: # exclude
            return Outcome(exclude_module = module)
        else: # no preference
            return None
    else:
        if r <= 0.5:
            return Outcome(include_module = module)
        else:
            return Outcome(exclude_module = module)

def generateNRandomPolicySets(role, n):
    policySets = []
    for i in xrange(n):
        policy = []
        if role == Role.USER:
            #Apps
            policy.append(Policy(role,FlowPredicate(Application.SPOTIFY, FlowType.ANY), [], generateRandomOutcome()))
            policy.append(Policy(role,FlowPredicate(Application.SKYPE, FlowType.ANY), [], generateRandomOutcome()))
            policy.append(Policy(role,FlowPredicate(Application.CHROME, FlowType.ANY), [], generateRandomOutcome()))
        
        #FlowTypes
        policy.append(Policy(role,FlowPredicate(Application.ANY, FlowType.MULTIMEDIA), [], generateRandomOutcome()))
        policy.append(Policy(role,FlowPredicate(Application.ANY, FlowType.LOW_LATENCY), [], generateRandomOutcome()))
        policy.append(Policy(role,FlowPredicate(Application.ANY, FlowType.BULK), [], generateRandomOutcome()))

        generic_flow = FlowPredicate(Application.ANY, FlowType.ANY)

        #Net Type
        policy.append(Policy(role, generic_flow, [ContextPredicate(ContextVar.NETWORK_TYPE, lambda x, y: x == y, NetworkType.WIFI)], generateRandomOutcome()))
        policy.append(Policy(role, generic_flow, [ContextPredicate(ContextVar.NETWORK_TYPE, lambda x, y: x == y, NetworkType.LTE)], generateRandomOutcome()))
        policy.append(Policy(role, generic_flow, [ContextPredicate(ContextVar.NETWORK_TYPE, lambda x, y: x == y, NetworkType.WIFI_AND_LTE)], generateRandomOutcome()))

        #data usage
        policy.append(Policy(role, generic_flow, [ContextPredicate(ContextVar.LTE_DATA_USAGE, lambda x, y: x < y, 0.8)], generateRandomOutcome()))
        policy.append(Policy(role, generic_flow, [ContextPredicate(ContextVar.LTE_DATA_USAGE, lambda x, y: x > y, 0.8)], generateRandomOutcome()))

        #battery
        policy.append(Policy(role, generic_flow, [ContextPredicate(ContextVar.BATTERY, lambda x, y: x < y, 0.2)], generateRandomOutcome()))
        policy.append(Policy(role, generic_flow, [ContextPredicate(ContextVar.BATTERY, lambda x, y: x > y, 0.2)], generateRandomOutcome()))
        
        #bw
        policy.append(Policy(role, generic_flow, [ContextPredicate(ContextVar.BW, lambda x, y: x > y, 1000)], generateRandomOutcome()))
        policy.append(Policy(role, generic_flow, [ContextPredicate(ContextVar.BW, lambda x, y: x < y, 1000)], generateRandomOutcome()))
        
        #latency
        policy.append(Policy(role, generic_flow, [ContextPredicate(ContextVar.LATENCY, lambda x, y: x > y, 100)], generateRandomOutcome()))
        policy.append(Policy(role, generic_flow, [ContextPredicate(ContextVar.LATENCY, lambda x, y: x < y, 100)], generateRandomOutcome()))

        policy = [p for p in policy if p.outcome]
        policySets.append(policy)
    return policySets
        

def generateAppPolicySets():
    if EXPERIMENT == "USER_CONTROL" or EXPERIMENT == "CONTEXT":
        return [[]]

    if EXPERIMENT == "CONFLICTS":
        return generateNRandomPolicySets(Role.APP, NUMBER_OF_APP_POLICIES)

def generateLegalOrderings():
    goodies = [ModuleName.PII_LEAK_DETECTION, ModuleName.COMPRESSION, ModuleName.ENCRYPTION, ModuleName.TRAFFIC_SHAPING]
    goodies_combinations = []
    for i in xrange(len(goodies)+1):
        goodies_combinations += list(itertools.combinations(goodies, i))
    transport = [ModuleName.TCP, ModuleName.UDP, ModuleName.MPTCP]
    network = [ModuleName.IPV4]
    nic = [ModuleName.WIFI, ModuleName.LTE, ModuleName.WIFI_AND_LTE]
    orderings = list(itertools.product(*[goodies_combinations, transport, network, nic]))
    orderings = [list(a)+[b,c,d] for (a,b,c,d) in orderings]
    orderings = [o for o in orderings if not (ModuleName.MPTCP in o and ModuleName.WIFI_AND_LTE not in o)]
    orderings = [o for o in orderings if not (ModuleName.MPTCP not in o and ModuleName.WIFI_AND_LTE in o)]
    for i in xrange(len(orderings)):
        if ModuleName.TRAFFIC_SHAPING in orderings[i]:
            o = orderings[i]
            loc = o.index(ModuleName.TRAFFIC_SHAPING)
            orderings[i] = o[:loc]+[o[loc+1]]+[o[loc]]+o[loc+2:]
    return orderings

def generateUserPolicySets(): 
    if EXPERIMENT == "USER_CONTROL":
        user = []
        testing = [ModuleName.ENCRYPTION, ModuleName.PII_LEAK_DETECTION, ModuleName.COMPRESSION, ModuleName.TRAFFIC_SHAPING, ModuleName.TCP, ModuleName.UDP, ModuleName.MPTCP, ModuleName.IPV4, ModuleName.WIFI, ModuleName.LTE, ModuleName.WIFI_AND_LTE]
        for m in testing:
            user.append([Outcome(include_module=m), None, Outcome(exclude_module=m)])

        user.append(general_concerns)
        user = list(itertools.product(*user))

        flow_predicate = FlowPredicate(Application.ANY, FlowType.ANY)

        policySets = []
        for u in user: 
            policySets.append([Policy(Role.USER, flow_predicate, [], o) for o in u if o])
        return policySets

    if EXPERIMENT == "CONTEXT" or EXPERIMENT == "CONFLICTS":
        if RANDOM_USER_POLICY:
            return generateNRandomPolicySets(Role.USER, NUMBER_OF_RANDOM_USER_POLICIES)

        policySet = []

        #Apps
        policySet.append(Policy(Role.USER,FlowPredicate(Application.SPOTIFY, FlowType.ANY), [], Outcome(include_module=ModuleName.TCP)))
        policySet.append(Policy(Role.USER,FlowPredicate(Application.SKYPE, FlowType.ANY), [], Outcome(include_module=ModuleName.ENCRYPTION)))
        policySet.append(Policy(Role.USER,FlowPredicate(Application.CHROME, FlowType.ANY), [], Outcome(include_module=ModuleName.PII_LEAK_DETECTION)))
        
        #FlowTypes
        policySet.append(Policy(Role.USER,FlowPredicate(Application.ANY, FlowType.MULTIMEDIA), [], Outcome(exclude_module=ModuleName.LTE)))
        policySet.append(Policy(Role.USER,FlowPredicate(Application.ANY, FlowType.LOW_LATENCY), [], Outcome(include_module=ModuleName.UDP)))
        policySet.append(Policy(Role.USER,FlowPredicate(Application.ANY, FlowType.BULK), [], Outcome(include_module=ModuleName.MPTCP)))

        generic_flow = FlowPredicate(Application.ANY, FlowType.ANY)

        #Net Type
        policySet.append(Policy(Role.USER, generic_flow, [ContextPredicate(ContextVar.NETWORK_TYPE, lambda x, y: x == y, NetworkType.WIFI)], Outcome(include_module=ModuleName.ENCRYPTION)))
        policySet.append(Policy(Role.USER, generic_flow, [ContextPredicate(ContextVar.NETWORK_TYPE, lambda x, y: x == y, NetworkType.LTE)], Outcome(include_module=ModuleName.TRAFFIC_SHAPING)))
        policySet.append(Policy(Role.USER, generic_flow, [ContextPredicate(ContextVar.NETWORK_TYPE, lambda x, y: x == y, NetworkType.WIFI_AND_LTE)], Outcome(include_module=ModuleName.MPTCP)))

        #data usage
        policySet.append(Policy(Role.USER, generic_flow, [ContextPredicate(ContextVar.LTE_DATA_USAGE, lambda x, y: x < y, 0.8)], Outcome(include_module=ModuleName.LTE)))
        policySet.append(Policy(Role.USER, generic_flow, [ContextPredicate(ContextVar.LTE_DATA_USAGE, lambda x, y: x > y, 0.8)], Outcome(include_module=ModuleName.COMPRESSION)))

        #battery
        policySet.append(Policy(Role.USER, generic_flow, [ContextPredicate(ContextVar.BATTERY, lambda x, y: x < y, 0.2)], Outcome(exclude_module=ModuleName.LTE)))
        policySet.append(Policy(Role.USER, generic_flow, [ContextPredicate(ContextVar.BATTERY, lambda x, y: x > y, 0.2)], Outcome(include_module=ModuleName.WIFI_AND_LTE)))
        
        #bw
        policySet.append(Policy(Role.USER, generic_flow, [ContextPredicate(ContextVar.BW, lambda x, y: x > y, 1000)], Outcome(exclude_module=ModuleName.COMPRESSION)))
        policySet.append(Policy(Role.USER, generic_flow, [ContextPredicate(ContextVar.BW, lambda x, y: x < y, 1000)], Outcome(include_module=ModuleName.COMPRESSION)))
        
        #latency
        policySet.append(Policy(Role.USER, generic_flow, [ContextPredicate(ContextVar.LATENCY, lambda x, y: x > y, 100)], Outcome(include_module=ModuleName.COMPRESSION)))
        policySet.append(Policy(Role.USER, generic_flow, [ContextPredicate(ContextVar.LATENCY, lambda x, y: x < y, 100)], Outcome(exclude_module=ModuleName.TRAFFIC_SHAPING)))

        return [policySet]
