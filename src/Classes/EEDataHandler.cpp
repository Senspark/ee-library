//
//  EEDataHandler.cpp
//  ee-library
//
//  Created by Zinge on 5/14/16.
//
//

#include <limits>
#include <unordered_set>

#include "EEDataHandler.hpp"

namespace ee {
namespace {
std::map<int, std::unordered_set<DataHandler*>> handlers_;
} // namespace

const int DataHandler::LowestPriority = std::numeric_limits<int>::max();

DataHandler::DataHandler(int priority)
    : priority_{priority} {
    handlers_[priority_].insert(this);
}

DataHandler::~DataHandler() { handlers_[priority_].erase(this); }

DataHandler::DataHandler(const DataHandler& other)
    : priority_{other.priority_}
    , setCallback_{other.setCallback_}
    , getCallback_{other.getCallback_}
    , removeCallback_(other.removeCallback_) {
    handlers_[priority_].insert(this);
}

void DataHandler::set0(int dataId, const std::string& key,
                       const cocos2d::Value& value) const {
    for (auto&& handlers : handlers_) {
        for (auto&& handler : handlers.second) {
            if (handler->setCallback_) {
                handler->setCallback_(dataId, key, value);
            }
        }
    }
}

bool DataHandler::get0(int dataId, const std::string& key,
                       cocos2d::Value& result) const {
    for (auto&& handlers : handlers_) {
        for (auto&& handler : handlers.second) {
            if (handler->getCallback_) {
                if (handler->getCallback_(dataId, key, result)) {
                    return true;
                }
            }
        }
    }
    return false;
}

void DataHandler::remove0(int dataId, const std::string& key) const {
    for (auto&& handlers : handlers_) {
        for (auto&& handler : handlers.second) {
            if (handler->removeCallback_) {
                handler->removeCallback_(dataId, key);
            }
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