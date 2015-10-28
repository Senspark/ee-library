//
//  EEDialog.h
//  roll-eat
//
//  Created by Hoang Le Hai on 9/1/15.
//
//

#ifndef EE_LIBRARY_DIALOG_H
#define EE_LIBRARY_DIALOG_H

#include "EEForward.hpp"
#include "EEControl.hpp"

namespace_ee_begin
class Dialog : public NodeButton {
public:
    virtual void show(int localZOrder) = 0;
    virtual void hide() = 0;
    
protected:
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    
    static void pushDialog(cocos2d::Node* container, Dialog* dialog, int localZOrder);
    static void popDialog(Dialog* dialog);
};
namespace_ee_end

#endif /* EE_LIBRARY_DIALOG_H */
