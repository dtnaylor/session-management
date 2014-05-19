/*
** Copyright 2014 Carnegie Mellon University
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**    http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <stdio.h>
#include <pthread.h>
#include <vector>
#include <string>
#include "logging.h"

using namespace std;


enum Party {
	kUser,
	kAdmin,
	kApp
};


class Preferences {
	public:
		bool getCompress(Party party, const std::string &app);
};

#endif /* PREFERENCES_H */
