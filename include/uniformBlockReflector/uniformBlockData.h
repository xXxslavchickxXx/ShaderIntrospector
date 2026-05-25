#pragma once

#include <uniformBlockReflector/uniformBlockView.h>

#include <unordered_map>

class uniform_block_data_info : public template_container_iterator<
    uniform_block_data_info,
    std::unordered_map<std::string, uniform_block_view_info>,
    const std::string&,
    uniform_block_view_info>
{
    using Base = template_container_iterator<uniform_block_data_info, std::unordered_map<std::string, uniform_block_view_info>, const std::string&, uniform_block_view_info>;

public:
    GLint binding = 0;
    GLuint index = 0;
    GLint byte_size = 0;
    GLint uniforms = 0;
    GLint offset = 0;

    uniform_block_data_info() = default;
    uniform_block_data_info(const std::string& name) : Base(name) {}

    void add_member(uniform_block_view_info&& member) {
        this->entries[member.name] = std::move(member);
    }

    void print(std::ostream& os, int indent = 0) const {
        os << std::string(indent, ' ') << "Uniform Block: " << this->name << "\n";
        os << std::string(indent + 2, ' ') << "Index: " << index << "\n";
        os << std::string(indent + 2, ' ') << "Binding: " << binding << "\n";
        os << std::string(indent + 2, ' ') << "Size: " << byte_size << " bytes\n";
        os << std::string(indent + 2, ' ') << "Active uniforms: " << uniforms << "\n";
        os << std::string(indent + 2, ' ') << "Members:\n";
        for (const auto& [name, member] : this->entries) {
            member.print(os, indent + 4);
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const uniform_block_data_info& it) {
        it.print(os);
        return os;
    }
};