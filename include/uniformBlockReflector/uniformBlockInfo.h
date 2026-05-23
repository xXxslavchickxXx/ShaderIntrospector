#pragma once

#include <uniformBlockReflector/uniformBlockReflector.h>
#include <templates/opengl_iterator.h>
#include <toString/toString.h>

#include <iostream>

namespace shader {
    struct uniform_block_handle {
        std::string name;
        GLint offset;
        GLenum type;
        GLint size;

        uniform_block_handle() = default;
        uniform_block_handle(const std::string& name, GLint offset, GLenum type, GLint size = 1)
            : name(name), offset(offset), type(type), size(size) {}

        friend std::ostream& operator<<(std::ostream& os, const uniform_block_handle& handle) {
            os << handle.name << " @ offset " << handle.offset;
            if (handle.size > 1) os << " [array of " << handle.size << "]";
            return os;
        }
    };

    struct uniform_block_member : opengl_template_iterator<uniform_block_member, uniform_block_handle> {
        using Base = opengl_template_iterator<uniform_block_member, uniform_block_handle>;

    public:
        using Base::Base;
        GLenum type = 0;
        GLint offset = 0;
        GLint arrayStride = 0;
        GLint size = 1;
        bool isRowMajor = false;
        
        uniform_block_member(const std::string& name, GLenum type, GLint offset, GLint arrayStride, GLint size, bool rowMajor, GLint program)
            : Base(name, program), type(type), offset(offset), arrayStride(arrayStride), size(size), isRowMajor(rowMajor) {}

        void print(std::ostream& os, int indent = 0) const override {
            os << std::string(indent, ' ') << this->name;
            if (size > 1) os << "[" << size << "]";
            os << " (" << toString(type) << ") @ offset " << offset;
            if (arrayStride > 0 && size > 1) {
                os << " [stride=" << arrayStride << "]";
            }
            os << "\n";
        }
    };

    class uniform_block_info : public template_container_iterator<
        uniform_block_info,
        std::unordered_map<std::string, uniform_block_member>,
        const std::string&,
        uniform_block_member>
    {
        using Base = template_container_iterator<uniform_block_info, std::unordered_map<std::string, uniform_block_member>, const std::string&, uniform_block_member>;

    public:
        GLuint index = 0;
        GLint binding = 0;
        GLint byte_size = 0;
        GLint activeUniforms = 0;

        uniform_block_info() = default;
        uniform_block_info(const std::string& name) : Base(name) {}

        using Base::operator[];

        void add_member(uniform_block_member&& member) {
            this->entries[member.name] = std::move(member);
        }

        void print(std::ostream& os, int indent = 0) const override {
            os << std::string(indent, ' ') << "Uniform Block: " << this->name << "\n";
            os << std::string(indent + 2, ' ') << "Binding: " << binding << "\n";
            os << std::string(indent + 2, ' ') << "Size: " << byte_size << " bytes\n";
            os << std::string(indent + 2, ' ') << "Members:\n";

            for (const auto& [key, value] : this->entries) {
                os << std::string(indent + 4, ' ') << "-> " << key << ":\n";
                value.print(os, indent + 6);
            }
        }

        friend std::ostream& operator<<(std::ostream& os, const uniform_block_info& info) {
            info.print(os);
            return os;
        }
    };
}