//
//  EEDialog-Impl.hpp
//  ee-library
//
//  Created by enrevol on 10/29/15.
//
//

#ifndef EE_LIBRARY_DIALOG_IMPL_HPP
#define EE_LIBRARY_DIALOG_IMPL_HPP

#include "EEDialog.hpp"

#include <vector>

namespace_ee_begin
class Dialog::Impl {
public:
    Impl(Dialog* base);
    
    Dialog* _base;
    
    using Callback1Info = std::pair<int, Callback1>;
    using Callback2Info = std::pair<int, Callback2>;
    
    void invokeCallback1(std::vector<Callback1Info>& infos);
    void invokeCallback2(std::vector<Callback2Info>& infos);
    
    std::vector<Callback1Info> _onShowBeganCallbacks;
    std::vector<Callback2Info> _onShowEndedCallbacks;
    std::vector<Callback2Info> _onHideBeganCallbacks;
    std::vector<Callback1Info> _onHideEndedCallbacks;
};
namespace_ee_end

#endif /* EEDialog_Impl_h */
