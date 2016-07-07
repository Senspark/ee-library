//
//  EEDataInfo.hpp
//  ee-library
//
//  Created by Zinge on 5/14/16.
//
//

#ifndef EEDataInfo_hpp
#define EEDataInfo_hpp

#include <string>

#include "EEMacro.hpp"
#include "EEUtils.hpp"

NS_EE_BEGIN
enum class DataType {
    Persist,
    Session
};

template<class... Args>
std::string createKey(Args&&... args) {
    return (toString("___", std::forward<Args>(args)) + ...);
}

NS_DETAIL_BEGIN
class BaseData {
public:
    explicit BaseData(DataType type, const std::string& key);
    
    const std::string& getKey() const noexcept;
    
private:
    DataType type_;
    
    std::string key_;
};

template<class T, class... Args>
class BaseDataInfo : public BaseData {
public:
    using ValueType = T;
    using FormatType = std::tuple<Args...>;
    
    using BaseData::BaseData;
};
NS_EE_END

template<class T, class... Args>
class SessionData : public detail::BaseDataInfo<T, Args...> {
public:
//    using detail::BaseDataInfo<T, Args...>::ValueType;
//    using detail::BaseDataInfo<T, Args...>::FormatType;
    
    explicit SessionData(const std::string& key)
    : detail::BaseDataInfo<T, Args...>(key) {}
};

template<class T, class... Args>
class PersistData : public detail::BaseDataInfo<T, Args...> {
public:
//    using detail::BaseDataInfo<T, Args...>::ValueType;
//    using detail::BaseDataInfo<T, Args...>::FormatType;
    
    explicit PersistData(const std::string& key)
    : detail::BaseDataInfo<T, Args...>(key) {}
};
NS_EE_END

#endif /* EEDataInfo_hpp */
