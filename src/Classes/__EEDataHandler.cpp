//
//  EEDataManager.cpp
//  ee-library
//
//  Created by enrevol on 3/1/16.
//
//

#include "__EEDataHandler.hpp"

#include "cocos2d.h"

NS_EE_BEGIN
DataHandler* DataHandler::getInstance() {
    static DataHandler sharedInstance;
    return &sharedInstance;
}

DataHandler::DataHandler() {
}

DataHandler::~DataHandler() {
}

void DataHandler::setBoolForKey(const std::string& key, bool value) {
    cocos2d::UserDefault::getInstance()->setBoolForKey(key.c_str(), value);
    notify(key, cocos2d::Value(value));
}

void DataHandler::setIntegerForKey(const std::string& key, int value) {
    cocos2d::UserDefault::getInstance()->setIntegerForKey(key.c_str(), value);
    notify(key, cocos2d::Value(value));
}

void DataHandler::setFloatForKey(const std::string& key, float value) {
    cocos2d::UserDefault::getInstance()->setFloatForKey(key.c_str(), value);
    notify(key, cocos2d::Value(value));
}

void DataHandler::setDoubleForKey(const std::string& key, double value) {
    cocos2d::UserDefault::getInstance()->setDoubleForKey(key.c_str(), value);
    notify(key, cocos2d::Value(value));
}

void DataHandler::setStringForKey(const std::string& key, const std::string& value) {
    cocos2d::UserDefault::getInstance()->setStringForKey(key.c_str(), value);
    notify(key, cocos2d::Value(value));
}

bool DataHandler::getBoolForKey(const std::string& key, bool defaultValue) {
    return cocos2d::UserDefault::getInstance()->getBoolForKey(key.c_str(), defaultValue);
}

int DataHandler::getIntegerForKey(const std::string& key, int defaultValue) {
    return cocos2d::UserDefault::getInstance()->getIntegerForKey(key.c_str(), defaultValue);
}

float DataHandler::getFloatForKey(const std::string& key, float defaultValue) {
    return cocos2d::UserDefault::getInstance()->getFloatForKey(key.c_str(), defaultValue);
}

double DataHandler::getDoubleForKey(const std::string& key, double defaultValue) {
    return cocos2d::UserDefault::getInstance()->getDoubleForKey(key.c_str(), defaultValue);
}

std::string DataHandler::getStringForKey(const std::string& key, const std::string& defaultValue) {
    return cocos2d::UserDefault::getInstance()->getStringForKey(key.c_str(), defaultValue);
}

DataListener DataHandler::addListener(const CallbackType& callback) {
    auto listener = std::make_shared<CallbackType>(callback);
    _listeners.emplace_back(listener);
    return listener;
}

void DataHandler::removeListener(const DataListener& listener) {
    _listeners.erase(std::remove_if(_listeners.begin(), _listeners.end(), [&listener](auto&& element) {
        if (element.expired()) {
            return true;
        }
        auto&& e = element.lock();
        return e == listener;
    }), _listeners.cend());
}

void DataHandler::notify(const std::string& key, const cocos2d::Value& value) {
    _listeners.erase(std::remove_if(_listeners.begin(), _listeners.end(), [&key, &value](auto&& element) {
        if (element.expired()) {
            return true;
        }
        auto&& e = element.lock();
        e->operator()(key, value);
        return false;
    }), _listeners.cend());
    cocos2d::UserDefault::getInstance()->flush();
}
NS_EE_END