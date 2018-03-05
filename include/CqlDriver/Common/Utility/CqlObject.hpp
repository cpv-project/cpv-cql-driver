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
	 * Reuseable object, back to free list automatically on destruction.
	 * T should provide two functions:
	 * - freeResources: called in deallocate
	 * - reset: called in allocate, with forwarded parameters
	 * Move CqlObject<Derived> to CqlObject<Base> is valid (polymorphism is supported).
	 */
	template <class T>
	class CqlObject {
	public:
		/** Constructor */
		explicit CqlObject(std::unique_ptr<T>&& ptr) :
			ptr_(std::move(ptr)),
			deleter_([](std::unique_ptr<T>&& ptr) {
				auto& freeList = getFreeList();
				if (freeList.size() < CqlObjectFreeListSize<T>::value) {
					ptr->freeResources();
					freeList.emplace_back(std::move(ptr));
				} else {
					ptr.reset(); // call the derived destructor
				}
			}) { }

		/** Move constructor */
		CqlObject(CqlObject&& object) noexcept :
			ptr_(std::move(object.ptr_)),
			deleter_(object.deleter_) { }

		/** Move constructor, T is base of U (up casting) */
		template <class U, std::enable_if_t<std::is_base_of<T, U>::value, int> = 0>
		CqlObject(CqlObject<U>&& object) noexcept :
			ptr_(std::move(object.ptr_)),
			deleter_(reinterpret_cast<decltype(deleter_)>(object.deleter_)) {
			static_assert(sizeof(ptr_) == sizeof(object.ptr_), "ensure unique_ptr<> have same layout");
		}

		/**
		 * Move constructor, U is base of T (down casting)
		 * Use static_cast instead of dynamic_cast for performance concern, use it carefully
		 */
		template <class U, std::enable_if_t<std::is_base_of<U, T>::value, int> = 0>
		CqlObject(CqlObject<U>&& object) noexcept :
			ptr_(std::unique_ptr<T>(static_cast<T*>(object.ptr_.release()))) ,
			deleter_(reinterpret_cast<decltype(deleter_)>(object.deleter_)) {
			static_assert(sizeof(ptr_) == sizeof(object.ptr_), "ensure unique_ptr<> have same layout");
		}

		/** Disallow copy */
		CqlObject(const CqlObject&) = delete;
		CqlObject& operator=(const CqlObject&) = delete;

		/** Destructor */
		~CqlObject() {
			if (ptr_ != nullptr) {
				deleter_(std::move(ptr_));
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

	private:
		template <class> friend class CqlObject;
		std::unique_ptr<T> ptr_;
		void(*deleter_)(std::unique_ptr<T>&&);
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

