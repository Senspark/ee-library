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

DataHandler::DataHandler(const DataHandler& other)
    : setCallback_{other.setCallback_}
    , getCallback_{other.getCallback_}
    , removeCallback_(other.removeCallback_) {
    handlers_.insert(this);
}

void DataHandler::set0(int dataId, const std::string& key,
                       const cocos2d::Value& value) const {
    for (auto&& handler : handlers_) {
        if (handler->setCallback_) {
            handler->setCallback_(dataId, key, value);
        }
    }
}

bool DataHandler::get0(int dataId, const std::string& key,
                       cocos2d::Value& result) const {
    for (auto&& handler : handlers_) {
        if (handler->getCallback_) {
            if (handler->getCallback_(dataId, key, result)) {
                return true;
            }
        }
    }
    return false;
}

void DataHandler::remove0(int dataId, const std::string& key) const {
    for (auto&& handler : handlers_) {
        if (handler->removeCallback_) {
            handler->removeCallback_(dataId, key);
        }
    }
}

void DataHandler::setCallback(const SetCallback& callback) {
    setCallback_ = callback;
}

void DataHandler::setCallback(const GetCallback& callback) {
    getCallback_ = callback;
}

void DataHandler::setCallback(const RemoveCallback& callback) {
    removeCallback_ = callback;
}
} // namespace ee