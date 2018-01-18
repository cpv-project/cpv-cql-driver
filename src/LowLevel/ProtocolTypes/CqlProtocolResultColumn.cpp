#include "CqlProtocolResultColumn.hpp"

namespace cql {
	/** Reset to initial state */
	void CqlProtocolResultColumn::reset() {
		keySpace_.reset();
		table_.reset();
		name_.reset();
		type_.reset();
	}

	/** Encode to binary data */
	void CqlProtocolResultColumn::encode(seastar::sstring& data, bool containsTableSpec) const {
		if (containsTableSpec) {
			keySpace_.encode(data);
			table_.encode(data);
		}
		name_.encode(data);
		type_.encode(data);
	}

	/** Decode from binary data */
	void CqlProtocolResultColumn::decode(const char*& ptr, const char* end, bool containsTableSpec) {
		if (containsTableSpec) {
			keySpace_.decode(ptr, end);
			table_.decode(ptr, end);
		}
		name_.decode(ptr, end);
		type_.decode(ptr, end);
	}

	/** Constructor */
	CqlProtocolResultColumn::CqlProtocolResultColumn() :
		keySpace_(),
		table_(),
		name_(),
		type_() { }
}

