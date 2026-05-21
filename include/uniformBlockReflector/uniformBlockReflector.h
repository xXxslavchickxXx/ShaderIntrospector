#pragma once

#include <GL/glew.h>
#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>

#include <toString/toString.h>
#include <uniformBlockReflector/uniformBlockInfo.h>

namespace shader {
	struct uniform_block_member;
	struct uniform_block_info;

	class uniform_block_reflector {
		std::unordered_map<std::string, uniform_block_info> blocks;

	public:
		uniform_block_reflector(GLuint programId);

		uniform_block_reflector(const uniform_block_reflector&) = delete;
		uniform_block_reflector& operator=(const uniform_block_reflector&) = delete;
		uniform_block_reflector(uniform_block_reflector&&) = default;
		uniform_block_reflector& operator=(uniform_block_reflector&&) = default;

		const uniform_block_info& operator[](const std::string& name) const;
		const uniform_block_info& getBlock(const std::string& name) const;

		void reflect(GLuint programId);
		void reflectMembers(GLuint blockIndex, uniform_block_info& info, GLint programId);

		const auto& get_blocks() const { return blocks; }
		// Для const итерации
		auto begin() const { return blocks.begin(); }
		auto end() const { return blocks.end(); }
		auto cbegin() const { return blocks.cbegin(); }
		auto cend() const { return blocks.cend(); }

		// Для non-const итерации
		auto begin() { return blocks.begin(); }
		auto end() { return blocks.end(); }

	private:
		friend std::ostream& operator<<(std::ostream& os, const uniform_block_reflector& reflector);
	};
}