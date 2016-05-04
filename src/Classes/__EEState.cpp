//
//  EEState.cpp
//  ee-library
//
//  Created by enrevol on 11/30/15.
//
//

#include "EEState.hpp"

NS_EE_BEGIN
template class State<std::uint8_t>;
template class State<std::uint16_t>;
template class State<std::uint32_t>;
template class State<std::uint64_t>;
template class State<std::bitset<128>>;
NS_EE_END