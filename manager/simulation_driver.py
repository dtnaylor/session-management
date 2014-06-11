#!/usr/bin/env python

import numpy
from session_manager import SessionManager
import analyze_results as ar
from generator import *
from returner import returner

def main():
    
    # TEST ONE: All combos of everything -- count total # unique configurations
    # (i.e., module lists)
    # TEST FIVE: How many policy sets had a conflict? Make a heatmap with user
    # policy sets on one axis and admin policy sets on the other: intersection
    # is red if conflict, green if not
    configurations = []
    contextSet = generateContextSet()
    userPolicySets = generateUserPolicySets()
    appPolicySets = generateAppPolicySets()
    testSet = list(itertools.product(*[userPolicySets, appPolicySets, contextSet]))
    for (up, ap, c) in testSet: 
        module_set, conflicts = returner(up, ap, c)
        module_list = SessionManager().run(module_set)
        configurations.append(module_list)

    print 'TEST ONE: total configurations: %d' %\
        ar.count_configurations(configurations)

    # TODO: figure out how to make heatmap from user-app conflicts


    # TEST TWO: Fix a policy set. Vary accross all possible contexts. Count
    # resulting unique configurations. Then move to the next policy set; keep
    # going through all policy sets and calculate the mean configuration count.
    # Make sure context includes flow type and the battery/data usage.
    configuration_counts = []
    for p in policySet: 
        configurations = []
        for c in contextSet: 
            module_set, conflicts = returner(p, c)
            module_list = SessionManager().run(module_set)
            configurations.append(module_list)

        configuration_counts.append(ar.count_configurations(configurations))

    print 'TEST TWO: mean configurations per policy set: %f' %\
        numpy.mean(configuration_counts)


    # TEST THREE:  TODO


    # TEST FOUR:  For each user policy set, generate all possible
    # configurations (module lists); count how many of them are good (e.g.,
    # legal ordering and satisfy user's includes/excludes). Do the same for the
    # next user policy set. Calculate the mean number of legal configurations
    # per policy set.
    #legal_configurations_counts = []
    #for <user policy set> in <generator>: # TODO
    #    legal = 0
    #    for module_list in ar.generate_all_possible_configurations():
    #        if ar.test_configuration(module_list, <user policy set>): # TODO
    #            legal += 1
    #    legal_configurations_count.append(legal)

    #print 'TEST FOUR: mean legal configurations per user policy set: %f' %\
    #    numpy.mean(legal_configuration_counts)

if __name__ == '__main__':
    main()
