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
#include "EEActiveControl.hpp"

#include <functional>

namespace_ee_begin
class Dialog : public ActiveNodeButton {
public:
    using Callback1 = std::function<void()>;
    using Callback2 = std::function<void(cocos2d::Node*, Dialog*)>;
    
    void show(int localZOrder = 0);
    void hide();
    
    Dialog* addOnShowBeganCallback(const Callback1& callback, int priority = 0);
    Dialog* addOnShowEndedCallback(const Callback2& callback, int priority = 0);
    Dialog* addOnHideBeganCallback(const Callback2& callback, int priority = 0);
    Dialog* addOnHideEndedCallback(const Callback1& callback, int priority = 0);
    
protected:
    virtual void internalShow(int localZOrder) = 0;
    virtual void internalHide() = 0;
    
    Dialog();
    virtual ~Dialog();
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    
    void invokeOnShowBeganCallbacks();
    void invokeOnShowEndedCallbacks();
    void invokeOnHideBeganCallbacks();
    void invokeOnHideEndedCallbacks();
    
    static void pushDialog(cocos2d::Node* container, Dialog* dialog, int localZOrder);
    static void popDialog(Dialog* dialog);
    
private:
    class Impl;
    friend class Impl;
    std::unique_ptr<Impl> _impl;
};
namespace_ee_end

#endif /* EE_LIBRARY_DIALOG_H */
