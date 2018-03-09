#pragma once
#include <tuple>
#include <utility>
#include <core/sstring.hh>
#include "../ColumnTrait.hpp"

namespace cql {
	/**
	 * A sequence of [bytes] values representing the items in a tuple.
	 * The encoding of each element depends on the data type for that position in the tuple.
	 * Null values may be represented by using length -1 for the [bytes] representing of an element.
	 */
	template <class... Types>
	class Tuple {
	public:
		using UnderlyingType = std::tuple<Types...>;
		template <std::size_t Index>
		using ElementType = typename std::tuple_element_t<Index, UnderlyingType>;

		/** Get the tuple value */
		const UnderlyingType& get() const& { return value_; }

		/** Get the mutable tuple value */
		UnderlyingType& get() & { return value_; }

		/** Get the nth element of tuple */
		template <std::size_t Index>
		const ElementType<Index>& get() const& { return std::get<Index>(value_); }

		/** Get the nth element of tuple */
		template <std::size_t Index>
		ElementType<Index>& get() & { return std::get<Index>(value_); }

		/** Set the tuple value */
		void set(const UnderlyingType& value) { value_ = value; }

		/** Set the tuple value */
		void set(UnderlyingType&& value) { value_ = std::move(value); }

		/** Set the tuple value by it's items */
		template <class... Args>
		void set(Args&&... args) {
			static_assert(sizeof...(Args) == sizeof...(Types), "check args count");
			setImpl<0>(std::forward<Args>(args)...);
		}

		/** Set the nth element of tuple */
		template <std::size_t Index, class... Args>
		void set(Args&&... args) {
			std::get<Index>(value_).set(std::forward<Args>(args)...);
		}

		/** Reset to initial state */
		void reset() { resetImpl<0>(); }

		/** Encode to binary data */
		void encodeBody(seastar::sstring& data) {
			encodeBodyImpl<0>(data);
		}

		/** Decode from binary data */
		void decodeBody(const char* ptr, ColumnEncodeDecodeSizeType size) {
			decodeBodyImpl<0>(ptr, ptr + size);
		}

		/** Constructors */
		Tuple() : value_() { }
		template <class... Args>
		explicit Tuple(Args&&... args) : value_() {
			set(std::forward<Args>(args)...);
		}

		/** Dereference operation */
		const UnderlyingType& operator*() const& { return value_; }
		UnderlyingType& operator*() & { return value_; }

		/** Get pointer operation */
		const UnderlyingType* operator->() const& { return &value_; }
		UnderlyingType* operator->() & { return &value_; }

	private:
		/** Set the tuple value by it's items, the implementation */
		template <std::size_t>
		void setImpl() { }
		template <std::size_t Index, class Head, class... Rest>
		void setImpl(Head&& head, Rest&&... rest) {
			std::get<Index>(value_).set(std::forward<Head>(head));
			setImpl<Index+1>(std::forward<Rest>(rest)...);
		}

		/** Reset to initial state, the implementation */
		template <std::size_t Index, std::enable_if_t<Index >= sizeof...(Types), int> = 0>
		void resetImpl() { }
		template <std::size_t Index, std::enable_if_t<Index < sizeof...(Types), int> = 0>
		void resetImpl() {
			std::get<Index>(value_).reset();
			resetImpl<Index+1>();
		}

		/** Encode to binary data, the implementation */
		template <std::size_t Index, std::enable_if_t<Index >= sizeof...(Types), int> = 0>
		void encodeBodyImpl(seastar::sstring&) { }
		template <std::size_t Index, std::enable_if_t<Index < sizeof...(Types), int> = 0>
		void encodeBodyImpl(seastar::sstring& data) {
			ColumnTrait<ElementType<Index>>::encode(std::get<Index>(value_), data);
			encodeBodyImpl<Index+1>(data);
		}

		/** Decode from binary data, the implementation */
		template <std::size_t Index, std::enable_if_t<Index >= sizeof...(Types), int> = 0>
		void decodeBodyImpl(const char*, const char*) { }
		template <std::size_t Index, std::enable_if_t<Index < sizeof...(Types), int> = 0>
		void decodeBodyImpl(const char* ptr, const char* end) {
			ColumnTrait<ElementType<Index>>::decode(std::get<Index>(value_), ptr, end);
			decodeBodyImpl<Index+1>(ptr, end);
		}

	private:
		UnderlyingType value_;
	};
}

