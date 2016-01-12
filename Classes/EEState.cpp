//
//  EEState.cpp
//  ee-library
//
//  Created by enrevol on 11/30/15.
//
//

#include "EEState.hpp"

namespace_ee_begin
template class State<std::uint8_t>;
template class State<std::uint16_t>;
template class State<std::uint32_t>;
template class State<std::uint64_t>;
template class State<std::bitset<128>>;
namespace_ee_end
