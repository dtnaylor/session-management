#!/usr/bin/env python

import numpy
from session_manager import SessionManager
import analyze_results as ar
from generator import generator
from returner import returner

def main():
    
    # TEST ONE: All combos of everything -- count total # unique configurations
    # (i.e., module lists)
    # TEST FIVE: How many policy sets had a conflict? Make a heatmap with user
    # policy sets on one axis and admin policy sets on the other: intersection
    # is red if conflict, green if not
    configurations = []
    for (policies, context) in generator(): 
        module_set, conflicts = returner(policies, context)
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
    # for <policy set> in <generator>: # TODO
    #     configurations = []
    #     for <context> in <generator>: # TODO
    #         module_set, conflicts = <returner> # TODO
    #         module_list = SessionManager().run(module_set)
    #         configurations.append(module_list)

    #     configuration_counts.append(ar.count_configurations(configurations))

    print 'TEST TWO: mean configurations per policy set: %f' %\
        numpy.mean(configuration_counts)


    # TEST THREE:  TODO


    # TEST FOUR:  TODO

if __name__ == '__main__':
    main()
