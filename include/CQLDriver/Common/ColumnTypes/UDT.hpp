#pragma once
#include "./Tuple.hpp"

namespace cql {
	/**
	 * A UDT value is composed of successive [bytes] values,
	 * one for each field of the UDT value (in the order defined by the type).
	 * A UDT value will generally have one value for each field of the type it represents,
	 * but it is allowed to have less values than the type has fields.
	 */
	template <class... Fields>
	class UDT : private Tuple<typename Fields::Type...> {
	public:
		using Base = Tuple<typename Fields::Type...>;
		using Base::UnderlyingType;
		using Base::ElementType;
		using Base::get;
		using Base::set;
		using Base::reset;
		using Base::encodeBody;
		using Base::Tuple;
		using Base::operator*;
		using Base::operator->;
		template <std::size_t Index>
		using FieldInfo = typename std::tuple_element_t<Index, std::tuple<Fields...>>;

		/** Decode from binary data */
		void decodeBody(const char* ptr, ColumnEncodeDecodeSizeType size) {
			decodeBodyImpl<0>(ptr, ptr + size);
		}

	private:
		/** Decode from binary data, the implementation */
		template <std::size_t Index, std::enable_if_t<Index >= sizeof...(Fields), int> = 0>
		void decodeBodyImpl(const char*, const char*) { }
		template <std::size_t Index, std::enable_if_t<Index < sizeof...(Fields), int> = 0>
		void decodeBodyImpl(const char* ptr, const char* end) {
			// it is allowed to have less values than the type has fields
			if (ptr < end) {
				ColumnTrait<typename FieldInfo<Index>::Type>::decode(
					std::get<Index>(get()), ptr, end);
			} else {
				std::get<Index>(get()).reset();
			}
			decodeBodyImpl<Index+1>(ptr, end);
		}
	};
}

