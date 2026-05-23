#pragma once

#include <string>
#include <GL/glew.h>
#include <ostream>

struct uniform_block_handle {
    std::string name;
    GLint offset;
    GLenum type;
    GLint byte_size = 0;
    GLint size;  // 1 для скаляра

    uniform_block_handle() = default;
    uniform_block_handle(const std::string& name, GLint offset, GLenum type, GLint byte_size, GLint size = 1)
        : name(name), offset(offset), type(type), size(size), byte_size(byte_size) {}

    friend std::ostream& operator<<(std::ostream& os, const uniform_block_handle& h) {
        os << h.name << " @ " << h.offset;
        if (h.size > 1) os << " [array of " << h.size << "]";
        return os;
    }
};