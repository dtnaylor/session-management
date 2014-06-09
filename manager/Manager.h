/*
** Copyright 2014 Carnegie Mellon University
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
** http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

#ifndef MANAGER_H
#define MANAGER_H

#define BIT_ARRAY_LEN 1 // one byte

using namespace std;

#include <unordered_map>
#include <list>
#include "DataPathModule.h"

const char kCompression = 128;

class Manager {
 public:
    int getInterface(int fd);
    int getTransport(int fd);
    int instantiateModules(int fd);
    int instantiateModulesFromBitArray(int fd, char *bitarray, int len);
    char *getModulesAsBitArray(int fd);
    int getBitArrayLen();
    int send(int fd, char *buf, int n);
    int recv(int fd, char *buf, int input_len, int output_len);
    int close(int fd);

 private:
    char array[2];
    std::unordered_map<int, std::list<DataPathModule *>*> module_table;
};

#endif /* MANGER_H */
