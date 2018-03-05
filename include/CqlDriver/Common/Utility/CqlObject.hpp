#pragma once
#include <vector>
#include <memory>
#include <type_traits>

namespace cql {
	/** Class used to determinate the free list size of the specified type */
	template <class T>
	struct CqlObjectFreeListSize {
		// Up to 32k per type
		static const constexpr std::size_t value = 32768/sizeof(T);
	};

	/**
	 * Reuseable unique pointer, back to free list automatically on destruction.
	 * T should provide two functions:
	 * - freeResources: called at deallocate
	 * - reset: called at allocate, with forwarded parameters
	 * Move CqlObject<Derived> to CqlObject<Base> is valid (polymorphism is supported).
	 * Move CqlObject<Base> to CqlObject<Derived> is also valid (use it carefully).
	 * Incomplete type is supported (however it require the complete definition on construct).
	 */
	template <class T>
	class CqlObject {
	public:
		/** Constructor */
		explicit CqlObject(std::unique_ptr<T>&& ptr) noexcept :
			ptr_(ptr.release()),
			deleter_([](void* ptr) {
				std::unique_ptr<T> tPtr(reinterpret_cast<T*>(ptr));
				auto& freeList = getFreeList();
				if (freeList.size() < CqlObjectFreeListSize<T>::value) {
					tPtr->freeResources();
					freeList.emplace_back(std::move(tPtr));
				} else {
					tPtr.reset(); // call the derived destructor
				}
			}) { }

		/** Move constructor */
		CqlObject(CqlObject&& other) noexcept :
			ptr_(other.ptr_),
			deleter_(other.deleter_) {
			other.ptr_ = nullptr;
		}

		/** Move constructor, T is base of U (up casting) */
		template <class U, std::enable_if_t<std::is_base_of<T, U>::value, int> = 0>
		CqlObject(CqlObject<U>&& other) noexcept :
			ptr_(other.ptr_),
			deleter_(other.deleter_) {
			other.ptr_ = nullptr;
		}

		/**
		 * Move constructor, U is base of T (down casting)
		 * Use static_cast instead of dynamic_cast for performance, use it carefully
		 */
		template <class U, std::enable_if_t<std::is_base_of<U, T>::value, int> = 0>
		CqlObject(CqlObject<U>&& other) noexcept :
			ptr_(static_cast<T*>(other.ptr_)) ,
			deleter_(other.deleter_) {
			other.ptr_ = nullptr;
		}

		/** Move assignment */
		CqlObject& operator=(CqlObject&& other) noexcept {
			if (this != &other) {
				ptr_ = other.ptr_;
				deleter_ = other.deleter_;
				other.ptr_ = nullptr;
			}
			return *this;
		}

		/** Move assignment, T is base of U (up casting) */
		template <class U, std::enable_if_t<std::is_base_of<T, U>::value, int> = 0>
		CqlObject& operator=(CqlObject<U>&& other) noexcept {
			if (this != static_cast<void*>(&other)) {
				ptr_ = other.ptr_;
				deleter_ = other.deleter_;
				other.ptr_ = nullptr;
			}
			return *this;
		}

		/**
		 * Move assignment, U is base of T (down casting)
		 * Use static_cast instead of dynamic_cast for performance, use it carefully
		 */
		template <class U, std::enable_if_t<std::is_base_of<U, T>::value, int> = 0>
		CqlObject& operator=(CqlObject<U>&& other) noexcept {
			if (this != static_cast<void*>(&other)) {
				ptr_ = static_cast<T*>(other.ptr_);
				deleter_ = other.deleter_;
				other.ptr_ = nullptr;
			}
			return *this;
		}

		/** Disallow copy */
		CqlObject(const CqlObject&) = delete;
		CqlObject& operator=(const CqlObject&) = delete;

		/** Destructor */
		~CqlObject() {
			T* ptr = ptr_;
			ptr_ = nullptr;
			if (ptr != nullptr) {
				// it may throws exception, then program abort
				// for now I don't want to catch it
				deleter_(ptr);
			}
		}

		/** Dereference */
		T& operator*() const& {
			return *ptr_;
		}

		/** Get pointer */
		T* operator->() const& {
			return ptr_;
		}

		/** Get pointer */
		T* get() const& {
			return ptr_;
		}

		/** Compare with nullptr */
		bool operator==(std::nullptr_t) const { return ptr_ == nullptr; }
		bool operator!=(std::nullptr_t) const { return ptr_ != nullptr; }

		/** Get the thread local storage of objects */
		static std::vector<std::unique_ptr<T>>& getFreeList() {
			static thread_local std::vector<std::unique_ptr<T>> freeList;
			return freeList;
		}

	private:
		template <class> friend class CqlObject;
		T* ptr_;
		void(*deleter_)(void*);
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
			CqlObject<T> object(std::move(freeList.back()));
			freeList.pop_back();
			object->reset(std::forward<Args>(args)...);
			return object;
		}
	}
}

