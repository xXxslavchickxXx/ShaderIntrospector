#include <uniformBlockReflector/uniformBlockReflector.h>
#include <tools.h>
#include <toString/toString.h>

namespace shader {

    uniform_block_reflector::uniform_block_reflector(GLuint programId)
        : Base("UniformBlockReflector"), programId(programId)
    {
        reflect(programId);
    }

    void uniform_block_reflector::reflect(GLuint programId) {
        if (!check_program(programId)) return;

        GLint numBlocks = 0;
        glGetProgramiv(programId, GL_ACTIVE_UNIFORM_BLOCKS, &numBlocks);
        if (numBlocks == 0) return;

        for (GLuint blockIndex = 0; blockIndex < (GLuint)numBlocks; ++blockIndex) {
            // Получаем имя блока
            GLint nameLength = 0;
            glGetActiveUniformBlockiv(programId, blockIndex, GL_UNIFORM_BLOCK_NAME_LENGTH, &nameLength);
            std::vector<char> nameBuffer(nameLength);
            glGetActiveUniformBlockName(programId, blockIndex, nameLength, nullptr, nameBuffer.data());
            std::string blockName(nameBuffer.data());

            // Создаём информацию о блоке
            uniform_block_info blockInfo(blockName);
            blockInfo.index = blockIndex;
            glGetActiveUniformBlockiv(programId, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockInfo.byte_size);
            glGetActiveUniformBlockiv(programId, blockIndex, GL_UNIFORM_BLOCK_BINDING, &blockInfo.binding);
            glGetActiveUniformBlockiv(programId, blockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &blockInfo.activeUniforms);

            // Получаем индексы активных юниформ в блоке
            std::vector<GLuint> uniformIndices(blockInfo.activeUniforms);
            glGetActiveUniformBlockiv(programId, blockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES,
                reinterpret_cast<GLint*>(uniformIndices.data()));

            // Буфер для имён
            GLint maxNameLength = 0;
            glGetProgramiv(programId, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxNameLength);
            std::vector<char> nameBuffer2(maxNameLength);

            // Рефлексим каждый член блока
            for (GLuint idx : uniformIndices) {
                // Получаем имя юниформа
                GLsizei length = 0;
                GLint size = 0;
                GLenum type = 0;
                glGetActiveUniform(programId, idx, maxNameLength, &length, &size, &type, nameBuffer2.data());
                std::string memberName(nameBuffer2.data(), length);

                // Получаем параметры
                GLint offset = 0;
                GLint arrayStride = 0;
                GLint matrixStride = 0;
                GLint isRowMajor = 0;

                glGetActiveUniformsiv(programId, 1, &idx, GL_UNIFORM_OFFSET, &offset);
                glGetActiveUniformsiv(programId, 1, &idx, GL_UNIFORM_ARRAY_STRIDE, &arrayStride);
                glGetActiveUniformsiv(programId, 1, &idx, GL_UNIFORM_MATRIX_STRIDE, &matrixStride);
                glGetActiveUniformsiv(programId, 1, &idx, GL_UNIFORM_IS_ROW_MAJOR, &isRowMajor);

                // Создаём члена блока
                uniform_block_member member(
                    memberName, type, offset, arrayStride, size,
                    isRowMajor == GL_TRUE, programId
                );

                blockInfo.add_member(std::move(member));
            }

            // Добавляем блок в рефлектор
            this->entries[blockName] = std::move(blockInfo);
        }
    }

    void uniform_block_reflector::setBinding(const std::string& blockName, GLuint bindingPoint) {
        auto it = this->entries.find(blockName);
        if (it == this->entries.end()) {
            throw std::out_of_range("Block not found: " + blockName);
        }
        glUniformBlockBinding(programId, it->second.index, bindingPoint);
        it->second.binding = static_cast<GLint>(bindingPoint);
    }

}