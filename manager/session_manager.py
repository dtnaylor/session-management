#!/usr/bin/env python

from collections import defaultdict
from policy import *

class DataPathState(object):
    ANY = 0
    MUTABLE_BYTES = 1
    ADJUSTABLE_TIMING = 2
    MUTABLE_DATA = 3
    VIEWABLE_DATA = 4
    UNCHANGED = 5

class ModuleClassName(object):
    LOCAL_EXTRA = 4
    TRANSPORT = 3
    NETWORK = 2
    NIC = 1

    
MODULE_STATES = {
    ModuleName.ENCRYPTION :      {'required': DataPathState.ANY,
                                  'resulting': DataPathState.MUTABLE_DATA,
                                  'class': ModuleClassName.LOCAL_EXTRA},
    ModuleName.COMPRESSION :     {'required': DataPathState.VIEWABLE_DATA,
                                  'resulting': DataPathState.MUTABLE_DATA,
                                  'class': ModuleClassName.LOCAL_EXTRA},
    ModuleName.PII_LEAK_DETECTION : {'required': DataPathState.VIEWABLE_DATA,
                                'resulting': DataPathState.UNCHANGED,
                                'class': ModuleClassName.LOCAL_EXTRA},
    ModuleName.TRAFFIC_SHAPING : {'required': DataPathState.ADJUSTABLE_TIMING,
                                  'resulting': DataPathState.MUTABLE_BYTES,
                                  'class': ModuleClassName.LOCAL_EXTRA},
    ModuleName.TCP             : {'required': DataPathState.ADJUSTABLE_TIMING,
                                  'resulting': DataPathState.ADJUSTABLE_TIMING,
                                  'class': ModuleClassName.TRANSPORT},
    ModuleName.UDP             : {'required': DataPathState.ANY,
                                  'resulting': DataPathState.ADJUSTABLE_TIMING,
                                  'class': ModuleClassName.TRANSPORT},
    ModuleName.MPTCP           : {'required': DataPathState.ADJUSTABLE_TIMING,
                                  'resulting': DataPathState.ADJUSTABLE_TIMING,
                                  'class': ModuleClassName.TRANSPORT},
    ModuleName.IPV4            : {'required': DataPathState.MUTABLE_BYTES,
                                  'resulting': DataPathState.UNCHANGED,
                                  'class': ModuleClassName.NETWORK},
    ModuleName.IPV6            : {'required': DataPathState.MUTABLE_BYTES,
                                  'resulting': DataPathState.UNCHANGED,
                                  'class': ModuleClassName.NETWORK},
    ModuleName.WIFI            : {'required': DataPathState.MUTABLE_BYTES,
                                  'resulting': DataPathState.UNCHANGED,
                                  'class': ModuleClassName.NIC},
    ModuleName.LTE             : {'required': DataPathState.MUTABLE_BYTES,
                                  'resulting': DataPathState.UNCHANGED,
                                  'class': ModuleClassName.NIC},
}


class SessionManager(object):
    def __init__(self, policies=[], context_dict={}):
        self.policies = policies
        self.context_dict = context_dict

    # configure the session! returns ordered list of modules
    def run(self, network_modules=[], other_modules=[], nic_modules=[]):
        # TODO: get modules from module classes
        #nic_modules = []
        #network_modules = []
        #other_modules = []

        # STEP ONE: group modules by ending state
        m_by_result = defaultdict(list)
        for m in other_modules:
            m_by_result[MODULE_STATES[m]['resulting']].append(m)
        print m_by_result

        # STEP TWO: within each group, sort by required state
        for result in m_by_result:
            m_by_result[result] = sorted(m_by_result[result], reverse=True,\
                key= lambda x: MODULE_STATES[x]['required'])
        print m_by_result

        # concatenate the lists together
        module_list = []
        for result, modules in sorted(m_by_result.items(), reverse=True):
            module_list += modules

        # add network and NICs
        module_list += network_modules + nic_modules
        print module_list




def main():
    SessionManager().run(other_modules=[ModuleName.ENCRYPTION, ModuleName.COMPRESSION, ModuleName.PII_LEAK_DETECTION])
    SessionManager().run(other_modules=[ModuleName.TCP, ModuleName.ENCRYPTION, ModuleName.COMPRESSION], nic_modules=[ModuleName.WIFI], network_modules=[ModuleName.IPV4])


if __name__ == '__main__':
    main()
