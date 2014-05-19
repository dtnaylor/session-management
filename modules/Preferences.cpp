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

#include "Preferences.h"


bool Preferences::getCompress(Party party, const std::string &app) {
	// Simulate preferences for compression
	// (for now we're ignoring the app name...)

	switch(party) {
		case kUser:
			return true;
		case kAdmin:
			return true;
		case kApp:
			return false;
		default:
			WARN("Unknown party");
			return false;
	}
}