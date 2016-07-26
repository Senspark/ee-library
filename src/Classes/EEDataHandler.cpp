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

void DataHandler::save(int dataId, const std::string& key,
                       const cocos2d::Value& value) const {
    for (auto&& handler : handlers_) {
        if (handler->savingCallback_) {
            handler->savingCallback_(dataId, key, value);
        }
    }
}

bool DataHandler::load(int dataId, const std::string& key,
                       cocos2d::Value& result) const {
    for (auto&& handler : handlers_) {
        if (handler->loadingCallback_) {
            if (handler->loadingCallback_(dataId, key, result)) {
                return true;
            }
        }
    }
    return false;
}

void DataHandler::setSavingCallback(const SavingCallback& callback) {
    savingCallback_ = callback;
}

void DataHandler::setLoadingCallback(const LoadingCallback& callback) {
    loadingCallback_ = callback;
}
} // namespace ee