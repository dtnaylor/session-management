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

		/**
		* @brief Tell the module to prepare itself for the new session.
		*
		* @param prefs User, App, and Admin preferences
		* @param info Information about the network and the device
		* @param initiator true if this endpoint started the session (so
		* 					the module knows if it's the "server" or "client")
		*/
		virtual void initialize(Preferences &prefs, ContextualInfo &info, bool initiator) = 0;

		/**
		* @brief Test if the module is done sending/receiving handshake messages
		*
		* @return true if the module is done with its handshake; false otherwise
		*/
		virtual bool handshake_done() = 0;

		/**
		* @brief Test if the module is ready to transmit/receive data.
		*
		* @return true if the module is ready; false otherwise
		*/
		virtual bool ready() = 0;
		
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

		/**
		* @brief Process inbound data.
		*
		* The data may change size during processing; if the processed data no
		* longer fits in the original buffer, data_out() allocates a new buffer
		* and frees the original. datalen and buflen will be updated accordingly.
		*
		* @param buf  Buffer of data to process
		* @param datalen  The length of the data (buffer may be bigger)
		* @param buflen  The length of the buffer
		*
		* @return 0 on success; -1 on failure
		*/
		virtual int data_in(void *buf, size_t *datalen, size_t *buflen) = 0;

		/**
		* @brief Process outbound data.
		*
		* The data may change size during processing; if the processed data no
		* longer fits in the original buffer, data_out() allocates a new buffer
		* and frees the original. datalen and buflen will be updated accordingly.
		*
		* @param buf  Buffer of data to process
		* @param datalen  The length of the data (buffer may be bigger)
		* @param buflen  The length of the buffer
		*
		* @return 0 on success; -1 on failure
		*/
		virtual int data_out(void *buf, size_t *datalen, size_t *buflen) = 0;

};


#endif /* DATA_PATH_MODULE_H */
