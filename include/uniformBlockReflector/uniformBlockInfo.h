#pragma once

#include <uniformBlockReflector/uniformBlockMember.h>

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

    GLint offset = 0;

    uniform_block_info() = default;
    uniform_block_info(const std::string& name) : Base(name) {}

    void add_member(uniform_block_member&& member) {
        this->entries[member.name] = std::move(member);
    }

    void print(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ') << "Uniform Block: " << this->name << "\n";
        os << std::string(indent + 2, ' ') << "Index: " << index << "\n";
        os << std::string(indent + 2, ' ') << "Binding: " << binding << "\n";
        os << std::string(indent + 2, ' ') << "Size: " << byte_size << " bytes\n";
        os << std::string(indent + 2, ' ') << "Active uniforms: " << activeUniforms << "\n";
        os << std::string(indent + 2, ' ') << "Members:\n";
        for (const auto& [name, member] : this->entries) {
            member.print(os, indent + 4);
        }
    }
};