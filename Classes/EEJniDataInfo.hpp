//
// Created by enrevol on 11/5/15.
//

#ifndef EE_LIBRARY_JNI_DATA_INFO_HPP
#define EE_LIBRARY_JNI_DATA_INFO_HPP

#include "EEMacro.hpp"
#include "EEFunctionTraits.hpp"
#include "EEJni.hpp"
#include "EEJniObject.hpp"
#include "EEUtils.hpp"

namespace_ee_begin
class JniDataInfoInitializer final {
public:
    JniDataInfoInitializer(std::string className);

    template<class DataInfo>
    void operator()(const DataInfo& info) const {
        constexpr int Arity = std::tuple_size<typename DataInfo::FormatArgs>::value;
        auto obj = ee::JniObject::create(DataInfoClassNames[Arity], &info);
        ee::setStaticField(_className, info.getKey(), DataInfoClassSignatures[Arity], obj->getInstance());
    }

private:
    std::string _className;

    static const char* DataInfoClassNames[3];
    static const char* DataInfoClassSignatures[3];
};

class JniDataInfoSetter final {
public:
    JniDataInfoSetter(jlong pointer, jobjectArray objects);

    template<class DataInfo>
    void operator()(const DataInfo& info) const {
        if ((jlong) &info == _pointer) {
            setInfo(info, _objects);
        }
    }

private:
    template<class DataType>
    void setInfo(const DataType& info, jobjectArray objects) const {
        auto&& convertedObjects = ee::JniUtils::toVectorJObject(objects);
        constexpr int Arity = std::tuple_size<typename DataType::FormatArgs>::value;
        using Sequence = typename ee::SequenceGenerator<Arity>::Type;
        setInfo(info, convertedObjects, Sequence());
    }

    template<class DataType, std::size_t... S>
    void setInfo(const DataType& info, const std::vector<jobject>& objects, ee::Sequence<S...>) const {
        using ValueType = typename DataType::ValueType;
        using FormatArgs = typename DataType::FormatArgs;
        auto&& value = ee::JniToCppConverter<ValueType>::convert(objects.at(0));
        EE_LOGD("value = %s", ee::toString(value).c_str());
        info.set(value, ee::JniToCppConverter<typename std::tuple_element<S, FormatArgs>::type>::convert(objects.at(S + 1))...);
    }

    jlong _pointer;
    jobjectArray _objects;
};

template<class DataInfoTuple>
class JniDataInfo final {
public:
    /**
     * Constructor.
     * @param className The name of the class, i.e. "com/senspark/rollandeat/Preferences"
     * @param infoTuple The data info tuple.
     */
    JniDataInfo(std::string className, DataInfoTuple infoTuple)
    : _className(std::move(className))
    , _infoTuple(std::move(infoTuple))
    {}

    /**
     * Sets data.
     * @param pointer The address of the data info.
     * @param objects An array of jobjects.
     */
    void set(jlong pointer, jobjectArray objects) const {
        constexpr int Arity = std::tuple_size<DataInfoTuple>::value;
        for (int i = 0; i < Arity; ++i) {
            ee::apply(_infoTuple, i, JniDataInfoSetter(pointer, objects));
        }
    }

    /**
     * Initializes static fields in Java side.
     */
    void init() const {
        constexpr int Arity = std::tuple_size<DataInfoTuple>::value;
        for (int i = 0; i < Arity; ++i) {
            ee::apply(_infoTuple, i, JniDataInfoInitializer(_className));
        }
    }

private:
    std::string _className;
    DataInfoTuple _infoTuple;
};
namespace_ee_end

#endif //EE_LIBRARY_JNI_DATA_INFO_HPP
