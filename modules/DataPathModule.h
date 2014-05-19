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

#ifndef DATA_PATH_MODULE_H
#define DATA_PATH_MODULE_H

#include <stdio.h>
#include <pthread.h>
#include <vector>
#include <string>
#include "logging.h"
#include "Preferences.h"
#include "ContextualInfo.h"

using namespace std;

enum DataPathState {
	kPlaintext,
	kObscured,
	kAny
};


class DataPathModule {
	public:

		virtual void initialize(Preferences &prefs, ContextualInfo &info) = 0;

		//virtual void handshake( ... ) = 0; // TODO

		
		/**
		* @brief Get the state this module requires all outbound data to be in.
		*
		* @return The expected DataPathState.
		*/
		virtual DataPathState get_required_data_path_state() = 0;

		/**
		* @brief Get the state outbound data will be in after passing through this
		* 			module.
		*
		* @return The resulting DataPathState.
		*/
		virtual DataPathState get_resulting_data_path_state() = 0;

		//virtual void data_in( ... ) = 0;  // TODO
		//virtual void data_out( ... ) = 0; // TODO

};


#endif /* DATA_PATH_MODULE_H */
