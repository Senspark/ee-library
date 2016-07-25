//
//  EEDataInfo.hpp
//  ee-library
//
//  Created by Zinge on 5/14/16.
//
//

#ifndef EE_LIBRARY_DATA_INFO_HPP_
#define EE_LIBRARY_DATA_INFO_HPP_

#include <functional>
#include <tuple>

#include "EEUtils.hpp"

namespace ee {
namespace detail {
class DataInfoBase {};
} // namespace detail

template <int DataId, class Value, class... Keys>
class DataInfo final : public detail::DataInfoBase {
public:
    using ValueType = Value;
    using KeyTypes = std::tuple<Keys...>;

    enum { Id = DataId };

    static std::string createKey(const Keys&... keys) {
        return toString(toString("___", keys)...);
    }
};
} // namespace ee

#endif /* EE_LIBRARY_DATA_INFO_HPP_ */
