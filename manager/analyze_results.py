import itertools
import matplotlib.pyplot as plt
from collections import defaultdict
from policy import *

# test if a configuration is "good":
#   1) each module receives data in the state it requires
#   2) user's specific properties are satisfied, if policies are provided
#   3) there is at least one NIC, one network and one transport
def test_configuration(module_list, policies=None):
    state = DataPathState.VIEWABLE_DATA
    class_counts = defaultdict(int)

    # make sure modules have the right data path state
    for module in module_list:
        info = MODULE_STATES[module]

        class_counts[info['class']] += 1
        if info['required'] > state:
            return False
        if info['resulting'] != DataPathState.UNCHANGED:
            state = info['resulting']

    # make sure there's at least one NIC, one transport, and one network
    if class_counts[ModuleClassName.NIC] <= 0 or\
       class_counts[ModuleClassName.NETWORK] != 1 or\
       class_counts[ModuleClassName.TRANSPORT] != 1:
        return False

    # make sure specific user policies are met...
    if policies:
        for policy in policies:
            if policy.role == Role.USER:
                if policy.outcome.include_module and\
                    policy.outcome.include_module not in module_list:
                    return False
                if policy.outcome.exclude_module and\
                    policy.outcome.exclude_module in module_list:
                    return False

    return True

# takes in an iterable containing configurations (i.e., module lists) and
# returns how many unique configurations there are
def count_configurations(configurations):
    conf_set = set()

    for conf in configurations:
        conf_set.add(tuple(conf))

    return len(conf_set)

# generate all possible orderings of modules
def generate_all_possible_configurations():
    for i in range(len(ModuleName.module_list())+1):
        for conf in itertools.permutations(ModuleName.module_list(), i):
            yield conf


# out of all possible data path module configurations, how many are legal
# (as determined by test_configuration())?
def count_legal_configurations():
    total = 0
    legal = 0
    for conf in generate_all_possible_configurations():
        total += 1
        if test_configuration(conf): legal += 1

    print 'Num configurations (legal/total):\t%d/%d' % (legal, total)



def heatmap(matrix, xlabel=None, ylabel=None, filename=None):
    plt.pcolor(np.array(matrix), cmap='RdYlGn')
    if xlabel: plt.xlabel(xlabel)
    if ylabel: plt.ylabel(ylabel)
    
    # make sure no text is clipped along the boundaries
    plt.tight_layout()

    # FIXME hardcoded stuff that shouldn't be
    plt.tick_params(\
        axis='x',          # changes apply to the x-axis
        which='both',      # both major and minor ticks are affected
        bottom='off',      # ticks along the bottom edge are off
        top='off',         # ticks along the top edge are off
        labelbottom='off') # labels along the bottom edge are off
    plt.tick_params(\
        axis='y',          # changes apply to the x-axis
        which='both',      # both major and minor ticks are affected
        left='off',
        right='off',
        labelleft='off')


    plt.savefig(filename)
