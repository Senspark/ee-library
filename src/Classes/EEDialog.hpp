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
#include "EECocos2dxFwd.hpp"

#include <ui/UIWidget.h>

NS_EE_BEGIN
namespace dialog {
class Dialog;
} // namespace dialog

using dialog::Dialog;

namespace dialog {
class Dialog : public cocos2d::ui::Widget {
public:
    using CallbackType = std::function<void(Dialog*)>;
    
    static const int ContainerLocalZOrder;
    static const std::size_t TopLevel;
    
    virtual void show(std::size_t level = TopLevel);
    
    virtual void hide();
    
    std::size_t getDialogLevel() const noexcept;
    
    virtual const cocos2d::Node* getContainer() const;
    virtual cocos2d::Node* getContainer();
    
    virtual void setActive(bool active);
    
    bool isActive() const noexcept;
    
protected:
    friend class DialogManager;
    
    virtual bool init() override;

    virtual void onEnter() override;
    virtual void onExit() override;    
    
    /// Invoked when the user clicked the outside boundary of this dialog.
    virtual void onClickedOutside();
    
private:
    virtual bool hitTest(const cocos2d::Point& pt,
                         const cocos2d::Camera* camera,
                         cocos2d::Vec3* p) const override;
    
    std::size_t dialogLevel_;
    bool isActive_;
};
} // namespace dialog.
NS_EE_END

#endif /* EE_LIBRARY_DIALOG_H */
