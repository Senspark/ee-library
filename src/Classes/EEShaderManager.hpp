//
//  EEShaderManager.hpp
//  ee-library
//
//  Created by Zinge on 8/15/17.
//
//

#ifndef EE_LIBRARY_SHADER_MANAGER_HPP
#define EE_LIBRARY_SHADER_MANAGER_HPP

#include <map>
#include <string>

#include "EECocos2dxFwd.hpp"

namespace ee {
class Shader;

class ShaderManager final {
private:
    using Self = ShaderManager;

public:
    static Self& getInstance();

    ShaderManager() = default;
    ~ShaderManager() = default;

    ShaderManager(const Self&) = delete;
    Self& operator=(const Self&) = delete;

    void addShader(const std::string& name, const std::string& vertex,
                   const std::string& fragment);

    cocos2d::GLProgram* getProgram(const std::string& name) const;
    cocos2d::GLProgramState* createProgramState(const std::string& name) const;

private:
    std::map<std::string, std::unique_ptr<Shader>> shaders_;
};
} // namespace ee

#endif /* EE_LIBRARY_SHADER_MANAGER_HPP */
