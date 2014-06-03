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

#ifndef COMPRESSION_MODULE_H
#define COMPRESSION_MODULE_H

#include "DataPathModule.h"

class CompressionModule : public DataPathModule {
	public:
		virtual void initialize(Preferences &prefs, ContextualInfo &info, bool initiator);
		virtual bool handshake_done();
		virtual bool ready();
		virtual DataPathState get_required_data_path_state();
		virtual DataPathState get_resulting_data_path_state();
		virtual int data_in(void *buf, size_t *datalen, size_t *buflen);
		virtual int data_out(void *buf, size_t *datalen, size_t *buflen);

	protected:
		int compress(unsigned char *in, size_t in_len, unsigned char *out, size_t out_len);
		int decompress(unsigned char *in, size_t in_len, unsigned char *out, size_t out_len);
};

#endif /* COMPRESSION_MODULE_H */
