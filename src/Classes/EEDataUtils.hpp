//
//  EEDataUtils.hpp
//  ee-library
//
//  Created by Zinge on 7/20/17.
//
//

#ifndef EE_LIBRARY_DATA_UTILS_HPP
#define EE_LIBRARY_DATA_UTILS_HPP

#include <cstddef>

namespace ee {
class Increment final {
public:
    explicit Increment(std::size_t times = 1)
        : times_(times) {}

    template <class T> void operator()(T& value) const {
        for (std::size_t i = 0; i < times_; ++i) {
            ++value;
        }
    }

private:
    std::size_t times_;
};

class Decrement final {
public:
    explicit Decrement(std::size_t times = 1)
        : times_(times) {}

    template <class T> void operator()(T& value) const {
        for (std::size_t i = 0; i < times_; ++i) {
            --value;
        }
    }

private:
    std::size_t times_;
};
} // namespace ee

#endif /* EE_LIBRARY_DATA_UTILS_HPP */
