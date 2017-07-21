//
//  EEDataHandler.cpp
//  ee-library
//
//  Created by Zinge on 5/14/16.
//
//

#include <cassert>
#include <limits>
#include <map>
#include <unordered_set>

#include "EEDataHandler.hpp"
#include "EEDataUtils.hpp"

namespace ee {
namespace {
bool isDispatching_ = false;

class DispatchGuard {
public:
    explicit DispatchGuard(bool& flag)
        : flag_(flag) {}

    bool lock() {
        if (flag_) {
            // The data handler is in dispatch state.
            assert(false);
            return false;
        }
        flag_ = true;
        return true;
    }

    void unlock() {
        assert(flag_);
        flag_ = false;
    }

    ~DispatchGuard() { unlock(); }

private:
    bool& flag_;
};

std::map<int, std::unordered_set<DataHandler*>> handlers_;
} // namespace

namespace detail {
void set0(std::size_t dataId, const std::string& key,
          const std::string& value) {
    DispatchGuard guard(isDispatching_);
    if (not guard.lock()) {
        return;
    }
    for (auto&& handlers : handlers_) {
        for (auto&& handler : handlers.second) {
            if (handler->setCallback_) {
                handler->setCallback_(dataId, key, value);
            }
        }
    }
}

bool get0(std::size_t dataId, const std::string& key, std::string& result) {
    DispatchGuard guard(isDispatching_);
    if (not guard.lock()) {
        return false;
    }
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

void remove0(std::size_t dataId, const std::string& key) {
    DispatchGuard guard(isDispatching_);
    if (not guard.lock()) {
        return;
    }
    for (auto&& handlers : handlers_) {
        for (auto&& handler : handlers.second) {
            if (handler->removeCallback_) {
                handler->removeCallback_(dataId, key);
            }
        }
    }
}
} // namespace detail

const int DataHandler::LowestPriority = std::numeric_limits<int>::max();

DataHandler::DataHandler(int priority)
    : inserted_(false)
    , priority_(priority) {
    insertHandler();
}

DataHandler::~DataHandler() {
    eraseHandler();
}

void DataHandler::insertHandler() {
    DispatchGuard guard(isDispatching_);
    if (not guard.lock()) {
        return;
    }
    inserted_ = true;
    assert(handlers_[priority_].count(this) == 0);
    handlers_[priority_].insert(this);
}

void DataHandler::eraseHandler() {
    DispatchGuard guard(isDispatching_);
    if (not guard.lock()) {
        return;
    }
    if (not inserted_) {
        return;
    }
    assert(handlers_[priority_].count(this) != 0);
    handlers_[priority_].erase(this);
}

DataHandler& DataHandler::setPriority(int priority) {
    DispatchGuard guard(isDispatching_);
    if (not guard.lock()) {
        return *this;
    }
    eraseHandler();
    priority_ = priority;
    insertHandler();
    return *this;
}

DataHandler& DataHandler::setCallback(const SetCallback& callback) {
    setCallback_ = callback;
    return *this;
}

DataHandler& DataHandler::setCallback(const GetCallback& callback) {
    getCallback_ = callback;
    return *this;
}

DataHandler& DataHandler::setCallback(const RemoveCallback& callback) {
    removeCallback_ = callback;
    return *this;
}
} // namespace ee
