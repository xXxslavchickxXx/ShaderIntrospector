#pragma once

#include <templates/opengl_iterator.h>
#include <toString/toString.h>
#include <tools.h>

#include <uniformBlockReflector/uniformBlockField.h>

class uniform_block_view_info : public opengl_template_iterator<uniform_block_view_info, uniform_block_field_info> {
    using Base = opengl_template_iterator<uniform_block_view_info, uniform_block_field_info>;

public:
    GLenum type = 0;
    GLint elementStride = 0;

    GLint offset = 0;

    uniform_block_view_info() = default;
    uniform_block_view_info(const std::string& name, GLint program) : Base(name, program) {}

    // Если не массив — приводим к handle
    operator uniform_block_field_info() const {
        return this->entries[0];
    }

    void print(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ') << this->name;
        if (size() > 1) os << "[" << size() << "]";
        os << " (" << toString(type) << ") @ offset " << offset;
        if (elementStride > 0 && size() > 1) {
            os << " [stride=" << elementStride << "]";
        }
        if (this->program != -1) {
            os << " [program=" << this->program << "]";
        }
        os << "\n";
        // Выводим элементы массива
        for (const auto& elem : this->entries) {
            os << std::string(indent + 2, ' ') << "  - " << elem << "\n";
        }
    }
};