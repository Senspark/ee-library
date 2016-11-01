//
//  EEPool.hpp
//  ee-library
//
//  Created by Zinge on 9/28/16.
//
//

#ifndef EE_LIBRARY_POOL_HPP_
#define EE_LIBRARY_POOL_HPP_

#include <functional>
#include <vector>
#include <queue>

#include <base/CCRefPtr.h>

namespace ee {
template <class T> class Pool {
public:
    using value_type = T;
    using pointer = T*;
    using reference = cocos2d::RefPtr<value_type>;
    using const_reference = const reference&;
    using constructor = std::function<pointer()>;
    using destructor = std::function<void(pointer)>;
    using size_type = std::size_t;

    explicit Pool(const constructor& ctor, const destructor& dtor = nullptr)
        : ctor_(ctor)
        , dtor_(dtor) {}

    size_type size() const { return unused_instances_.size(); }

    size_type capacity() const {
        return using_instances_.size() + unused_instances_.size();
    }

    void reserve(size_type n) {
        auto cap = capacity();
        if (n > cap) {
            auto difference = cap - n;
            for (size_type i = 0; i < difference; ++i) {
                unused_instances_.emplace_back(instantiate());
            }
        }
    }

    pointer pop() {
        check_for_unused_instances();
        if (unused_instances_.empty()) {
            unused_instances_.emplace(instantiate());
        }

        auto instance = unused_instances_.front();
        using_instances_.emplace_back(instance);
        unused_instances_.pop();
        return instance;
    }

    void push(pointer instance, bool destroy = true) {
        if (destroy && dtor_) {
            dtor_(instance);
        }
        unused_instances_.emplace(instance);
    }

protected:
    pointer instantiate() const { return ctor_(); }

    void check_for_unused_instances() {
        auto iter =
            std::remove_if(using_instances_.begin(), using_instances_.end(),
                           [this](const_reference instance) {
                               if (instance->getReferenceCount() == 1) {
                                   push(instance);
                                   return true;
                               }
                               return false;
                           });
        using_instances_.erase(iter, using_instances_.cend());
    }

    std::queue<reference> unused_instances_;
    std::vector<reference> using_instances_;

    constructor ctor_;
    destructor dtor_;
};
} // namespace ee

#endif /* EE_LIBRARY_POOL_HPP_ */
