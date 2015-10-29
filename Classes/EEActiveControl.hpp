//
//  EEActiveControl.hpp
//  ee-library
//
//  Created by enrevol on 10/29/15.
//
//

#ifndef EE_LIBRARY_ACTIVE_CONTROL_HPP
#define EE_LIBRARY_ACTIVE_CONTROL_HPP

#include "EEControl.hpp"

namespace_ee_begin
class ActiveNodeButton : public NodeButton {
public:
    static ActiveNodeButton* create();
    
protected:
    virtual void onEnter() override;
    virtual void onExit() override;
};

class ActiveButton : public Button {
public:
    static ActiveButton* create();
    
protected:
    virtual void onEnter() override;
    virtual void onExit() override;
};
namespace_ee_end

#endif /* EE_LIBRARY_ACTIVE_CONTROL_HPP */
