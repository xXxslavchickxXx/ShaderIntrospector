#include <uniformReflector/uniformReflector.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace shader {
    const uniform_entry& uniform_reflector::operator[](const std::string& name) const {
        static const uniform_entry empty_entry{ "", -1, 0, 0, 0 };
        auto iter = uniforms.find(name);
        if (iter == uniforms.end()) {
            std::cerr << "Uniform doesn't exist: " << name << std::endl;
            return empty_entry;
        }
        return iter->second;
    }
    const uniform_entry& uniform_reflector::getUniform(const std::string& name) const {
        auto iter = uniforms.find(name);
        if (iter == uniforms.end()) {
            throw std::out_of_range("this uniform doesn't exist in this shader: " + name);
        }
        return iter->second;
    }
    uniform_entry& uniform_reflector::operator[](const std::string& name) {
        static uniform_entry empty_entry{ "", -1, 0, 0, 0 };
        auto iter = uniforms.find(name);
        if (iter == uniforms.end()) {
            std::cerr << "Uniform doesn't exist: " << name << std::endl;
            return empty_entry;
        }
        return iter->second;
    }
    uniform_entry& uniform_reflector::getUniform(const std::string& name) {
        auto iter = uniforms.find(name);
        if (iter == uniforms.end()) {
            throw std::out_of_range("this uniform doesn't exist in this shader: " + name);
        }
        return iter->second;
    }

    uniform_reflector::uniform_reflector(GLuint id) {
        if (!program_is_valid(id)) throw std::runtime_error("program doesn't exist");
        reflectUniforms(id);
    }

    void uniform_reflector::reflectUniforms(GLint newId) {
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

    std::ostream& operator<<(std::ostream& os, const uniform_reflector& reflector) {
        os << "=== Shader Uniforms ===\n";
        if (reflector.uniforms.empty()) {
            os << "  (no uniforms)\n";
            return os;
        }

        os << "\n";
        size_t count = 0;
        for (const auto& [name, unit] : reflector.uniforms) {
            os << unit;
            if (++count < reflector.uniforms.size()) {
                os << "\n\n";
            }
        }
        os << "\n";
        return os;
    }
}