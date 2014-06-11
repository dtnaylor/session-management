#!/usr/bin/env python

import numpy
from session_manager import SessionManager
import analyze_results as ar
from generator import generator
from returner import returner

def main():
    
    # TEST ONE: All combos of everything -- count total # unique configurations
    # (i.e., module lists)
    # TEST THREE:  Make sure each configuration we output is correct. (Should
    # we test includes/excludes? Probably not, because if we do, then conflict
    # cases will be marked as invalid configurations.)
    # TEST FIVE: How many policy sets had a conflict? Make a heatmap with user
    # policy sets on one axis and admin policy sets on the other: intersection
    # is red if conflict, green if not
    configurations = []
    illegal_configuration_count = 0
    for (policies, context) in generator(): 
        module_set, conflicts = returner(policies, context)
        module_list = SessionManager().run(module_set)
        configurations.append(module_list)
        if not ar.test_configuration(module_list):
            illegal_configuration_count += 1

    print 'TEST ONE: total configurations: %d (%d illegal)' %\
        (ar.count_configurations(configurations), illegal_configuration_count)

    # TODO: figure out how to make heatmap from user-app conflicts


    # TEST TWO: Fix a policy set. Vary accross all possible contexts. Count
    # resulting unique configurations. Then move to the next policy set; keep
    # going through all policy sets and calculate the mean configuration count.
    # Make sure context includes flow type and the battery/data usage.
    configuration_counts = []
    # for <policy set> in <generator>: # TODO
    #     configurations = []
    #     for <context> in <generator>: # TODO
    #         module_set, conflicts = <returner> # TODO
    #         module_list = SessionManager().run(module_set)
    #         configurations.append(module_list)

    #     configuration_counts.append(ar.count_configurations(configurations))

    print 'TEST TWO: mean configurations per policy set: %f' %\
        numpy.mean(configuration_counts)




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
