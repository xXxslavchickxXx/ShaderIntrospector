#pragma once

#include <templates/opengl_iterator.h>
#include <uniformBlockReflector/uniformBlockData.h>

namespace shader {
    class uniform_block_sequence_info : public opengl_template_iterator<uniform_block_sequence_info, uniform_block_data_info> {
        using Base = opengl_template_iterator<uniform_block_sequence_info, uniform_block_data_info>;
        
    private:
        uniform_block_data_info& get(size_t i) {
            return this->entries[i];
        }
        friend class uniform_block_reflector;

    public:
        GLint offset = 0;

        uniform_block_sequence_info() = default;
        uniform_block_sequence_info(const std::string& name, GLint program) : Base(name, program) {}
        
        // Если не массив — приводим к handle
        operator uniform_block_data_info() const {
            return this->entries[0];
        }

        const uniform_block_data_info& operator[](size_t i) {
            return this->entries[i];
        }

        const uniform_block_data_info& operator[](size_t i) const {
            return this->entries.at(i);
        }

        void print(std::ostream& os, int indent = 0) const {
            os << std::string(indent, ' ') << "Uniform Block Array: " << this->name << "\n";
            os << std::string(indent + 2, ' ') << "Count: " << this->entries.size() << "\n";
            for (size_t i = 0; i < this->entries.size(); ++i) {
                os << std::string(indent + 2, ' ') << "[" << i << "]\n";
                this->entries[i].print(os, indent + 4);
            }
        }

        friend std::ostream& operator<<(std::ostream& os, const uniform_block_sequence_info& it) {
            it.print(os);
            return os;
        }
    };

}