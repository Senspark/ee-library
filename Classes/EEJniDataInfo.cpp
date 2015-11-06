//
// Created by enrevol on 11/5/15.
//

#include "EEJniDataInfo.hpp"

namespace_ee_begin
JniDataInfoInitializer::JniDataInfoInitializer(std::string className)
: _className(std::move(className))
{}

const char* JniDataInfoInitializer::DataInfoClassNames[3] = { "com/senspark/ee/DataInfo1", "com/senspark/ee/DataInfo2", "com/senspark/ee/DataInfo3" };
const char* JniDataInfoInitializer::DataInfoClassSignatures[3] = { "Lcom/senspark/ee/DataInfo1;", "Lcom/senspark/ee/DataInfo2;", "Lcom/senspark/ee/DataInfo3;" };

JniDataInfoSetter::JniDataInfoSetter(jlong pointer, jobjectArray objects)
: _pointer(pointer)
, _objects(objects)
{}
namespace_ee_end