#pragma once
#include <cstdint>
#include <vector>
#include <seastar/core/shared_ptr.hh>

namespace cql {
	/** RAII class that hold an in use stream id for the database connection */
	class ConnectionStream {
	public:
		using IdType = std::uint16_t;

		/** Get the stream id */
		IdType getStreamId() const;

		/** Get whether this object holds a valid and in use stream id */
		bool isValid() const;

		/** Release this stream */
		void close();

		/** Constructor */
		ConnectionStream();

		/** Constructor */
		ConnectionStream(
			const IdType& streamId,
			const seastar::lw_shared_ptr<std::vector<IdType>>& freeStreamIds);

		/** Disallow copy */
		ConnectionStream(const ConnectionStream&) = delete;
		ConnectionStream& operator=(const ConnectionStream&) = delete;

		/** Allow move */
		ConnectionStream(ConnectionStream&& stream) noexcept;
		ConnectionStream& operator=(ConnectionStream&& stream) noexcept;

		/** Destructor */
		~ConnectionStream();

	private:
		bool isValid_;
		IdType streamId_;
		seastar::lw_shared_ptr<std::vector<IdType>> freeStreamIds_;
	};
}

