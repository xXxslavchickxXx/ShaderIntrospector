#include <uniformReflector/uniformReflector.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace shader {

    UniformReflector::UniformReflector(GLuint id) {
        glUseProgram(id);
        reflectUniforms(id);
        setDefaults();
    }

    void UniformReflector::setDefaults() {
        for (auto& [name, unit] : uniforms) {
            setDefault(unit.location, unit.type);
        }
    }

    void UniformReflector::setDefault(GLint loc, GLenum type) {
        switch (type) {
            case GL_FLOAT:              glUniform1f(loc, 0.0f); break;
            case GL_FLOAT_VEC2:         glUniform2f(loc, 1.0f, 1.0f); break;
            case GL_FLOAT_VEC3:         glUniform3f(loc, 1.0f, 1.0f, 1.0f); break;
            case GL_FLOAT_VEC4:         glUniform4f(loc, 1.0f, 1.0f, 1.0f, 1.0f); break;
            case GL_FLOAT_MAT2: {
                glm::mat2 identity(1.0f);
                glUniformMatrix2fv(loc, 1, GL_FALSE, glm::value_ptr(identity));
                break;
            }
            case GL_FLOAT_MAT3: {
                glm::mat3 identity(1.0f);
                glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(identity));
                break;
            }
            case GL_FLOAT_MAT4: {
                glm::mat4 identity(1.0f);
                glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(identity));
                break;
            }
            case GL_INT:
            case GL_SAMPLER_2D:
            case GL_SAMPLER_CUBE:
                glUniform1i(loc, 0);
                break;
            case GL_INT_VEC2:           glUniform2i(loc, 1, 1); break;
            case GL_INT_VEC3:           glUniform3i(loc, 1, 1, 1); break;
            case GL_INT_VEC4:           glUniform4i(loc, 1, 1, 1, 1); break;
            case GL_UNSIGNED_INT:       glUniform1ui(loc, 0u); break;
            case GL_UNSIGNED_INT_VEC2:  glUniform2ui(loc, 1u, 1u); break;
            case GL_UNSIGNED_INT_VEC3:  glUniform3ui(loc, 1u, 1u, 1u); break;
            case GL_UNSIGNED_INT_VEC4:  glUniform4ui(loc, 1u, 1u, 1u, 1u); break;
            case GL_BOOL:               glUniform1i(loc, GL_FALSE); break;
            case GL_BOOL_VEC2:          glUniform2i(loc, GL_TRUE, GL_TRUE); break;
            case GL_BOOL_VEC3:          glUniform3i(loc, GL_TRUE, GL_TRUE, GL_TRUE); break;
            case GL_BOOL_VEC4:          glUniform4i(loc, GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); break;
            default:
                std::cerr << "Warning: unknown uniform type: " << type << std::endl;
                break;
        }
    }

    const uniform_entry& UniformReflector::getUniform(const std::string& name) const {
        auto iter = uniforms.find(name);
        if (iter == uniforms.end()) {
            throw std::out_of_range("this uniform doesn't exist in this shader: " + name);
        }
        return iter->second;
    }

    void UniformReflector::reflectUniforms(GLint newId) {
        GLint numUniforms = 0;
        glGetProgramiv(newId, GL_ACTIVE_UNIFORMS, &numUniforms);

        GLint maxNameLength = 0;
        glGetProgramiv(newId, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxNameLength);
        
        if (maxNameLength <= 0) {
            maxNameLength = 256;
        }
        
        std::vector<GLchar> nameBuffer(maxNameLength);

        for (GLint i = 0; i < numUniforms; i++) {
            GLsizei length = 0;
            GLint elements = 0;
            GLenum type = 0;

            glGetActiveUniform(newId, i, nameBuffer.size(), &length,
                &elements, &type, nameBuffer.data());

            std::string name(nameBuffer.data(), length);

            if (elements > 1) {
                name = name.substr(0, name.find("[0]"));
            }

            GLint loc = glGetUniformLocation(newId, name.c_str());
            if (loc == -1) {
                continue;
            }

            uniforms[name] = { name, loc, type, elements, newId };
        }
    }

    void UniformReflector::checkUniformValue(const std::string& name, GLenum type) {
        auto it = uniforms.find(name);
        if (it == uniforms.end()) {
            std::cerr << "Uniform not found: " << name << std::endl;
            return;
        }
        
        if (it->second.type != type) {
            std::cerr << "Type mismatch for uniform: " << name << std::endl;
        }
    }

} // namespace shader