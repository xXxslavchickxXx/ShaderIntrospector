#pragma once

#include <string>
#include <GL/glew.h>
#include <ostream>
#include <toString/toString.h>
#include <format>

struct uniform_block_field_info {
    std::string name;
    GLenum type;
    GLint byte_size = 0;

    GLint offset;

    uniform_block_field_info() = default;
    uniform_block_field_info(const std::string& name) : name(name) {}

    friend std::ostream& operator<<(std::ostream& os, const uniform_block_field_info& h) {
        os << std::format("{} @ {}", h.name, h.offset);
        return os;
    }
};