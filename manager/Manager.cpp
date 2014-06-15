#include "Manager.h"

// int Manager::getInterface(int fd) {
//     // do something with a hash lookup and some smarts
//     return 0;
// }

// int Manager::getTransport(int fd) {
//     // do something with a hash lookup and some smarts
//     return 0;
// }

int Manager::instantiateModules(int fd, bool listener) {
    // do something smart with preferences
    char modules[1];
    modules[0] = kEncryption;
    int modules_len = 1;

    printf("instantiating modules\n");

    return instantiateModulesFromBitArray(fd, modules, modules_len, listener);
}

int Manager::instantiateModulesFromBitArray(int fd, char *bitarray, int modules_len, bool initiator) {
    if(module_table.find(fd) == module_table.end()) {
        module_table[fd] = new std::list<DataPathModule *>;
    }

    for(int i = 0; i < modules_len; i++) {
        if (bitarray[i] & kCompression) {
            module_table[fd]->push_back(new CompressionModule);
        }
        if (bitarray[i] & kEncryption) {
            EncryptionModule *enc = new EncryptionModule;
            Preferences p;
            ContextualInfo i;
            enc->initialize(p, i, initiator);
            module_table[fd]->push_back(enc);
        }
    }
    return 0;
}

char *Manager::getModulesAsBitArray(int fd) {
    (void)fd;
    // do something with a hash lookup and some smarts
    array[0] = BIT_ARRAY_LEN;
    array[1] = kEncryption; //kCompression;
    return array;
}

int Manager::getBitArrayLen() {
    return BIT_ARRAY_LEN + 1;
}

int Manager::send(int fd, char *buf, size_t *n, size_t *buf_len) {
    if (module_table.find(fd) != module_table.end()) {
        for(std::list<DataPathModule *>::iterator it=module_table[fd]->begin();
            it != module_table[fd]->end(); ++it) {
            if((*it)->data_out(buf, (size_t *)n, buf_len)==-1) {
                // pass...
            }
        }
    }
    return 1; // TODO: this should be an error code
}

int Manager::recv(int fd, char *buf, size_t *input_len, size_t *output_len) {
    if (module_table.find(fd) != module_table.end()) {
        for(std::list<DataPathModule *>::reverse_iterator rit=module_table[fd]->rbegin();
            rit!=module_table[fd]->rend(); ++rit) {
            if((*rit)->data_in(buf, input_len, output_len)==-1) { // output_len?
                // pass...
            }
        }
    }
    return 1; //TODO: this should be an error code
}

int Manager::close(int fd) {
    if (module_table.find(fd) != module_table.end()) {
        std::list<DataPathModule *>::iterator it=module_table[fd]->begin();
        while (it != module_table[fd]->end()) {
            delete *it;
            it++;
        }
        module_table.erase(fd);
    }
    return 0;
}

DataPathModule *Manager::getFront(int fd) {
    return module_table[fd]->front();
}
