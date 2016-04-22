//
//  EEForward.hpp
//  ee-library.
//
//  Created by Hoang Le Hai on 7/20/15.
//
//

#ifndef EE_LIBRARY_FORWARD_HPP_
#define EE_LIBRARY_FORWARD_HPP_

/// Forward ee library classes.

namespace ee {
template<class Np, class Lp, class... Ts>
class GenericLoader;

class UiButtonLoader;
    
template<class... Args>
class EventInfo;
    
template<class... Args>
class DataInfo;
    
class DataHandler;    
} // namespace ee

#endif // EE_LIBRARY_FORWARD_HPP_
