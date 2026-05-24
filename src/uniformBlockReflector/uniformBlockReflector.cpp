#include <uniformBlockReflector/uniformBlockReflector.h>
#include <uniformBlockReflector/uniformBlockInfo.h>
#include <toString/toString.h>
#include <tools.h>
#include <algorithm>

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

        // Временное хранилище для обычных блоков и массивов
        std::unordered_map<std::string, std::vector<std::pair<int, uniform_block_info>>> arrayGroups;
        std::unordered_map<std::string, uniform_block_info> singleBlocks;

        // Параметры для рефлексии
        GLint maxUniformNameLength = 0;
        glGetProgramiv(programId, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformNameLength);
        std::vector<char> nameBuffer(maxUniformNameLength);
        std::vector<GLuint> indices;

        size_t block_offset = 0;
        std::string last_block_name;
        for (GLuint blockIdx = 0; blockIdx < (GLuint)numBlocks; ++blockIdx) {
            // Получаем имя блока
            GLint nameLength = 0;
            glGetActiveUniformBlockiv(programId, blockIdx, GL_UNIFORM_BLOCK_NAME_LENGTH, &nameLength);
            std::vector<char> blockNameBuffer(nameLength);
            glGetActiveUniformBlockName(programId, blockIdx, nameLength, nullptr, blockNameBuffer.data());
            std::string fullName(blockNameBuffer.data());

            // Проверяем, элемент ли это массива блоков
            size_t bracketPos = fullName.find('[');
            if (bracketPos != std::string::npos) {
                // Это элемент массива: "name[index]"
                std::string baseName = fullName.substr(0, bracketPos);
                int index = std::stoi(fullName.substr(bracketPos + 1, fullName.find(']') - bracketPos - 1));

                // Создаём блок для этого элемента
                uniform_block_info elementBlock(fullName);
                elementBlock.index = blockIdx;

                std::cout << index << '\n';

                glGetActiveUniformBlockiv(programId, blockIdx, GL_UNIFORM_BLOCK_DATA_SIZE, &elementBlock.byte_size);
                glGetActiveUniformBlockiv(programId, blockIdx, GL_UNIFORM_BLOCK_BINDING, &elementBlock.binding);
                glGetActiveUniformBlockiv(programId, blockIdx, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &elementBlock.activeUniforms);

                // Рефлексим члены блока
                std::vector<GLuint> uniformIndices(elementBlock.activeUniforms);
                glGetActiveUniformBlockiv(programId, blockIdx, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES,
                    reinterpret_cast<GLint*>(uniformIndices.data()));

                indices.resize(elementBlock.activeUniforms);
                std::copy(uniformIndices.begin(), uniformIndices.end(), indices.begin());

                std::vector<GLint> offsets(elementBlock.activeUniforms);
                std::vector<GLint> arrayStrides(elementBlock.activeUniforms);
                std::vector<GLint> matrixStrides(elementBlock.activeUniforms);
                std::vector<GLint> isRowMajors(elementBlock.activeUniforms);

                glGetActiveUniformsiv(programId, elementBlock.activeUniforms, indices.data(),
                    GL_UNIFORM_OFFSET, offsets.data());
                glGetActiveUniformsiv(programId, elementBlock.activeUniforms, indices.data(),
                    GL_UNIFORM_ARRAY_STRIDE, arrayStrides.data());
                glGetActiveUniformsiv(programId, elementBlock.activeUniforms, indices.data(),
                    GL_UNIFORM_MATRIX_STRIDE, matrixStrides.data());
                glGetActiveUniformsiv(programId, elementBlock.activeUniforms, indices.data(),
                    GL_UNIFORM_IS_ROW_MAJOR, isRowMajors.data());

                elementBlock.offset = index * elementBlock.byte_size;

                for (size_t i = 0; i < uniformIndices.size(); ++i) {
                    GLsizei length = 0;
                    GLint size = 0;
                    GLenum type = 0;
                    glGetActiveUniform(programId, uniformIndices[i], maxUniformNameLength,
                        &length, &size, &type, nameBuffer.data());
                    std::string memberName(nameBuffer.data(), length);

                    uniform_block_member member(
                        memberName, type, block_offset + offsets[i], arrayStrides[i], size,
                        isRowMajors[i] == GL_TRUE, programId
                    );
                    elementBlock.add_member(std::move(member));
                }

                arrayGroups[baseName].push_back({ index, std::move(elementBlock) });

                block_offset += elementBlock.byte_size;
            }
            else {
                block_offset = 0;

                // Обычный блок (не массив)
                uniform_block_info block(fullName);
                block.index = blockIdx;
                glGetActiveUniformBlockiv(programId, blockIdx, GL_UNIFORM_BLOCK_DATA_SIZE, &block.byte_size);
                glGetActiveUniformBlockiv(programId, blockIdx, GL_UNIFORM_BLOCK_BINDING, &block.binding);
                glGetActiveUniformBlockiv(programId, blockIdx, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &block.activeUniforms);

                std::vector<GLuint> uniformIndices(block.activeUniforms);
                glGetActiveUniformBlockiv(programId, blockIdx, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES,
                    reinterpret_cast<GLint*>(uniformIndices.data()));

                indices.resize(block.activeUniforms);
                std::copy(uniformIndices.begin(), uniformIndices.end(), indices.begin());

                std::vector<GLint> offsets(block.activeUniforms);
                std::vector<GLint> arrayStrides(block.activeUniforms);
                std::vector<GLint> matrixStrides(block.activeUniforms);
                std::vector<GLint> isRowMajors(block.activeUniforms);

                glGetActiveUniformsiv(programId, block.activeUniforms, indices.data(),
                    GL_UNIFORM_OFFSET, offsets.data());
                glGetActiveUniformsiv(programId, block.activeUniforms, indices.data(),
                    GL_UNIFORM_ARRAY_STRIDE, arrayStrides.data());
                glGetActiveUniformsiv(programId, block.activeUniforms, indices.data(),
                    GL_UNIFORM_MATRIX_STRIDE, matrixStrides.data());
                glGetActiveUniformsiv(programId, block.activeUniforms, indices.data(),
                    GL_UNIFORM_IS_ROW_MAJOR, isRowMajors.data());

                for (size_t i = 0; i < uniformIndices.size(); ++i) {
                    GLsizei length = 0;
                    GLint size = 0;
                    GLenum type = 0;
                    glGetActiveUniform(programId, uniformIndices[i], maxUniformNameLength,
                        &length, &size, &type, nameBuffer.data());
                    std::string memberName(nameBuffer.data(), length);

                    uniform_block_member member(
                        memberName, type, block_offset + offsets[i], arrayStrides[i], size,
                        isRowMajors[i] == GL_TRUE, programId
                    );
                    block.add_member(std::move(member));
                }
                singleBlocks[fullName] = std::move(block);
            }
        }

        // Собираем массивы блоков в uniform_block_iterator
        for (auto& [baseName, elements] : arrayGroups) {
            // Сортируем по индексу
            std::sort(elements.begin(), elements.end(),
                [](const auto& a, const auto& b) { return a.first < b.first; });

            uniform_block_iterator blockArray(baseName, programId);
            for (auto& [idx, element] : elements) {
                blockArray.add_block(std::move(element));
            }

            this->entries[baseName] = std::move(blockArray);
        }

        // Добавляем обычные блоки как итераторы с одним элементом
        for (auto& [name, block] : singleBlocks) {
            uniform_block_iterator singleIterator(name, programId);
            singleIterator.index = block.index;
            singleIterator.binding = block.binding;
            singleIterator.add_block(std::move(block));
            this->entries[name] = std::move(singleIterator);
        }
    }

    void uniform_block_reflector::setBinding(const std::string& blockName, GLuint bindingPoint) {
        auto it = this->entries.find(blockName);
        if (it == this->entries.end()) {
            throw std::out_of_range("Block not found: " + blockName);
        }

        // Для каждого элемента массива блоков
        for (size_t i = 0; i < it->second.size(); ++i) {
            glUniformBlockBinding(programId, it->second[i].index, bindingPoint);
            it->second[i].binding = static_cast<GLint>(bindingPoint);
        }
    }

}