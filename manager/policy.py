
class Role(object):
    USER = 'user'
    ADMIN = 'administrator'
    APP = 'application'

##
## FLOW PREDICATES
##
class FlowType(object):
    MULTIMEDIA = 'multimedia'
    LOW_LATENCY = 'low latency'
    BULK = 'bulk'
    ANY = '*'

class Application(object):
    SPOTIFY = 'spotify'
    SKYPE = 'skype'
    CHROME = 'chrome'
    ANY = '*'

class FlowPredicate(object):
    def __init__(self, app, flow_type):
        self.app = app
        self.flow_type = flow_type

    # test if supplied app and flow_type match this predicate
    def test(self, context):
        if not context:
            return True
        app = context[ContextVar.APPLICATION]
        flow_type = context[ContextVar.FLOW_TYPE]
        app_match = True if self.app == Application.ANY else app == self.app
        type_match = True if self.flow_type == FlowType.ANY else\
            flow_type == self.flow_type
        return app_match and type_match

    def __str__(self):
        return "(%s, %s)" % (self.app, self.flow_type)


##
## CONTEXT PREDICATES
##
class ContextVar(object):
    NETWORK_TYPE = 'network type'
    BW = 'bandwidth'
    LATENCY = 'latency'
    LTE_DATA_USAGE = 'LTE data usage'
    BATTERY = 'battery'
    APPLICATION = 'application'
    FLOW_TYPE = 'flow type'

class NetworkType(object):
    WIFI = 'wifi'
    LTE = 'lte'
    WIFI_AND_LTE = 'wifi+lte'

class ContextPredicate(object):
    # relation should be a function like that compares value to its argument X:
    # return value relation X
    # e.g., for <=:
    # lambda value, testval: value <= testval 
    def __init__(self, var, relation, value):
        self.var = var
        self.relation = relation
        self.value = value

    # test if current context matches this predicate
    # context_dict is a dict with all network and devices values, like
    # { ContextVar.BATTERY = 0.87 }
    def test(self, context_dict):
        return True if self.value == '*' else \
            self.relation(self.value, context_dict[self.var])

    def __str__(self):
        if self.relation == None:
            return '*'
        return "%s %s %s" % (self.var, self.relation, self.value)


##
## OUTCOME
##
class GeneralConcern(object):
    SPEED = 'speed'
    ENERGY = 'energy'
    COST = 'data cost'

class ModuleName(object):
    ENCRYPTION = 'encryption'
    COMPRESSION = 'compression'
    PII_LEAK_DETECTION = 'PII leak detection'
    TRAFFIC_SHAPING = 'traffic shaping'
    TCP = 'tcp'
    UDP = 'udp'
    MPTCP = 'mptcp'
    IPV4 = 'IPv4'
    IPV6 = 'IPv6'
    WIFI = 'wifi'
    LTE = 'lte'
    WIFI_AND_LTE = 'wifi+lte'

    @classmethod 
    def module_list(cls):
        return [cls.__dict__[x] for x in cls.__dict__ if x.isupper()]  # HACK!

class Outcome(object):
    # only one of these should be set, but we don't check
    def __init__(self, include_module=None, exclude_module=None, priority=None):
        self.include_module = include_module
        self.exclude_module = exclude_module
        self.priority = priority

    def __str__(self):
        if self.include_module:
            return "INCLUDE %s" % self.include_module
        if self.exclude_module:
            return "EXCLUDE %s" % self.exclude_module
        return '(%s, %s, %s)' % (self.priority)

    def __eq__(self, other):
        return (isinstance(other, self.__class__)
            and self.include_module == other.include_module
            and self.exclude_module == other.exclude_module
            and self.priority == other.priority)

##
## POLICIES
##
class Policy(object):
    def __init__(self, role, flow_predicate, context_predicates, outcome):
        self.role = role
        self.flow_predicate  = flow_predicate
        self.context_predicates = context_predicates
        self.outcome = outcome

    # test if this predicate applies to a given flow and context dict
    def policy_applies(self, app, flow_type, context_dict):
        applies = self.flow_predicate.test(app, flow_type)
        for context_pred in self.context_predicates:
            applies = applies and context_pred.test(context_dict)
        return applies

    def __str__(self):
        c_str = ""
        for c in self.context_predicates:
            c_str += '%s,' % c
        c_str = list(set(c_str.split(',')[:-1]))
        c_str = ' and '.join(c_str)
        c_str = "*" if c_str == "" else c_str
        return "(%s) if %s && %s then %s" % (self.role, self.flow_predicate, c_str, self.outcome)

##
## DATA PATH
##
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
    ModuleName.WIFI_AND_LTE    : {'required': DataPathState.MUTABLE_BYTES,
                                  'resulting': DataPathState.UNCHANGED,
                                  'class': ModuleClassName.NIC},
}
