#pragma once

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace shader {

    class uniform_entry {
    public:
        std::string name;
        GLint location = -1;
        GLenum type = 0;
        GLint elements = 0;
        GLint programId = 0;

    private:
        void bindProgram() const {
            GLint currentProgram = 0;
            glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
            if (currentProgram != programId) {
                glUseProgram(programId);
            }
        }

    public:
        template<typename T>
        void set(const T& value) const;
        template<typename T>
        T get() const;

    public:
        uniform_entry operator[](size_t index) const {
            return at(index);
        }

        uniform_entry at(size_t index = 0) const {
            if (index > elements) {
                throw std::out_of_range("out of range uniforms\n");
            }

            if (elements > 1) {
                std::string indexedName = name + "[" + std::to_string(index) + "]";
                GLint indexedLoc = glGetUniformLocation(programId, indexedName.c_str());
                return uniform_entry(indexedName, indexedLoc, type, 1, programId);
            }

            return *this;
        }

    public:
        uniform_entry(std::string_view name, GLint location, GLenum type, GLint elements, GLint programId)
            : name(name)
            , location(location)
            , type(type)
            , elements(elements)
            , programId(programId)
        {}
        uniform_entry() = default;

        uniform_entry(const uniform_entry&) = default;
        uniform_entry& operator=(const uniform_entry&) = default;
        uniform_entry(uniform_entry&&) = default;
        uniform_entry& operator=(uniform_entry&&) = default;

        template<typename T>
        operator T() const;

        template<typename T>
        const uniform_entry& operator=(const T& value) const {
            set(value);
            return *this;
        }
        template<typename T>
        const uniform_entry& operator=(T&& value) const {
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