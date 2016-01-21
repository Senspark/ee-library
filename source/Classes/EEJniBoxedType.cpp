//
//  EEJniBoxedType.cpp
//  ee-library
//
//  Created by enrevol on 11/12/15.
//
//

#include "EEJniBoxedType.hpp"

namespace_ee_begin
template class BoxedType<std::int8_t>;
template class BoxedType<bool>;
template class BoxedType<std::int16_t>;
template class BoxedType<std::uint8_t>;
template class BoxedType<std::int32_t>;
template class BoxedType<std::int64_t>;
template class BoxedType<float>;
template class BoxedType<double>;
namespace_ee_end
