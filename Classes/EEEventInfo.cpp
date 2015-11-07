//
//  EEEvent.cpp
//  roll-eat
//
//  Created by enrevol on 9/19/15.
//
//

#include "EEEventInfo.hpp"
#include "cocos2d.h"

namespace_ee_begin
namespace_detail_begin
void EventInfoBase::removeListeners() const {
    cocos2d::Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(_key);
}
    
cocos2d::EventListenerCustom* EventInfoBase::addListenerHelper(const std::function<void(cocos2d::EventCustom*)>& callback) const {
    return cocos2d::Director::getInstance()->getEventDispatcher()->addCustomEventListener(_key, callback);
}

void EventInfoBase::dispatchHelper(void* data) const {
    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(_key, data);
}

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
class JniEventInfoManager {
public:
    static JniEventInfoManager* getInstance() {
        static JniEventInfoManager sharedInstance;
        return &sharedInstance;
    }
    
    void addInfo(const JniEventInfoBase& info) {
        _infos.push_back(&info);
    }
    
    void dispatch(jlong pointer, jobjectArray objects) const {
        for (auto info : _infos) {
            if ((jlong) info == pointer) {
                (*info).jniDispatch(objects);
                break;
            }
        }
    }
    
private:
    std::vector<const JniEventInfoBase*> _infos;
};

void JniEventInfoBase::initialize(std::string className) const {
    internalInitialize(std::move(className));
    JniEventInfoManager::getInstance()->addInfo(*this);
}

void JniEventInfoBase::initialize(std::string className, std::string fieldName) const {
    internalInitialize(std::move(className), std::move(fieldName));
    JniEventInfoManager::getInstance()->addInfo(*this);
}

const char* JniEventInfoBase::ClassNames[3] = {
    "com/ee/library/EventInfo0",
    "com/ee/library/EventInfo1",
    "com/ee/library/EventInfo2"
};

const char* JniEventInfoBase::ClassSignatures[3] = {
    "Lcom/ee/library/EventInfo0;",
    "Lcom/ee/library/EventInfo1;",
    "Lcom/ee/library/EventInfo2;"
};
#endif
namespace_detail_end
namespace_ee_end

namespace_anonymous_begin
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
void Java_com_senspark_ee_EventInfo_dispatch(JNIEnv* env, jobject thiz, jlong pointer, jobjectArray objects) {
    EE_LOGE("%s", __PRETTY_FUNCTION__);
    ee::detail::JniEventInfoManager::getInstance()->dispatch(pointer, objects);
}
#endif
namespace_anonymous_end