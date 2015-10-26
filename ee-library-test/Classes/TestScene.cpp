#include "TestScene-Impl.hpp"
#include "cocos2d.h"
#include "EEHeader.hpp"
#include "CCBUtils.hpp"

TestScene* TestScene::create() {
    auto ret = new Impl();
    ret->init();
    ret->autorelease();
    return ret;
}

cocos2d::Scene* TestScene::openScene() {
    auto ret = CCBUtils::openScene("TestScene");
    return ret;
}

bool TestScene::Impl::init() {
    if (Node::init() == false) {
        return false;
    }
    
    return true;
}

void TestScene::Impl::onEnter() {
    TestScene::onEnter();
    
    _imageSprite->setScale(getContentSize().width / _imageSprite->getContentSize().width, getContentSize().height / _imageSprite->getContentSize().height);
    
    _rangeLabel->enableOutline(cocos2d::Color4B(0, 0, 0, 255), 2);
    _filterLabel->enableOutline(cocos2d::Color4B(0, 0, 0, 255), 2);
    _timeLabel->enableOutline(cocos2d::Color4B(0, 0, 0, 255), 2);
}

bool TestScene::Impl::onAssignCCBMemberVariable(cocos2d::Ref* pTarget, const char* pMemberVariableName, cocos2d::Node* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK_EX(imageSprite);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK_EX(rangeLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK_EX(filterLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK_EX(timeLabel);
    return false;
}

cocos2d::extension::Control::Handler TestScene::Impl::onResolveCCBCCControlSelector(cocos2d::Ref* pTarget, const char* pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "pressedReset", Impl::pressedReset);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "pressedProcess", Impl::pressedProcess);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "pressed1pxButton", Impl::pressed1pxButton);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "pressed2pxButton", Impl::pressed2pxButton);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "pressed4pxButton", Impl::pressed4pxButton);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "pressed8pxButton", Impl::pressed8pxButton);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "pressed16pxButton", Impl::pressed16pxButton);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "pressed32pxButton", Impl::pressed32pxButton);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "pressed64pxButton", Impl::pressed64pxButton);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "pressed128pxButton", Impl::pressed128pxButton);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "pressed256pxButton", Impl::pressed256pxButton);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "pressedBoxBlur1D", Impl::pressedBoxBlur1D);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "pressedBoxBlur1Dx2", Impl::pressedBoxBlur1Dx2);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "pressedBoxBlur2D", Impl::pressedBoxBlur2D);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "pressedTentBlur1D", Impl::pressedTentBlur1D);
    return nullptr;
}

void TestScene::Impl::pressedReset(cocos2d::Ref*, cocos2d::extension::Control::EventType) {
    _imageSprite->setTexture("background.jpg");
}

void TestScene::Impl::pressedProcess(cocos2d::Ref*, cocos2d::extension::Control::EventType) {
    if (_filter == Filter::None || _range == 0) {
        return;
    }
    float scale = 1.0f;
    int width = (int) _imageSprite->getContentSize().width * scale;
    int height = (int) _imageSprite->getContentSize().height * scale;
    auto render = cocos2d::RenderTexture::create(width, height);
    render->begin();
    _imageSprite->visit();
    render->end();
    
    auto start = std::chrono::system_clock::now().time_since_epoch().count();
    
    static cocos2d::CustomCommand command;
    command.init(std::numeric_limits<float>::max());
    command.func = [this, start, render]() {
        auto image = render->newImage();
        auto current = std::chrono::system_clock::now().time_since_epoch().count();
        cocos2d::log("Render time: %lld ms", (current - start) / 1000);
        switch (_filter) {
            case Filter::BoxBlur1D:
                ee::Image::boxBlur1D(image, _range);
                break;
            case Filter::BoxBlur1Dx2:
                ee::Image::boxBlur1D(image, _range, 2);
                break;
            case Filter::BoxBlur2D:
                ee::Image::boxBlur2D(image, _range);
                break;
            case Filter::TentBlur1D:
                ee::Image::tentBlur1D(image, _range);
                break;
            default: CC_ASSERT(false);
        }
        auto mark = std::chrono::system_clock::now().time_since_epoch().count();
        _timeLabel->setString(cocos2d::StringUtils::format("Processing time: %lld ms", (mark - current) / 1000));
        auto texture = new cocos2d::Texture2D();
        texture->initWithImage(image);
        _imageSprite->setTexture(texture);;
        _imageSprite->setScale(getContentSize().width / _imageSprite->getContentSize().width, getContentSize().height / _imageSprite->getContentSize().height);
        texture->release();
        image->release();
    };
    cocos2d::Director::getInstance()->getRenderer()->addCommand(&command);
}

void TestScene::Impl::pressed1pxButton(cocos2d::Ref*, cocos2d::extension::Control::EventType) { updateRange(1); }
void TestScene::Impl::pressed2pxButton(cocos2d::Ref*, cocos2d::extension::Control::EventType) { updateRange(2); }
void TestScene::Impl::pressed4pxButton(cocos2d::Ref*, cocos2d::extension::Control::EventType) { updateRange(4); }
void TestScene::Impl::pressed8pxButton(cocos2d::Ref*, cocos2d::extension::Control::EventType) { updateRange(8); }
void TestScene::Impl::pressed16pxButton(cocos2d::Ref*, cocos2d::extension::Control::EventType) { updateRange(16); }
void TestScene::Impl::pressed32pxButton(cocos2d::Ref*, cocos2d::extension::Control::EventType) { updateRange(32); }
void TestScene::Impl::pressed64pxButton(cocos2d::Ref*, cocos2d::extension::Control::EventType) { updateRange(64); }
void TestScene::Impl::pressed128pxButton(cocos2d::Ref*, cocos2d::extension::Control::EventType) { updateRange(128); }
void TestScene::Impl::pressed256pxButton(cocos2d::Ref*, cocos2d::extension::Control::EventType) { updateRange(256); }

void TestScene::Impl::pressedBoxBlur1D(cocos2d::Ref*, cocos2d::extension::Control::EventType) { updateFilter(Filter::BoxBlur1D); }
void TestScene::Impl::pressedBoxBlur1Dx2(cocos2d::Ref*, cocos2d::extension::Control::EventType) { updateFilter(Filter::BoxBlur1Dx2); }
void TestScene::Impl::pressedBoxBlur2D(cocos2d::Ref*, cocos2d::extension::Control::EventType) { updateFilter(Filter::BoxBlur2D); }
void TestScene::Impl::pressedTentBlur1D(cocos2d::Ref*, cocos2d::extension::Control::EventType) { updateFilter(Filter::TentBlur1D); }

void TestScene::Impl::updateRange(int range) {
    _range = range;
    _rangeLabel->setString(cocos2d::StringUtils::format("Range: %d px", range));
}

void TestScene::Impl::updateFilter(Filter filter) {
    std::string filterName;
    switch (filter) {
        case Filter::BoxBlur1D: filterName = "Box Blur 1D"; break;
        case Filter::BoxBlur1Dx2: filterName = "Box Blur 1D x2"; break;
        case Filter::BoxBlur2D: filterName = "Box Blur 2D"; break;
        case Filter::TentBlur1D: filterName = "Tent Blur 1D"; break;
        default: CC_ASSERT(false);
    }
    _filter = filter;
    _filterLabel->setString(cocos2d::StringUtils::format("Filter: %s", filterName.c_str()));
}