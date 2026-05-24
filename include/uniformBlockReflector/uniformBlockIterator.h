#pragma once

#include <templates/opengl_iterator.h>
#include <uniformBlockReflector/uniformBlockInfo.h>

namespace shader {
    class uniform_block_iterator : public opengl_template_iterator<uniform_block_iterator, uniform_block_info> {
        using Base = opengl_template_iterator<uniform_block_iterator, uniform_block_info>;

    public:
        GLuint index = 0;
        GLint binding = 0;

        uniform_block_iterator() = default;
        uniform_block_iterator(const std::string& name, GLint program) : Base(name, program) {}

        void add_block(uniform_block_info&& block) {
            this->add_entry(std::move(block));
        }

        void print(std::ostream& os, int indent = 0) const override {
            os << std::string(indent, ' ') << "Uniform Block Array: " << this->name << "\n";
            os << std::string(indent + 2, ' ') << "Count: " << this->entries.size() << "\n";
            for (size_t i = 0; i < this->entries.size(); ++i) {
                os << std::string(indent + 2, ' ') << "[" << i << "]\n";
                this->entries[0].print(os, indent + 4);
            }
        }
    };

}