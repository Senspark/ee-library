//
//  EEDataInfo.cpp
//  ee-library
//
//  Created by enrevol on 11/7/15.
//
//

#include "EEDataInfo.hpp"
#include "cocos2d.h"

namespace_ee_begin
#pragma mark - SET INFO HELPER

template<> void setInfoHelper(const char* key, const bool& value) {
    cocos2d::UserDefault::getInstance()->setBoolForKey(key, value);
    cocos2d::UserDefault::getInstance()->flush();
}

template<> void setInfoHelper(const char* key, const int& value) {
    cocos2d::UserDefault::getInstance()->setIntegerForKey(key, value);
    cocos2d::UserDefault::getInstance()->flush();
}

template<> void setInfoHelper(const char* key, const std::string& value) {
    cocos2d::UserDefault::getInstance()->setStringForKey(key, value);
    cocos2d::UserDefault::getInstance()->flush();
}

template<> void setInfoHelper(const char* key, const float& value) {
    cocos2d::UserDefault::getInstance()->setFloatForKey(key, value);
    cocos2d::UserDefault::getInstance()->flush();
}

#pragma mark - GET INFO HELPER

template<> bool getInfoHelper(const char* key, const bool& defaultValue) {
    return cocos2d::UserDefault::getInstance()->getBoolForKey(key, defaultValue);
}

template<> int getInfoHelper(const char* key, const int& defaultValue) {
    return cocos2d::UserDefault::getInstance()->getIntegerForKey(key, defaultValue);
}

template<> std::string getInfoHelper(const char* key, const std::string& defaultValue) {
    return cocos2d::UserDefault::getInstance()->getStringForKey(key, defaultValue);
}

template<> float getInfoHelper(const char* key, const float& defaultValue) {
    return cocos2d::UserDefault::getInstance()->getFloatForKey(key, defaultValue);
}

namespace_detail_begin
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
class JniDataInfoManager {
public:
    static JniDataInfoManager* getInstance() {
        static JniDataInfoManager sharedInstance;
        return &sharedInstance;
    }
    
    void addInfo(const JniDataInfoBase& info) {
        _infos.push_back(&info);
    }
    
    jobject get(jlong pointer, jobjectArray objects) const {
        jobject ret = nullptr;
        for (auto info : _infos) {
            if ((jlong) info == pointer) {
                ret = (*info).jniGet(objects);
                break;
            }
        }
        return ret;
    }
    
    void set(jlong pointer, jobjectArray objects) const {
        for (auto info : _infos) {
            if ((jlong) info == pointer) {
                (*info).jniSet(objects);
                break;
            }
        }
    }
    
private:
    std::vector<const JniDataInfoBase*> _infos;
};

void JniDataInfoBase::initialize(std::string className) const {
    internalInitialize(std::move(className));
    JniDataInfoManager::getInstance()->addInfo(*this);
}

void JniDataInfoBase::initialize(std::string className, std::string fieldName) const {
    internalInitialize(std::move(className), std::move(fieldName));
    JniDataInfoManager::getInstance()->addInfo(*this);
}

const char* JniDataInfoBase::ClassNames[3] = {
    "com/ee/library/DataInfo1",
    "com/ee/library/DataInfo2",
    "com/ee/library/DataInfo3"
};

const char* JniDataInfoBase::ClassSignatures[3] = {
    "Lcom/ee/library/DataInfo1;",
    "Lcom/ee/library/DataInfo2;",
    "Lcom/ee/library/DataInfo3;"
};
#endif
namespace_detail_end
namespace_ee_end

namespace_anonymous_begin
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
void Java_com_senspark_ee_DataInfo_set(JNIEnv* env, jobject thiz, jlong pointer, jobjectArray objects) {
    EE_LOGD("%s", __PRETTY_FUNCTION__);
    ee::detail::JniDataInfoManager::getInstance()->set(pointer, objects);
}

jobject Java_com_senspark_ee_DataInfo_get(JNIEnv* env, jobject thiz, jlong pointer, jobjectArray objects) {
    EE_LOGD("%s", __PRETTY_FUNCTION__);
    return ee::detail::JniDataInfoManager::getInstance()->get(pointer, objects);
}
#endif
namespace_anonymous_end