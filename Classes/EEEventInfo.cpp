//
//  EEEvent.cpp
//  roll-eat
//
//  Created by enrevol on 9/19/15.
//
//

#include "EEEventInfo.hpp"
#include "cocos2d.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#   include "EEJniUtils.hpp"
#endif

namespace_ee_begin
namespace_detail_begin
void EventInfoBase::removeListeners() const {
    cocos2d::Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(_key);
}

const std::string& EventInfoBase::getKey() const {
    return _key;
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
        bool ret = false;
        for (auto info : _infos) {
            if ((jlong) info == pointer) {
                (*info).jniDispatch(objects);
                ret = true;
                break;
            }
        }
        CC_ASSERT(ret);
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
extern "C" {
void Java_com_ee_library_EventInfo_dispatch(JNIEnv* env, jobject thiz, jlong pointer, jobjectArray objects) {
    EE_LOGD("%s", __PRETTY_FUNCTION__);
    JNIEnv* currentEnv = ee::JniUtils::getJNIEnv();
    objects = (jobjectArray) env->NewGlobalRef(objects);
    cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([pointer, objects] {
        JNIEnv* oldEnv = ee::JniUtils::getJNIEnv();
        JNIEnv* thisEnv = ee::JniUtils::getJNIEnvAttach();
        ee::JniUtils::setJniEnv(thisEnv);
        ee::detail::JniEventInfoManager::getInstance()->dispatch(pointer, objects);
        thisEnv->DeleteGlobalRef(objects);
        ee::JniUtils::setJniEnv(oldEnv);
    });
}
} // extern "C"
#endif
namespace_anonymous_end