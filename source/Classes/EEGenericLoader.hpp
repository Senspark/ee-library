//
//  EEGenericLoader.hpp
//  ee-library
//
//  Created by enrevol on 12/3/15.
//
//

#ifndef EE_LIBRARY_GENERIC_LOADER_HPP_
#define EE_LIBRARY_GENERIC_LOADER_HPP_

#include "EEMacro.hpp"

#include "CocosBuilder.h"

#include <tuple>
#include <type_traits>

namespace_ee_begin
template<class NodeType, class ParentLoader, class... Ts>
class GenericLoader : public ParentLoader {
public:
    template<class... Us>
    static GenericLoader* loader(Us&&... args) {
        auto result = new (std::nothrow) GenericLoader();
        if (result != nullptr) {
            result->autorelease();
            auto tuple = std::tuple<Us...>(std::forward<Us>(args)...);
            result->_callback = [result, data = std::move(tuple)] {
                using Indices = std::make_index_sequence<std::tuple_size<decltype(data)>::value>;
                return result->internalCreateNode(std::integral_constant<bool, sizeof...(Ts) == 0>(), data, Indices());
            };
        } else {
            delete result;
            result = nullptr;
        }
        return result;
    }
    
private:
    using ConstructorCallback = std::function<NodeType*()>;
    
    virtual NodeType* createNode(cocos2d::Node* parent, cocosbuilder::CCBReader* reader) {
        return _callback();
    }
    
    template<class DataType, std::size_t... Indices>
    NodeType* internalCreateNode(std::true_type, DataType&& data, std::index_sequence<Indices...>) {
        return NodeType::create(std::get<Indices>(data)...);
    }
    
    template<class DataType, std::size_t... Indices>
    NodeType* internalCreateNode(std::false_type, DataType&& data, std::index_sequence<Indices...>) {
        return NodeType::template create<Ts...>(std::get<Indices>(data)...);
    }
    
    ConstructorCallback _callback;
};
namespace_ee_end

#endif /* EE_LIBRARY_GENERIC_LOADER_HPP_ */
