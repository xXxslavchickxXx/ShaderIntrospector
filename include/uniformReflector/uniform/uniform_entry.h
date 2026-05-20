#pragma once

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace shader {
    class uniform_entry {
        std::string name = "";
        GLint location = -1;
        GLenum type = 0;
        GLint elements = 0;
        GLint programId = 0;

        void bindProgram() const;
        bool check_program() const;

    public:
        uniform_entry(std::string name, GLint location, GLenum type, GLint elements, GLint programId);
        uniform_entry() = default;

    public:
        template<typename T>
        void set(const T& value);
        template<typename T>
        T get() const;

    public:
        const uniform_entry operator[](size_t index) const;
        const uniform_entry at(size_t index = 0) const;

        uniform_entry operator[](size_t index);
        uniform_entry at(size_t index = 0);

        size_t size() const { return elements; }

        uniform_entry(const uniform_entry&) = default;
        uniform_entry& operator=(const uniform_entry&) = default;
        uniform_entry(uniform_entry&&) = default;
        uniform_entry& operator=(uniform_entry&&) = default;

        template<typename T>
        operator T() const;

        template<typename T>
        uniform_entry& operator=(T&& value) {
            set(std::forward<T>(value));
            return *this;
        }

        friend std::ostream& operator<<(std::ostream& os, const uniform_entry& entry);
    };

}

#include "methods/set.inl"
#include "methods/get.inl"
#include "methods/operator_T.inl"
#include "methods/operator_os.inl"