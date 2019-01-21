/*!
 * Copyright (C) tkornuta, IBM Corporation 2015-2019
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef __SYNAPSE_H__
#define __SYNAPSE_H__

namespace mic {
namespace htm {

/*!
 * Class storing synapse parameters.
 * @author krocki/tkornuta
 */
class synapse {
public:
	/// Permanence value.
	float permanence;

	// connected to column
	unsigned int column_connection;

	// connected to cell
	unsigned int cell_connection;

	bool mispredicted;

	bool update_queued;

	bool fresh;
};

} /* namespace htm */
} /* namespace mic */

#endif
