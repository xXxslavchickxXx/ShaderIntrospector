#pragma once

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <tools.h>

namespace shader {
    struct attribute_entry {
        std::string name = "";
        GLint location = -1;
        GLenum type = 0;
        GLint elements = 0;
        GLint programId = 0;

        attribute_entry(std::string name, GLint location, GLenum type, GLint elements, GLint programId);
        attribute_entry() = default;

        const attribute_entry operator[](size_t index) const;
        const attribute_entry at(size_t index = 0) const;

        attribute_entry operator[](size_t index);
        attribute_entry at(size_t index = 0);

        size_t size() const { return elements; }

        attribute_entry(const attribute_entry&) = default;
        attribute_entry& operator=(const attribute_entry&) = default;
        attribute_entry(attribute_entry&&) = default;
        attribute_entry& operator=(attribute_entry&&) = default;

        friend std::ostream& operator<<(std::ostream& os, const attribute_entry& entry);

    private:
        void bindProgram() const;
    };
}