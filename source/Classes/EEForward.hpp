//
//  EEForward.hpp
//  ee-library.
//
//  Created by Hoang Le Hai on 7/20/15.
//
//

#ifndef EE_LIBRARY_FORWARD_HPP_
#define EE_LIBRARY_FORWARD_HPP_

#include <bitset>
#include <cstdint>

namespace ee {
enum class ButtonState;
    
class NodeButton;
class Button;
class ActiveNodeButton;
class ActiveButton;
class Dialog;
class CheckBox;
class CheckAllBox;
    
template<class... Args>
class DataInfo;
    
template<class... Args>
class JniDataInfo;
    
template<class... Args>
class EventInfo;
    
template<class... Args>
class JniEventInfo;
    
template<class... Args>
using Event [[deprecated("Use EventInfo instead.")]] = EventInfo<Args...>;
    
template<class MaskType>
class State;
    
using State8   = State<std::uint8_t>;
using State16  = State<std::uint16_t>;
using State32  = State<std::uint32_t>;
using State64  = State<std::uint64_t>;
using State128 = State<std::bitset<128>>;
} // namespace utils

#endif // EE_LIBRARY_FORWARD_HPP_
