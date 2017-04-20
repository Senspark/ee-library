//
//  GenericLoader.hpp
//  ee-library
//
//  Created by enrevol on 3/9/16.
//
//

#ifndef EE_LIBRARY_GENERIC_LOADER_HPP_
#define EE_LIBRARY_GENERIC_LOADER_HPP_

#include <tuple>
#include <type_traits>

#include "EEMacro.hpp"
#include "EECocos2dxFwd.hpp"

NS_EE_BEGIN
/// Generic template loader for cocosbuilder.
///
/// Simple loader:
/// @code
/// your_node_loader_library->registerNodeLoader(
///     "your_class_name_in_cocosbuilder",
///     ee::GenericLoader<
///         your_class_name,
///         inherited_node_loader /// E.g: if your_class_name
///                               /// inherits from Node, use
///                               /// cocosbuilder::NodeLoader
///     >::loader());
/// @endcode
template <class Np, class Lp, class... Ts>
class GenericLoader final : public Lp {
public:
    using NodeType = Np;
    using ParentLoaderType = Lp;
    using ArgsType = std::tuple<Ts...>;

    static_assert(
        std::is_base_of<cocosbuilder::NodeLoader, ParentLoaderType>::value,
        "ParentLoader should be derived from cocosbuilder::NodeLoader");

    template <class... Us> static GenericLoader* loader(Us&&... args) {
        auto result = new (std::nothrow) GenericLoader();
        if (result != nullptr) {
            result->autorelease();
            result->callback_ =
                [result, data = std::make_tuple(std::forward<Us>(args)...)] {
                return result->internalCreateNode(
                    std::integral_constant<bool, sizeof...(Ts) == 0>(), data,
                    std::make_index_sequence<sizeof...(Us)>());
            };
        } else {
            delete result;
            result = nullptr;
        }
        return result;
    }

private:
    virtual NodeType* createNode(cocos2d::Node* parent,
                                 cocosbuilder::CCBReader* reader) override {
        return callback_();
    }

    /// Non-templated @c create overload.
    template <class DataType, std::size_t... Indices>
    NodeType* internalCreateNode(std::true_type, DataType&& data,
                                 std::index_sequence<Indices...>) {
        return NodeType::create(
            std::get<Indices>(std::forward<DataType>(data))...);
    }

    /// Templated @c create overload.
    template <class DataType, std::size_t... Indices>
    NodeType* internalCreateNode(std::false_type, DataType&& data,
                                 std::index_sequence<Indices...>) {
        return NodeType::template create<Ts...>(
            std::get<Indices>(std::forward<DataType>(data))...);
    }

    std::function<NodeType*()> callback_;
};
NS_EE_END

#endif /* EE_LIBRARY_GENERIC_LOADER_HPP_ */
