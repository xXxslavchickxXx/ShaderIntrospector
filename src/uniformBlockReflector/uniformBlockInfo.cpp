#include <uniformBlockReflector/uniformBlockInfo.h>
#include <iostream>
#include <tools.h>
#include <toString/toString.h>

namespace shader {

    // ==================== uniform_block_member ====================

    std::ostream& operator<<(std::ostream& os, const uniform_block_member& member) {
        os << " " << member.name << " (" << toString(member.type) << ")";
        os << " @ offset " << member.offset;

        if (member.size > 1) {
            os << " [array of " << member.size << ", stride=" << member.arrayStride << "]";
        }

        if (member.matrixStride > 0) {
            os << " [matrix stride=" << member.matrixStride
                << (member.isRowMajor ? ", row_major" : ", col_major") << "]";
        }

        return os;
    }

    // ==================== uniform_block_info ====================

    const uniform_block_member& uniform_block_info::operator[](const std::string& memberName) const {
        static const uniform_block_member empty{};
        for (const auto& m : members) {
            if (m.name == memberName) return m;
        }
        std::cerr << "Member not found in block '" << name << "': " << memberName << std::endl;
        return empty;
    }

    uniform_block_member& uniform_block_info::operator[](const std::string& memberName) {
        for (auto& m : members) {
            if (m.name == memberName) return m;
        }
        throw std::out_of_range("Member not found in block '" + name + "': " + memberName);
    }

    const uniform_block_member& uniform_block_info::getMember(const std::string& memberName) const {
        for (const auto& m : members) {
            if (m.name == memberName) return m;
        }
        throw std::out_of_range("Member not found in block '" + name + "': " + memberName);
    }

    uniform_block_member& uniform_block_info::getMember(const std::string& memberName) {
        for (auto& m : members) {
            if (m.name == memberName) return m;
        }
        throw std::out_of_range("Member not found in block '" + name + "': " + memberName);
    }

    const uniform_block_member& uniform_block_info::operator[](size_t index) const {
        if (index >= members.size()) {
            throw std::out_of_range("Member index out of range in block '" + name + "'");
        }
        return members[index];
    }

    uniform_block_member& uniform_block_info::operator[](size_t index) {
        if (index >= members.size()) {
            throw std::out_of_range("Member index out of range in block '" + name + "'");
        }
        return members[index];
    }

    std::ostream& operator<<(std::ostream& os, const uniform_block_info& block) {
        os << "=== Uniform Block: " << block.name << " ===\n";
        os << "  Binding: " << block.binding << "\n";
        os << "  Size: " << block.dataSize << " bytes\n";
        os << "  Active uniforms: " << block.activeUniforms << "\n";
        os << "  Members:\n";

        for (size_t i = 0; i < block.members.size(); ++i) {
            os << "    [" << i << "] " << block.members[i];
            if (i < block.members.size() - 1) os << "\n";
        }

        return os;
    }

} // namespace shader