#include <uniformBlockReflector/uniformBlockReflector.h>
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

        // Параметры для рефлексии
        GLint maxUniformNameLength = 0;
        glGetProgramiv(programId, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformNameLength);
        std::vector<char> nameBuffer(maxUniformNameLength);

        size_t block_offset = 0;

        for (GLuint blockIdx = 0; blockIdx < (GLuint)numBlocks; ++blockIdx) {
            // Получаем имя блока
            GLint nameLength = 0;
            glGetActiveUniformBlockiv(programId, blockIdx, GL_UNIFORM_BLOCK_NAME_LENGTH, &nameLength);
            std::vector<char> blockNameBuffer(nameLength);
            glGetActiveUniformBlockName(programId, blockIdx, nameLength, nullptr, blockNameBuffer.data());
            std::string fullName(blockNameBuffer.data());

            // Определяем, массив это или нет
            size_t bracketPos = fullName.find('[');
            bool isArrayBlock = (bracketPos != std::string::npos);

            // Базовое имя (без индекса для массива)
            std::string baseName = isArrayBlock ? fullName.substr(0, bracketPos) : fullName;

            // Создаем блок
            uniform_block_data_info block(baseName);
            block.index = blockIdx;
            block.offset = isArrayBlock ? block_offset : 0;

            glGetActiveUniformBlockiv(programId, blockIdx, GL_UNIFORM_BLOCK_DATA_SIZE, &block.byte_size);
            glGetActiveUniformBlockiv(programId, blockIdx, GL_UNIFORM_BLOCK_BINDING, &block.binding);
            glGetActiveUniformBlockiv(programId, blockIdx, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &block.uniforms);

            // Рефлексим члены блока (общая логика)
            reflectBlockMembers(programId, blockIdx, block, maxUniformNameLength, nameBuffer);

            // Сохраняем блок
            this->entries[baseName].add_entry(std::move(block));

            // Обновляем offset для следующего элемента массива
            if (isArrayBlock) {
                block_offset += block.byte_size;
            }
            else {
                block_offset = 0;
            }
        }
    }

    // Вынесенная функция для рефлексии членов блока
    void uniform_block_reflector::reflectBlockMembers(GLuint programId, GLuint blockIdx,
        uniform_block_data_info& block, GLint maxUniformNameLength, std::vector<char>& nameBuffer) {

        std::vector<GLuint> uniforms(block.uniforms);
        glGetActiveUniformBlockiv(programId, blockIdx, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES,
            reinterpret_cast<GLint*>(uniforms.data()));

        std::vector<GLint> offsets(block.uniforms);
        std::vector<GLint> elementStrides(block.uniforms);

        glGetActiveUniformsiv(programId, block.uniforms, uniforms.data(),
            GL_UNIFORM_OFFSET, offsets.data());
        glGetActiveUniformsiv(programId, block.uniforms, uniforms.data(),
            GL_UNIFORM_ARRAY_STRIDE, elementStrides.data());

        for (size_t i = 0; i < uniforms.size(); ++i) {
            GLsizei length = 0;
            GLint elements = 0;
            GLenum type = 0;

            glGetActiveUniform(programId, uniforms[i], maxUniformNameLength,
                &length, &elements, &type, nameBuffer.data());

            std::string uniformName(nameBuffer.data(), length);

            // Убираем префикс с точкой (часть блока)
            size_t dotPos = uniformName.find('.');
            if (dotPos != std::string::npos) {
                uniformName = uniformName.substr(dotPos + 1);
            }

            // Убираем суффикс массива
            size_t bracketPos = uniformName.find('[');
            if (bracketPos != std::string::npos) {
                uniformName = uniformName.substr(0, bracketPos);
            }

            uniform_block_view_info uniform(uniformName, programId);
            uniform.type = type;
            uniform.offset = offsets[i] + block.offset;
            uniform.elementStride = elementStrides[i];

            if (elements > 1) {
                // Массив внутри блока
                for (size_t j = 0; j < elements; ++j) {
                    uniform_block_field_info uniform_field(std::format("{}[{}]", uniformName, j));
                    uniform_field.type = type;
                    uniform_field.byte_size = get_gl_type_size(type);
                    uniform_field.offset = uniform.offset + j * elementStrides[i];
                    uniform.add_entry(std::move(uniform_field));
                }
            }
            else {
                // Обычная переменная
                uniform_block_field_info uniform_field(uniformName);
                uniform_field.type = type;
                uniform_field.byte_size = get_gl_type_size(type);
                uniform_field.offset = uniform.offset;
                uniform.add_entry(std::move(uniform_field));
            }

            block.add_member(std::move(uniform));
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