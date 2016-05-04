//
//  EEButton.hpp
//  ee-library
//
//  Created by Zinge on 4/23/16.
//
//

#ifndef EEButton_hpp
#define EEButton_hpp

#include <array>
#include <functional>

#include "EEMacro.hpp"
#include "EECocos2dxFwd.hpp"
#include "EEExtension.hpp"

#include <ui/UiButton.h>

NS_EE_BEGIN
NS_DETAIL_BEGIN
class ButtonEx;
NS_DETAIL_END

using Button = detail::ButtonEx;

NS_DETAIL_BEGIN
/// Improved button class for @c cocos2d::ui::Button:
///
/// - Correct hit test.
///
/// - Children will be scaled when the button is pressed.
///
/// Use the following code to register the loader.
/// @code
/// your_node_loader_library->registerNodeLoader("class_name_in_cocosbuilder",
///                                              ee::GenericLoader<ee::Button,
///                                              ee::UiButtonLoader>::loader());
/// @endcode
class ButtonEx : public cocos2d::ui::Button {
public:
    using HitTester = std::function<bool(cocos2d::Touch* touch,
                                         Button* button)>;
    
    using TouchCallback = std::function<void(cocos2d::Touch* touch,
                                             cocos2d::Event* event)>;
    
    /// Retrieves the default inside checker.
    static const HitTester& getDefaultHitTester();
    
    /// Default constructor.
    ButtonEx();
    
    /// Default destructor.
    virtual ~ButtonEx();
    
    /// Creates an empty Button.
    /// @return An empty Button instance.
    static ButtonEx* create();
    
    /// Creates a button with custom textures.
    /// @param normalImage      normal state texture name.
    /// @param selectedImage    selected state texture name.
    /// @param disableImage     disabled state texture name.
    /// @param texType          texture resource type.
    /// @see @c cocos2d::ui::Button::TextureResType
    /// @return a Button instance.
    static ButtonEx* create(const std::string& normalImage,
                            const std::string& selectedImage = "",
                            const std::string& disableImage = "",
                            TextureResType texType = TextureResType::LOCAL);
    
    /// Adds a child to the internal container.
    /// @see @c cocos2d::Node::addChild.
    virtual void addChild(Node* child, int localZOrder, int tag) override;
    
    /// Adds a child to the internal container.
    /// @see @c cocos2d::Node::addChild.
    virtual void addChild(Node* child, int localZOrder,
                          const std::string& name) override;
    
    /// Inherits other @c cocos2d::Node::addChild overloads.
    using Node::addChild;
    
    /// @see @c cocos2d::Node::getChildByTag.
    virtual Node* getChildByTag(int tag) const override;
    
    /// @see @c cocos2d::Node::getChildByName.
    virtual Node* getChildByName(const std::string& name) const override;
    
    /// @see @c cocos2d::Node::getChildren.
    virtual cocos2d::Vector<Node*>& getChildren() override;
    
    /// @see @c cocos2d::Node::getChildren.
    virtual const cocos2d::Vector<Node*>& getChildren() const override;
    
    /// @see @c cocos2d::Node::getChildrenCount.
    virtual ssize_t getChildrenCount() const override;
    
    /// @see @c cocos2d::Node::removeChild.
    virtual void removeChild(Node* child, bool cleanup = true) override;
    
    /// @see @c cocos2d::Node::removeAllChildrenWithCleanup.
    virtual void removeAllChildrenWithCleanup(bool cleanup = true) override;
    
    /// @see @c cocos2d::Node::reorderChild.
    virtual void reorderChild(Node* child, int localZOrder) override;
    
    /// @see @c cocos2d::Node::sortAllChildren.
    virtual void sortAllChildren() override;
    
    /// @see @c cocos2d::ui::Button::setContentSize.
    virtual void setContentSize(const cocos2d::Size& contentSize) override;
    
    /// @see @c cocos2d::ui::Widget::hitTest.
    virtual bool hitTest(const cocos2d::Point& point,
                         const cocos2d::Camera* camera,
                         cocos2d::Vec3* p) const override;
    
    virtual bool onTouchBegan(cocos2d::Touch* touch,
                              cocos2d::Event* event) override;
    
    virtual void onTouchMoved(cocos2d::Touch* touch,
                              cocos2d::Event* event) override;
    
    virtual void onTouchEnded(cocos2d::Touch* touch,
                              cocos2d::Event* event) override;
    
    /// @see @c cocos2d::ui::Button::setScale9Enabled.
    virtual void setScale9Enabled(bool enable) override;
    
    virtual std::string getDescription() const override;
    
    /// Assigns the zooming duration for the default animation.
    void setZoomingDuration(float duration);
    
    /// Retrieves the zooming duration for the default animation.
    ///
    /// Default is 0.05f (seconds).
    float getZoomingDuration() const;
    
    void setTouchBeganCallback(const TouchCallback& callback);
    
    void setTouchMovedCallback(const TouchCallback& callback);
    
    void setTouchEndedCallback(const TouchCallback& callback);
    
    /// Retrieves the internal container.
    Widget* getContainer();
    
    /// Retrieves the internal container (@c const version).
    const Widget* getContainer() const;
    
protected:
    virtual bool init() override;
    
    virtual bool init(const std::string& normalImage,
                      const std::string& selectedImage,
                      const std::string& disableImage,
                      TextureResType texType) override;
    
    virtual void initRenderer() override;
    
    virtual void onPressStateChangedToNormal() override;
    virtual void onPressStateChangedToPressed() override;
    virtual void onPressStateChangedToDisabled() override;
    
    virtual void adaptRenderers() override;
    
    virtual Widget* createCloneInstance() override;
    virtual void copySpecialProperties(Widget* model) override;
    
    /// Update texture for current state after loading new texture via
    /// @c loadTextures, @c loadTextureNormal, @c loadTexturePressed
    /// or @c loadTextureDisabled.
    virtual void updateTexture();
    
private:
    float _zoomingDuration;
    
    cocos2d::Touch* _currentTouch;
    cocos2d::Event* _currentEvent;
    
    cocos2d::ui::Widget* _container;
};
NS_DETAIL_END
NS_EE_END

#endif /* EEButton_hpp */