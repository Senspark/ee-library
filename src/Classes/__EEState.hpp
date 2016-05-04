//
//  EEState.hpp
//  ee-library
//
//  Created by enrevol on 11/30/15.
//
//

#ifndef EE_LIBRARY_STATE_HPP_
#define EE_LIBRARY_STATE_HPP_

#include "EEMacro.hpp"

#include <bitset>
#include "boost/dynamic_bitset.hpp"

NS_EE_BEGIN
template<class T>
class State {
public:
    using MaskType = T;
    
    /**
     * Tests whether this state contains other state.
     * Equivalent to: (mask & other.mask) == other.mask.
     */
    bool test(const State& other) const {
        return (_mask & other._mask) == other._mask;
    }
    
    /**
     * Flips all mask in other mask.
     * Equivalent to: mask = mask ^ other.mask.
     */
    State& flip(const State& other) {
        _mask ^= other._mask;
        return *this;
    }
    
    /**
     * Adds all mask in other mask.
     * Equivalent to: mask = mask | other.mask.
     */
    State& set(const State& other) {
        _mask |= other._mask;
        return *this;
    }
    
    [[deprecated("Use set.")]]
    State& add(const State& other) {
        return set(other);
    }
    
    /**
     * Removes all mask in other mask.
     * Equivalent to: mask = mask & ~other.mask.
     */
    State& clear(const State& other) {
        _mask &= ~other._mask;
        return *this;
    }
    
    [[deprecated("Use clear.")]]
    State& remove(const State& other) {
        return clear(other);
    }
    
    State& operator|=(const State& other) {
        set(other);
        return *this;
    }
    
    State operator|(const State& other) const {
        State result(_mask);
        return result |= other;
    }
    
    State& operator^=(const State& other) {
        return flip(other);
    }
    
    State operator^(const State& other) const {
        State result(_mask);
        return result ^= other;
    }
    
    State& operator&=(const State& other) {
        _mask &= other._mask;
        return *this;
    }
    
    State operator&(const State& other) const {
        State result(_mask);
        return result &= other;
    }
    
    State() = default;
    
    explicit State(const MaskType& mask) : _mask(mask) {}
    
    bool operator==(const State& other) const {
        return _mask == other._mask;
    }
    
    const MaskType& getMask() const {
        return _mask;
    }
    
protected:
    MaskType _mask;
};

extern template class State<std::uint8_t>;
extern template class State<std::uint16_t>;
extern template class State<std::uint32_t>;
extern template class State<std::uint64_t>;
extern template class State<std::bitset<128>>;

using State8   = State<std::uint8_t>;
using State16  = State<std::uint16_t>;
using State32  = State<std::uint32_t>;
using State64  = State<std::uint64_t>;
using State128 = State<std::bitset<128>>;
NS_EE_END

#endif /* EE_LIBRARY_STATE_HPP_ */
