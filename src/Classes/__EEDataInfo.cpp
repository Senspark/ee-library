//
//  EEDataInfo.cpp
//  ee-library
//
//  Created by enrevol on 3/24/16.
//
//

#include "__EEDataInfo.hpp"
#include "__EEDataHandler.hpp"

NS_EE_BEGIN
NS_DETAIL_BEGIN
template<>
void setInfoHelper<bool>(DataHandler* handler, const std::string& key, bool value) {
    if (handler == nullptr) {
        handler = DataHandler::getInstance();
    }
    handler->setBoolForKey(key, value);
}

template<>
void setInfoHelper<int>(DataHandler* handler, const std::string& key, int value) {
    if (handler == nullptr) {
        handler = DataHandler::getInstance();
    }
    handler->setIntegerForKey(key, value);
}

template<>
bool getInfoHelper<bool>(DataHandler* handler, const std::string& key, bool defaultValue) {
    if (handler == nullptr) {
        handler = DataHandler::getInstance();
    }
    return handler->getBoolForKey(key, defaultValue);
}

template<>
int getInfoHelper<int>(DataHandler* handler, const std::string& key, int defaultValue) {
    if (handler == nullptr) {
        handler = DataHandler::getInstance();
    }
    return handler->getIntegerForKey(key, defaultValue);
}
NS_DETAIL_END
NS_EE_END
