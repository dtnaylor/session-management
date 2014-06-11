#!/usr/bin/python

import sys
import itertools
from policy import *

general_concerns = [Outcome(priority=c) for c in itertools.permutations((GeneralConcern.ENERGY, GeneralConcern.SPEED, GeneralConcern.COST))]

def generateContextSet():
    context = {}
    context[ContextVar.NETWORK_TYPE] = [NetworkType.WIFI, NetworkType.LTE, NetworkType.WIFI_AND_LTE]
    context[ContextVar.LTE_DATA_USAGE] = [.1, .9]
    context[ContextVar.BATTERY] = [.1, .9]
    context[ContextVar.APPLICATION] = [Application.SPOTIFY, Application.SKYPE, Application.CHROME]
    context[ContextVar.FLOW_TYPE] = [FlowType.MULTIMEDIA, FlowType.LOW_LATENCY, FlowType.BULK]

    (keys,values) = zip(*context.items())
    context_list = list(itertools.product(*values))
    context = []
    for c in context_list:
        context.append(dict(zip(keys,c)))
    return context

def generateAppPolicySets():
    app = []
    app.append([Outcome(include_module=ModuleName.ENCRYPTION), None, Outcome(exclude_module=ModuleName.ENCRYPTION)])
    app.append([Outcome(include_module=ModuleName.PII_LEAK_DETECTION), None, Outcome(exclude_module=ModuleName.PII_LEAK_DETECTION)])
    app.append([Outcome(include_module=ModuleName.COMPRESSION), None, Outcome(exclude_module=ModuleName.COMPRESSION)])
    app.append([Outcome(include_module=ModuleName.TRAFFIC_SHAPING), None, Outcome(exclude_module=ModuleName.TRAFFIC_SHAPING)])
    app.append([Outcome(exclude_module=ModuleName.UDP), None]) # required, no preference
    app.append(general_concerns)
    app = list(itertools.product(*app))

    policySets = []
    flow_predicate = FlowPredicate(Application.ANY, FlowType.ANY)
    for a in app:
        policySets.append([Policy(Role.APP, flow_predicate, [], o) for o in a if o])
    return policySets

def generateUserPolicySets(): 
    user = []
    user.append([Outcome(include_module=ModuleName.ENCRYPTION), None, Outcome(exclude_module=ModuleName.ENCRYPTION)])
    user.append([Outcome(include_module=ModuleName.PII_LEAK_DETECTION), None, Outcome(exclude_module=ModuleName.PII_LEAK_DETECTION)])
    user.append([Outcome(include_module=ModuleName.COMPRESSION), None, Outcome(exclude_module=ModuleName.COMPRESSION)])
    user.append([Outcome(include_module=ModuleName.TRAFFIC_SHAPING), None, Outcome(exclude_module=ModuleName.TRAFFIC_SHAPING)])
    user.append([None, Outcome(exclude_module=ModuleName.LTE)]) # no preference, disallow
    user.append(general_concerns)
    user = list(itertools.product(*user))

    contextpredicate = [] 
    contextpredicate.append([ContextPredicate(ContextVar.LTE_DATA_USAGE, None, '*'), ContextPredicate(ContextVar.LTE_DATA_USAGE, lambda x, y: x < y, 0.85), ContextPredicate(ContextVar.LTE_DATA_USAGE, lambda x, y: x > y, 0.85)])
    contextpredicate.append([ContextPredicate(ContextVar.BATTERY, None, '*'), ContextPredicate(ContextVar.BATTERY, lambda x, y: x < y, 0.15), ContextPredicate(ContextVar.BATTERY, lambda x, y: x > y, 0.15)])
    contextpredicate = list(itertools.product(*contextpredicate))

    flow_predicate = FlowPredicate(Application.ANY, FlowType.ANY)

    policySets = []
    for u in user: 
        for cp in contextpredicate:
            policySets.append([Policy(Role.USER, flow_predicate, cp, o) for o in u if o])
    return policySets
