#!/usr/bin/python

import sys
import itertools
from policy import *

def generator(): 
    context_NETWORK_TYPE = [NetworkType.WIFI, NetworkType.LTE, NetworkType.WIFI_AND_LTE]
    context_LTE_DATA_USAGE = [.1, .9]
    context_BATTERY = [.1, .9]
    context_APPLICATION = [Application.SPOTIFY, Application.SKYPE, Application.CHROME]
    context_FLOW_TYPE = [FlowType.MULTIMEDIA, FlowType.LOW_LATENCY, FlowType.BULK]
    context_list = [context_NETWORK_TYPE, context_LTE_DATA_USAGE, context_BATTERY, context_APPLICATION, context_FLOW_TYPE]
    context_list = list(itertools.product(*context_list))
    context = []
    for c in context_list:
        d = {}
        d[ContextVar.NETWORK_TYPE] = c[0]
        d[ContextVar.LTE_DATA_USAGE] = c[1]
        d[ContextVar.BATTERY] = c[2]
        d[ContextVar.APPLICATION] = c[3]
        d[ContextVar.FLOW_TYPE] = c[4]
        context.append(d)

    app_encryption = [True, False] # required, no preference
    app_reliability = [True, False] # required, no preference
    app_concerns = list(itertools.permutations((GeneralConcern.ENERGY, GeneralConcern.SPEED, GeneralConcern.COST)))
    app = [app_encryption, app_reliability, app_concerns]
    app = list(itertools.product(*app))

    user_encryption = [True, False] # required, no preference
    user_pii_leak_detection = [True, False] # required, no preference
    user_lte_data_usage = [True, False] # no preference, disallow
    user_concerns = list(itertools.permutations((GeneralConcern.ENERGY, GeneralConcern.SPEED, GeneralConcern.COST)))
    user_lte_predicate = [(None, '*'), (lambda x, y: x < y, 0.85), (lambda x, y: x > y, 0.85)]
    user_battery_predicate = [(None, '*'), (lambda x, y: x < y, 0.15), (lambda x, y: x > y, 0.15)]
    user = [user_encryption, user_pii_leak_detection, user_lte_data_usage, user_concerns, user_lte_predicate, user_battery_predicate]
    user = list(itertools.product(*user))

    output = []
    for u in user: #[encryption, pii, data_usage, concerns, lte_predicate, battery_predicate]
        for a in app: #[encryption, reliability, concerns]
            policies = []

            # user policies
            flow_predicate = FlowPredicate(Application.ANY, FlowType.ANY)
            context_predicates = [ContextPredicate(ContextVar.LTE_DATA_USAGE, u[4][0], u[4][1]), ContextPredicate(ContextVar.BATTERY, u[5][0], u[5][1])]
            if u[0]: #encryption
                policies.append(Policy(Role.USER, flow_predicate, context_predicates, Outcome(include_module=ModuleName.ENCRYPTION)))
            if u[1]: #pii
                policies.append(Policy(Role.USER, flow_predicate, context_predicates, Outcome(include_module=ModuleName.PII_LEAK_DETECTION)))
            if u[2] == False: #data_usage
                policies.append(Policy(Role.USER, flow_predicate, context_predicates, Outcome(exclude_module=ModuleName.LTE)))

            # app policies
            if a[0]: #encryption
                policies.append(Policy(Role.APP, flow_predicate, [], Outcome(include_module=ModuleName.ENCRYPTION)))
            if a[1]: #reliability
                policies.append(Policy(Role.APP, flow_predicate, [], Outcome(exclude_module=ModuleName.UDP)))

            # General concerns
            policies.append(Policy(Role.USER, flow_predicate, context_predicates, Outcome(priority=u[3])))
            policies.append(Policy(Role.APP, flow_predicate, [], Outcome(priority=a[2])))



            output.append(policies)

    output = list(itertools.product(*[output,context]))
    return output
