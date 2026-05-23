#pragma once

#include <templates/template_iterator.h>
#include <unordered_map>
#include <string>
#include <vector>
#include <uniformBlockReflector/uniformBlockIterator.h>

namespace shader {

    class uniform_block_reflector : public template_container_iterator<
        uniform_block_reflector,
        std::unordered_map<std::string, uniform_block_iterator>,
        const std::string&,
        uniform_block_iterator>
    {
        using Base = template_container_iterator<uniform_block_reflector,
            std::unordered_map<std::string, uniform_block_iterator>,
            const std::string&,
            uniform_block_iterator>;

        GLuint programId = 0;

    public:
        uniform_block_reflector() = default;
        explicit uniform_block_reflector(GLuint programId);

        void reflect(GLuint programId);
        void setBinding(const std::string& blockName, GLuint bindingPoint);

        // Доступ к блоку (обычному или массиву)
        uniform_block_iterator& operator[](const std::string& name) {
            return this->entries[name];
        }

        const uniform_block_iterator& operator[](const std::string& name) const {
            return this->entries.at(name);
        }

        void print(std::ostream& os, int indent = 0) const override {
            os << std::string(indent, ' ') << "=== Uniform Block Reflector ===\n";
            os << std::string(indent, ' ') << "Program ID: " << programId << "\n";
            os << std::string(indent, ' ') << "Blocks:\n";
            for (const auto& [name, iterator] : this->entries) {
                iterator.print(os, indent + 2);
                os << "\n";
            }
        }

        friend std::ostream& operator<<(std::ostream& os, const uniform_block_reflector& ref) {
            ref.print(os);
            return os;
        }
    };

}