#!/usr/bin/env python

import numpy, sys
from session_manager import SessionManager
import analyze_results as ar
from generator import *
from returner import returner

def main():
    
    # TEST ONE: All combos of everything -- count total # unique configurations
    # (i.e., module lists)
    #
    # TEST TWO: Fix a policy set. Vary accross all possible contexts. Count
    # resulting unique configurations. Then move to the next policy set; keep
    # going through all policy sets and calculate the mean configuration count.
    # Make sure context includes flow type and the battery/data usage.
    #
    # TEST THREE:  Make sure each configuration we output is correct. (Should
    # we test includes/excludes? Probably not, because if we do, then conflict
    # cases will be marked as invalid configurations.)
    #
    # TEST FIVE: How many policy sets had a conflict? Make a heatmap with user
    # policy sets on one axis and admin policy sets on the other: intersection
    # is red if conflict, green if not
    configurations = []
    contextSet = generateContextSet()
    userPolicySets = generateUserPolicySets()
    appPolicySets = generateAppPolicySets()

    illegal_configuration_count = 0
    num_conflicting_policy_sets = 0
    user_policy_set_index = 0
    app_policy_set_index = 0
    conflict_coordinates = []  #list of tuples (x, y) -> there is a conflict with user policy set x and app policy set y
    per_policy_configuration_counts = []

    seen_modules = []
    for user_policies in userPolicySets:
        user_policy_set_index += 1

        for app_policies in appPolicySets:
            app_policy_set_index += 1

            per_policy_configurations = []
            for context in contextSet:
                module_set, conflicts = returner(user_policies, app_policies, context)
                seen_modules = list(set(module_set+seen_modules))
                module_list = SessionManager().run(module_set)
                configurations.append(module_list)
                per_policy_configurations.append(module_list)

                # count number of illegal configurations
                if not ar.test_configuration(module_list):
                    illegal_configuration_count += 1

                # count number of app-user conflicts
                for conflict in conflicts:
                    if conflict[0].role != conflict[1].role:
                        num_conflicting_policy_sets += 1
                        conflict_coordinates.append((user_policy_set_index, app_policy_set_index))

            per_policy_configuration_counts.append(ar.count_configurations(per_policy_configurations))

    print 'TEST ONE: total configurations: %d' %\
        ar.count_configurations(configurations)
    print seen_modules
    print len(seen_modules)
    print 'TEST TWO: mean configurations per policy set: %f' %\
        numpy.mean(per_policy_configuration_counts)
    print 'TEST THREE: illegal configurations: %d' %\
        illegal_configuration_count
    print 'TEST FIVE: conflicting app-user policy sets: %d' %\
        num_conflicting_policy_sets
    sys.exit(-1)

    # plot heatmap of app-user conflicts
    # user policy sets on X axis, app policy sets on Y axis
    print 'Generating conflict matrix...'
    matrix = numpy.zeros((user_policy_set_index+1, app_policy_set_index+1))
    for conflict in conflict_coordinates:
        matrix[conflict[0]][conflict[1]] = 1
    print 'Generating plot...'
    ar.heatmap(matrix, xlabel='User Policies', ylabel='App Policies',\
        filename='app-user-conflicts.pdf')



    ## TEST FOUR:  For each user policy set, generate all possible
    ## configurations (module lists); count how many of them are good (e.g.,
    ## legal ordering and satisfy user's includes/excludes). Do the same for the
    ## next user policy set. Calculate the mean number of legal configurations
    ## per policy set.
    #legal_configurations_counts = []
    #for user_policies in userPolicySets: 
    #   legal = 0
    #   for module_list in ar.generate_all_possible_configurations():
    #       if ar.test_configuration(module_list, user_policies): 
    #           legal += 1
    #   legal_configurations_count.append(legal)

    #print 'TEST FOUR: mean legal configurations per user policy set: %f' %\
    #   numpy.mean(legal_configuration_counts)

if __name__ == '__main__':
    main()
