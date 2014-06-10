#!/usr/bin/env python

import analyze_results as ar
from collections import defaultdict
from policy import *



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

        # STEP TWO: within each group, sort by required state
        for result in m_by_result:
            m_by_result[result] = sorted(m_by_result[result], reverse=True,\
                key= lambda x: MODULE_STATES[x]['required'])

        # concatenate the lists together
        module_list = []
        for result, modules in sorted(m_by_result.items(), reverse=True):
            module_list += modules

        # add network and NICs
        module_list += network_modules + nic_modules

        return module_list

def dbg_conf(other_modules=[], nic_modules=[], network_modules=[]):
    conf = SessionManager().run(other_modules=other_modules,\
        nic_modules=nic_modules, network_modules=network_modules)
    print conf
    print ar.test_configuration(conf)
    return conf
        




def main():

    # testing:
    conf_list = []
    conf_list.append(dbg_conf(other_modules=[ModuleName.ENCRYPTION, ModuleName.COMPRESSION, ModuleName.PII_LEAK_DETECTION]))
    conf_list.append(dbg_conf(other_modules=[ModuleName.TCP, ModuleName.ENCRYPTION, ModuleName.COMPRESSION], nic_modules=[ModuleName.WIFI], network_modules=[ModuleName.IPV4]))
    conf_list.append([ModuleName.COMPRESSION, ModuleName.PII_LEAK_DETECTION, ModuleName.ENCRYPTION])

    print 'num configurations: %d' % ar.count_configurations(conf_list)



if __name__ == '__main__':
    main()
