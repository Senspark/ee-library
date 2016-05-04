//
//  EEDialog.h
//  roll-eat
//
//  Created by Hoang Le Hai on 9/1/15.
//
//

#ifndef EE_LIBRARY_DIALOG_H
#define EE_LIBRARY_DIALOG_H

#include <functional>
#include <memory>
#include <vector>

#include "EEMacro.hpp"

#include <2d/CCNode.h>

NS_EE_BEGIN
class Dialog : public cocos2d::Node {
public:
    using Container = cocos2d::Node;
    
    using Callback1 = std::function<void()>;
    using Callback2 = std::function<void(Container* container,
                                         Dialog* dialog)>;
    
    using Command = std::function<Dialog*>;
    
    /// Attempts to push a dialog.
    void pushDialog(const Command& command);
    
    virtual void show(int level);
    
    virtual void hide();
    
    int getDialogLevel() const;
    
    Container* getContainer();
    
    const Container* getContainer() const;
    
    Dialog* addOnShowBeganCallback(const Callback1& callback, int priority = 0);
    Dialog* addOnShowEndedCallback(const Callback2& callback, int priority = 0);
    Dialog* addOnHideBeganCallback(const Callback2& callback, int priority = 0);
    Dialog* addOnHideEndedCallback(const Callback1& callback, int priority = 0);
    
protected:
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
    bool _isShowing;
    bool _isHiding;
    
    using Callback1Info = std::pair<int, Callback1>;
    using Callback2Info = std::pair<int, Callback2>;
    
    void invokeCallback1(std::vector<Callback1Info>& infos);
    void invokeCallback2(std::vector<Callback2Info>& infos);
    
    std::vector<Callback1Info> _onShowBeganCallbacks;
    std::vector<Callback2Info> _onShowEndedCallbacks;
    std::vector<Callback2Info> _onHideBeganCallbacks;
    std::vector<Callback1Info> _onHideEndedCallbacks;
};
NS_EE_END

#endif /* EE_LIBRARY_DIALOG_H */
