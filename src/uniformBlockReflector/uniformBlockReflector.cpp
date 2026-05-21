#include <uniformBlockReflector/uniformBlockReflector.h>

#include <tools.h>

namespace shader {
	uniform_block_reflector::uniform_block_reflector(GLuint programId) {
		reflect(programId);
	}

	const uniform_block_info& uniform_block_reflector::operator[](const std::string& name) const {
		auto it = blocks.find(name);

		if (it == blocks.end()) {
			std::cerr << "error: this block doesn't exist in this program\n";
			return uniform_block_info();
		}
		return it->second;
	}
	const uniform_block_info& uniform_block_reflector::getBlock(const std::string& name) const {
		auto it = blocks.find(name);

		if (it == blocks.end()) {
			throw std::runtime_error("error: this block doesn't exist in this program\n");
		}
		return it->second;
	}

	void uniform_block_reflector::reflect(GLuint programId) {
		if (!check_program(programId)) return;

		GLint numBlocks = 0;
		glGetProgramiv(programId, GL_ACTIVE_UNIFORM_BLOCKS, &numBlocks);
		if (numBlocks == 0) return;

		for (GLuint blockIndex = 0; blockIndex < (GLuint)numBlocks; ++blockIndex) {
			uniform_block_info info;
			info.index = blockIndex;

			// Получаем имя блока
			GLint nameLength = 0;
			glGetActiveUniformBlockiv(programId, blockIndex, GL_UNIFORM_BLOCK_NAME_LENGTH, &nameLength);
			std::vector<char> nameBuffer(nameLength);
			glGetActiveUniformBlockName(programId, blockIndex, nameLength, nullptr, nameBuffer.data());
			info.name = std::string(nameBuffer.data());

			// Получаем основные параметры блока
			glGetActiveUniformBlockiv(programId, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &info.dataSize);
			glGetActiveUniformBlockiv(programId, blockIndex, GL_UNIFORM_BLOCK_BINDING, &info.binding);
			glGetActiveUniformBlockiv(programId, blockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &info.activeUniforms);

			// Рефлексим членов блока
			reflectMembers(blockIndex, info, programId);

			blocks[info.name] = std::move(info);
		}
	}

	void uniform_block_reflector::reflectMembers(GLuint blockIndex, uniform_block_info& info, GLint programId) {
		// Получаем индексы активных юниформ в блоке
		std::vector<GLuint> uniformIndices(info.activeUniforms);
		glGetActiveUniformBlockiv(programId, blockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES,
			reinterpret_cast<GLint*>(uniformIndices.data()));

		// Буфер для имён
		GLint maxNameLength = 0;
		glGetProgramiv(programId, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxNameLength);
		std::vector<char> nameBuffer(maxNameLength);

		info.members.reserve(info.activeUniforms);

		for (GLuint idx : uniformIndices) {
			uniform_block_member member;

			// Получаем имя юниформа
			GLsizei length = 0;
			GLint size = 0;
			GLenum type = 0;
			glGetActiveUniform(programId, idx, maxNameLength, &length, &size, &type, nameBuffer.data());
			member.name = std::string(nameBuffer.data(), length);
			member.type = type;
			member.size = size;

			// Получаем смещение в блоке
			glGetActiveUniformsiv(programId, 1, &idx, GL_UNIFORM_OFFSET, &member.offset);

			// Для массивов - шаг между элементами
			glGetActiveUniformsiv(programId, 1, &idx, GL_UNIFORM_ARRAY_STRIDE, &member.arrayStride);

			// Для матриц - шаг между столбцами
			glGetActiveUniformsiv(programId, 1, &idx, GL_UNIFORM_MATRIX_STRIDE, &member.matrixStride);

			// Row major или column major
			GLint isRowMajor = 0;
			glGetActiveUniformsiv(programId, 1, &idx, GL_UNIFORM_IS_ROW_MAJOR, &isRowMajor);
			member.isRowMajor = (isRowMajor == GL_TRUE);

			info.members.push_back(std::move(member));
		}
	}

	std::ostream& operator<<(std::ostream& os, const uniform_block_reflector& reflector) {
		os << "========================================\n";
		os << "        UNIFORM BLOCK REFLECTOR         \n";
		os << "========================================\n";

		if (reflector.blocks.empty()) {
			os << "  (no uniform blocks in this program)\n";
			return os;
		}

		os << "\n";
		size_t count = 0;
		for (const auto& [name, block] : reflector.blocks) {
			os << block;
			if (++count < reflector.blocks.size()) {
				os << "\n\n";
			}
		}
		os << "\n========================================\n";
		return os;
	}
}