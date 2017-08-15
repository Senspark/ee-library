//
//  EEShaderManager.cpp
//  ee-library
//
//  Created by Zinge on 8/15/17.
//
//

#include "EEShaderManager.hpp"
#include "EEShader.hpp"

#include <platform/CCPlatformMacros.h>

namespace ee {
ShaderManager& ShaderManager::getInstance() {
    static Self sharedInstance;
    return sharedInstance;
}

void ShaderManager::addShader(const std::string& name,
                              const std::string& vertex,
                              const std::string& fragment) {
    if (shaders_.count(name)) {
        CC_ASSERT(false);
        return;
    }
    shaders_.emplace(name, Shader::create(name, vertex, fragment));
}

cocos2d::GLProgram* ShaderManager::getProgram(const std::string& name) const {
    auto iter = shaders_.find(name);
    if (iter == shaders_.cend()) {
        CC_ASSERT(false);
        return nullptr;
    }
    return iter->second->getProgram();
}

cocos2d::GLProgramState*
ShaderManager::createProgramState(const std::string& name) const {
    auto iter = shaders_.find(name);
    if (iter == shaders_.cend()) {
        CC_ASSERT(false);
        return nullptr;
    }
    return iter->second->createProgramState();
}
} // namespace ee
