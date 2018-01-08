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
		/** Constructor */
		CqlObject(std::unique_ptr<T>&& ptr) :
			ptr_(std::move(ptr)) { }

		/** Constructor */
		CqlObject(CqlObject&&) = default;

		/** Destructor */
		~CqlObject() {
			auto& freeList = getFreeList();
			if (ptr_ != nullptr && freeList.size() < getCapacity()) {
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

		/** Get the thread local storage of objects */
		static std::vector<std::unique_ptr<T>>& getFreeList() {
			static thread_local std::vector<std::unique_ptr<T>> freeList;
			return freeList;
		}

		/** Get the modifiable capacity value, defines how many objects can store in free list per thread */
		static std::size_t& getCapacity() {
			static std::size_t capacity = 128;
			return capacity;
		}

	private:
		std::unique_ptr<T> ptr_;
	};

	/** Allocate object */
	template <class T, class... Args>
	CqlObject<T> makeObject(Args&&... args) {
		auto& freeList = CqlObject<T>::getFreeList();
		if (freeList.empty()) {
			CqlObject<T> object(std::make_unique<T>());
			object->reset(std::forward<Args>(args)...);
			return object;
		} else {
			auto object = std::move(freeList.back());
			freeList.pop_back();
			object->reset(std::forward<Args>(args)...);
			return object;
		}
	}
}

