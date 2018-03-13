#pragma once
#include "./ProtocolString.hpp"
#include "./ProtocolColumnOption.hpp"

namespace cql {
	/**
	 * Specifies the columns returned in the query
	 * Format: (<ksname><tablename>)?<name><type>
	 * The initial <ksname> and <tablename> are two [string] and are only present if the
	 * GlobalTablesSpec flag is not set.
	 * The <name> is a [string] and <type> is an [option] that corresponds to the description,
	 * what this description is depends a bit on the context:
	 * In results to selects, this will be either the user chosen alias or the selection used;
	 * In results to a PREPARE, this will be either the name of the corresponding bind variable or
	 * the column name for the variable if it is "anonymous".
	 * Check native_protocol_v4.spec section 4.2.5.2.
	 */
	class ProtocolResultColumn {
	public:
		/** Reset to initial state */
		void reset();

		/** The keyspace name the column belongs to */
		const std::string& getKeySpace() const& { return keySpace_.get(); }
		std::string& getKeySpace() & { return keySpace_.get(); }
		void setKeySpace(const std::string& keySpace) { keySpace_.set(keySpace); }
		void setKeySpace(std::string&& keySpace) { keySpace_.set(std::move(keySpace)); }

		/** The table name the column belongs to */
		const std::string& getTable() const& { return table_.get(); }
		std::string& getTable() & { return table_.get(); }
		void setTable(const std::string& table) { table_.set(table); }
		void setTable(std::string&& table) { table_.set(std::move(table)); }

		/** The name of the column */
		const std::string& getName() const& { return name_.get(); }
		std::string& getName() & { return name_.get(); }
		void setName(const std::string& name) { name_.set(name); }
		void setName(std::string&& name) { name_.set(std::move(name)); }

		/** The type of the column */
		const ProtocolColumnOption& getType() const& { return type_; }
		ProtocolColumnOption& getType() & { return type_; }
		void setType(const ProtocolColumnOption& type) { type_ = type; }
		void setType(ProtocolColumnOption&& type) { type_ = std::move(type); }

		/** Encode and decode functions */
		void encode(std::string& data, bool containsTableSpec) const;
		void decode(const char*& ptr, const char* end, bool containsTableSpec);

		/** Constructor */
		ProtocolResultColumn();

	private:
		ProtocolString keySpace_;
		ProtocolString table_;
		ProtocolString name_;
		ProtocolColumnOption type_;
	};
}

