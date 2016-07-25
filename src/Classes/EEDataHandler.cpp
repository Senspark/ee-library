//
//  EEDataHandler.cpp
//  ee-library
//
//  Created by Zinge on 5/14/16.
//
//

#include "EEDataHandler.hpp"

namespace ee {
std::unordered_set<DataHandler*> DataHandler::handlers_;

DataHandler::DataHandler() { handlers_.insert(this); }

DataHandler::~DataHandler() { handlers_.erase(this); }

void DataHandler::notify(int dataId, const std::string& key,
                         const cocos2d::Value& value) const {
    for (auto&& handler : handlers_) {
        if (handler->notifyCallback_) {
            handler->notifyCallback_(dataId, key, value);
        }
    }
}

bool DataHandler::request(int dataId, const std::string& key,
                          cocos2d::Value& result) const {
    for (auto&& handler : handlers_) {
        if (handler->requestCallback_) {
            if (handler->requestCallback_(dataId, key, result)) {
                return true;
            }
        }
    }
    return false;
}

template <>
bool DataHandler::request(int dataId, const std::string& key) const {
    cocos2d::Value result{false};
    request(dataId, key, result);
    return result.asBool();
}

template <> int DataHandler::request(int dataId, const std::string& key) const {
    cocos2d::Value result{0};
    request(dataId, key, result);
    return result.asInt();
}

void DataHandler::setNotifyCallback(const NotifyCallback& callback) {
    notifyCallback_ = callback;
}

void DataHandler::setRequestCallback(const RequestCallback& callback) {
    requestCallback_ = callback;
}
} // namespace ee