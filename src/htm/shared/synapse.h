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
