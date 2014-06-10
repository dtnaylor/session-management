import itertools
from collections import defaultdict
from policy import *

# test if a configuration is "good":
#   1) each module receives data in the state it requires
#   2) specific properties are satisfied << NO -- this is just a conflict
#   3) there is at least one NIC, one network and one transport
def test_configuration(module_list):
    state = DataPathState.VIEWABLE_DATA
    class_counts = defaultdict(int)

    for module in module_list:
        info = MODULE_STATES[module]

        class_counts[info['class']] += 1
        if info['required'] > state:
            return False
        if info['resulting'] != DataPathState.UNCHANGED:
            state = info['resulting']

    if class_counts[ModuleClassName.NIC] <= 0 or\
       class_counts[ModuleClassName.NETWORK] != 1 or\
       class_counts[ModuleClassName.TRANSPORT] != 1:
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


