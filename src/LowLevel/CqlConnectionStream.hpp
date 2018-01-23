#pragma once
#include <cstdint>
#include <core/shared_ptr.hh>

namespace cql {
	/** RAII class that hold an in use stream id for the database connection */
	class CqlConnectionStream {
	public:
		/** Shared state */
		struct State { bool isInUse; State() : isInUse(false) { } };

		/** Get the stream id */
		std::uint16_t getStreamId() const { return streamId_; }

		/** Get whether this object hold a valid and in use stream id */
		bool isValid() const { return state_.get() != nullptr; }

		/** Constructor */
		CqlConnectionStream(std::uint16_t streamId, const seastar::lw_shared_ptr<State>& state);

		/** Disallow copy */
		CqlConnectionStream(const CqlConnectionStream&) = delete;
		CqlConnectionStream& operator=(const CqlConnectionStream&) = delete;

		/** Allow move */
		CqlConnectionStream(CqlConnectionStream&& stream);
		CqlConnectionStream& operator=(CqlConnectionStream&& stream);

		/** Destructor */
		~CqlConnectionStream();

	private:
		std::uint16_t streamId_;
		seastar::lw_shared_ptr<State> state_;
	};
}

