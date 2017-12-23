#pragma once
#include <vector>
#include <memory>

namespace cql {
	/**
	 * Reuseable object, back to free list automatically when destruct
	 * T should provide two functions:
	 * - freeResources: called in deallocate
	 * - reset: called in allocate, with forwarded parameters
	 */
	template <class T>
	class CqlObject {
	public:
		/** How many objects can store in free list per thread */
#if defined(CQL_OBJECT_FREELIST_CAPACITY)
		static const std::size_t Capacity = CQL_OBJECT_FREELIST_CAPACITY;
#else
		static const std::size_t Capacity = 128;
#endif

		/** Constructor */
		CqlObject(std::unique_ptr<T> ptr) :
			ptr_(std::move(ptr)) { }

		/** Destructor */
		~CqlObject() {
			auto& freeList = getFreeList();
			if (freeList.size() < Capacity) {
				ptr_->freeResources();
				freeList.emplace_back(std::move(ptr_));
			}
		}

		/** Dereference */
		typename std::add_lvalue_reference<T>::type operator*() const {
			return ptr_.operator*();
		}

		/** Get pointer */
		typename std::unique_ptr<T>::pointer operator->() const {
			return ptr_.operator->();
		}

		/** Get pointer */
		typename std::unique_ptr<T>::pointer get() const {
			return ptr_.get();
		}

	private:
		static std::vector<T>& getFreeList() {
			static thread_local std::vector<std::unique_ptr<T>> freeList;
			return freeList;
		}

		std::unique_ptr<T> ptr_;
	};

	/** Allocate object */
	template <class T, class... Args>
	CqlObject<T> makeObject(Args&&... args) {
		auto& freeList = CqlObject<T>::getFreeList();
		if (freeList.empty()) {
			auto object = CqlObject<T>(std::make_unique<T>());
			object->reset(std::forward<Args>(args)...);
			return object;
		} else {
			auto object = std::move(freeList.back());
			freeList.popBack();
			object->reset(std::forward<Args>(args)...);
			return object;
		}
	}
}

