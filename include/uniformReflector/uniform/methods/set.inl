#include <uniformReflector/traits/traits.h>

namespace shader {
    template<typename T>
    void uniform_entry::set(const T& value) const {
        bindProgram();

        if (type != mapGlType<T>) {
            throw std::runtime_error("type mismatch");
        }

        if constexpr (std::is_same_v<T, float>) {
            if (type == GL_FLOAT) glUniform1f(location, value);
        }
        else if constexpr (std::is_same_v<T, int>) {
            if (type == GL_INT) glUniform1i(location, value);
        }
        else if constexpr (std::is_same_v<T, unsigned int>) {
            if (type == GL_UNSIGNED_INT) glUniform1ui(location, value);
        }
        else if constexpr (std::is_same_v<T, bool>) {
            if (type == GL_BOOL) glUniform1i(location, value ? GL_TRUE : GL_FALSE);
        }
        else if constexpr (std::is_same_v<T, glm::vec2>) {
            if (type == GL_FLOAT_VEC2) glUniform2fv(location, 1, glm::value_ptr(value));
        }
        else if constexpr (std::is_same_v<T, glm::vec3>) {
            if (type == GL_FLOAT_VEC3) glUniform3fv(location, 1, glm::value_ptr(value));
        }
        else if constexpr (std::is_same_v<T, glm::vec4>) {
            if (type == GL_FLOAT_VEC4) glUniform4fv(location, 1, glm::value_ptr(value));
        }
        else if constexpr (std::is_same_v<T, glm::mat4>) {
            if (type == GL_FLOAT_MAT4) glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
        }
        else if constexpr (std::is_same_v<T, glm::ivec2>) {
            if (type == GL_INT_VEC2) glUniform2iv(location, 1, glm::value_ptr(value));
        }
        else if constexpr (std::is_same_v<T, glm::ivec3>) {
            if (type == GL_INT_VEC3) glUniform3iv(location, 1, glm::value_ptr(value));
        }
        else if constexpr (std::is_same_v<T, glm::ivec4>) {
            if (type == GL_INT_VEC4) glUniform4iv(location, 1, glm::value_ptr(value));
        }
        else if constexpr (std::is_same_v<T, GLuint>) {
            if (type == GL_SAMPLER_2D || type == GL_SAMPLER_CUBE) {
                glUniform1i(location, (GLint)value);
            }
        }
    }
}