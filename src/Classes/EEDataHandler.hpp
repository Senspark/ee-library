//
//  EEDataManager.hpp
//  ee-library
//
//  Created by enrevol on 3/1/16.
//
//

#ifndef EE_LIBRARY_DATA_HANDLER_HPP_
#define EE_LIBRARY_DATA_HANDLER_HPP_

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "EEMacro.hpp"
#include "EECocos2dxFwd.hpp"
#include "EEExtension.hpp"

NS_EE_BEGIN
using DataListener = std::shared_ptr<std::function<void(const std::string&, const cocos2d::Value&)>>;

class DataHandler {
public:
    using CallbackType = typename DataListener::element_type;
    
    static DataHandler* getInstance();
    
    DataHandler();
    virtual ~DataHandler();
    
    virtual void setBoolForKey(const std::string& key, bool value);
    virtual void setIntegerForKey(const std::string& key, int value);
    virtual void setFloatForKey(const std::string& key, float value);
    virtual void setDoubleForKey(const std::string& key, double value);
    virtual void setStringForKey(const std::string& key, const std::string& value);
    
    virtual bool getBoolForKey(const std::string& key, bool defaultValue);
    virtual int getIntegerForKey(const std::string& key, int defaultValue);
    virtual float getFloatForKey(const std::string& key, float defaultValue);
    virtual double getDoubleForKey(const std::string& key, double defaultValue);
    virtual std::string getStringForKey(const std::string& key, const std::string& defaultValue);
    
    /// Add a listener to data info changes.
    virtual DataListener addListener(const CallbackType& callback);
    
    /// Remove the given listener.
    virtual void removeListener(const DataListener& listener);
    
private:
    virtual void notify(const std::string& key, const cocos2d::Value& value);
    
    std::vector<std::weak_ptr<CallbackType>> _listeners;
};
NS_EE_END

#endif /* EE_LIBRARY_DATA_HANDLER_HPP_ */
