#pragma once

#include <uniformReflector/traits/traits.h>
#include <iostream>

namespace shader {
    template<typename T>
    T uniform_entry::get() const {
        T value{};
        if (!check_program()) return value;

        if (type != mapGlType<T>) {
            std::cerr << type << " " << mapGlType<T>;
            throw std::runtime_error("Uniform type mismatch: " + name);
        }

        if constexpr (std::is_same_v<T, float>) {
            glGetUniformfv(programId, location, &value);
        }
        else if constexpr (std::is_same_v<T, int>) {
            glGetUniformiv(programId, location, &value);
        }
        else if constexpr (std::is_same_v<T, unsigned int>) {
            glGetUniformuiv(programId, location, &value);
        }
        else if constexpr (std::is_same_v<T, bool>) {
            int v; glGetUniformiv(programId, location, &v);
            value = (v != 0);
        }
        else if constexpr (std::is_same_v<T, glm::vec2>) {
            glGetUniformfv(programId, location, glm::value_ptr(value));
        }
        else if constexpr (std::is_same_v<T, glm::vec3>) {
            glGetUniformfv(programId, location, glm::value_ptr(value));
        }
        else if constexpr (std::is_same_v<T, glm::vec4>) {
            glGetUniformfv(programId, location, glm::value_ptr(value));
        }
        else if constexpr (std::is_same_v<T, glm::mat4>) {
            glGetUniformfv(programId, location, glm::value_ptr(value));
        }
        else if constexpr (std::is_same_v<T, glm::ivec2>) {
            glGetUniformiv(programId, location, glm::value_ptr(value));
        }
        else if constexpr (std::is_same_v<T, glm::ivec3>) {
            glGetUniformiv(programId, location, glm::value_ptr(value));
        }
        else if constexpr (std::is_same_v<T, glm::ivec4>) {
            glGetUniformiv(programId, location, glm::value_ptr(value));
        }
        else if constexpr (std::is_same_v<T, GLuint>) {
            int v; glGetUniformiv(programId, location, &v);
            value = (GLuint)v;
        }

        return value;
    }
}