#!/usr/bin/env python

import numpy

import sys
sys.path.append('../../tools/myplot')
import myplot


##
## context plots
##

mean_configs_per_user_across_contexts = []
with open('context_results', 'r') as f:
    lines = f.readlines()
    mean_configs_per_user_across_contexts = eval(lines[-2].strip())
f.closed

myplot.cdf([mean_configs_per_user_across_contexts ], height_scale=0.6,
    xlabel='Mean # Unique Configurations per Context across 1000 Users',
    filename='configs_per_user.pdf')


##
## conflict plots
##

mean_conflicts_per_app_across_contexts = []
num_contexts_per_app_with_any_conflict = []
with open('conflict_results2', 'r') as f:
    lines = f.readlines()
    mean_conflicts_per_app_across_contexts = eval(lines[-4].strip())
    num_contexts_per_app_with_any_conflict = eval(lines[-2].strip())
f.closed


num_contexts_per_app_with_any_conflict = numpy.array(num_contexts_per_app_with_any_conflict) / 432.0 * 100  # percent of contexts with any conflict

myplot.cdf([mean_conflicts_per_app_across_contexts], height_scale=0.6,
    xlabel='Mean # Conflicting Policies per Context across 1000 Apps',
    filename='mean_conflicts_per_app_across_contexts.pdf')

myplot.cdf([num_contexts_per_app_with_any_conflict], height_scale=0.6,
    xlabel='% of Contexts Producing a Conflict Across 1000 Apps',
    filename='num_contexts_per_app_with_any_conflict.pdf')


##
## IRC plot
##

x_plain = [10, 20, 30, 50, 100]
y_plain = [437, 1872, 2857, 3392, 3389]
x_dummy = [10, 20, 30, 40, 50, 100]
y_dummy = [536, 1862, 2520, 2925, 3073, 3290]
x_enc = [10, 20, 30, 40, 50, 100]
y_enc = [51, 124, 128, 229, 303, 411]

myplot.plot([x_plain, x_dummy, x_enc], [y_plain, y_dummy, y_enc],
    labels=['No Shim', 'Shim w/o Encryption', 'Shim w/ Encryption'],
    xlabel='Number of Connected Clients', ylabel='Messages per Second',
    height_scale=0.6, legend='right',
    filename='irc-tput-all.pdf')

myplot.plot([x_plain, x_dummy, x_enc], [y_plain, y_dummy, y_enc],
    labels=['No Shim', 'Shim w/o Encryption', 'Shim w/ Encryption'],
    xlabel='Number of Connected Clients', ylabel='Messages per Second',
    height_scale=0.6, yscale='log',
    filename='irc-tput-all-log.pdf')
