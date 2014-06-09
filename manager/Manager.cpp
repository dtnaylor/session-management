#include "Manager.h"

int Manager::getInterface(int fd) {
    // do something with a hash lookup and some smarts
    return 0;
}

int Manager::getTransport(int fd) {
    // do something with a hash lookup and some smarts
    return 0;
}

int Manager::instantiateModules(int fd) {
    // do something smart with preferences
    char modules[1];
    modules[0] = kCompression;
    int modules_len = 1;

    printf("instantiating modules\n");

    return instantiateModulesFromBitArray(fd, modules, modules_len);
}

int Manager::instantiateModulesFromBitArray(int fd, char *bitarray, int modules_len) {
    if(module_table.find(fd) == module_table.end()) {
        module_table[fd] = new std::list<DataPathModule *>;
    }

    for(int i = 0; i < modules_len; i++) {
        if (modules[i] & kCompression) {
            module_table[fd].push_back(new CompressionModule)
        }
    }
    return 0;
}

char *Manager::getModulesAsBitArray(int fd) {
    // do something with a hash lookup and some smarts
    array[0] = BIT_ARRAY_LEN;
    array[1] = kCompression;
    return array;
}

int Manager::getBitArrayLen() {
    return BIT_ARRAY_LEN + 1;
}

int Manager::send(int fd, char *buf, int n) {
    int len = 0;
    if (module_table.find(fd) != module_table.end()) {
        for(std::list<DataPathModule>::iterator it=module_table[fd].begin();
            it != module_table[fd].end(); ++it) {
            len = module->data_out(buf, n);
        }
    }
    return len;
}

int Manager::recv(int fd, char *buf, int input_len, int output_len) {
    int len = 0;
    if (module_table.find(fd) != module_table.end()) {
        for(std::list<DataPathModule>::reverse_iterator rit=module_table[fd].rbegin();
            rit!=module_table[fd].rend(); ++rit) {
            len = module->data_in(buf, input_len, output_len); // output_len?
        }
    }
    return len;
}

int Manager::close(int fd) {
    if (module_table.find(fd) != module_table.end()) {
        std::list<DataPathModule>::iterator it=module_table[fd].begin();
        while (it != module_table[fd].end()) {
            delete it++;
            //module_table[fd].erase(it++);
        }
        delete module_table[fd];
    }
}
