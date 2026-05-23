#pragma once

#include <templates/template_iterator.h>
#include <unordered_map>
#include <string>
#include <vector>
#include <uniformBlockReflector/uniformBlockInfo.h>

namespace shader {

    class uniform_block_reflector : public template_container_iterator<
        uniform_block_reflector,
        std::unordered_map<std::string, uniform_block_info>,
        const std::string&,
        uniform_block_info>
    {
        using Base = template_container_iterator<uniform_block_reflector, std::unordered_map<std::string, uniform_block_info>, const std::string&, uniform_block_info>;

        GLuint programId = 0;

    public:
        uniform_block_reflector() = default;
        explicit uniform_block_reflector(GLuint programId);

        void reflect(GLuint programId);
        void setBinding(const std::string& blockName, GLuint bindingPoint);

        const uniform_block_info& getBlock(const std::string& name) const {
            auto it = this->entries.find(name);
            if (it == this->entries.end()) {
                throw std::out_of_range("Block not found: " + name);
            }
            return it->second;
        }

        using Base::operator[];

        void print(std::ostream& os, int indent = 0) const override {
            os << std::string(indent, ' ') << "=== Uniform Block Reflector ===\n";
            os << std::string(indent, ' ') << "Program ID: " << programId << "\n";
            os << std::string(indent, ' ') << "Blocks:\n";
            for (const auto& [name, block] : this->entries) {
                block.print(os, indent + 2);
                os << "\n";
            }
        }

        friend std::ostream& operator<<(std::ostream& os, const uniform_block_reflector& reflector) {
            reflector.print(os);
            return os;
        }
    };

}