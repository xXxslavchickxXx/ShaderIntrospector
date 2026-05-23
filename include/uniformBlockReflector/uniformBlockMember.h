#pragma once

#include <templates/opengl_iterator.h>
#include <toString/toString.h>

#include <uniformBlockReflector/uniformBlockHandle.h>

class uniform_block_member : public opengl_template_iterator<uniform_block_member, uniform_block_handle> {
    using Base = opengl_template_iterator<uniform_block_member, uniform_block_handle>;

public:
    GLenum type = 0;
    GLint offset = 0;        // offset первого элемента
    GLint arrayStride = 0;   // шаг для массивов
    GLint size = 1;          // количество элементов
    bool isRowMajor = false;

    uniform_block_member() = default;

    // Конструктор для скаляра или массива
    uniform_block_member(const std::string& name, GLenum type, GLint offset,
        GLint arrayStride, GLint size, bool rowMajor, GLint program)
        : Base(name, program), type(type), offset(offset),
        arrayStride(arrayStride), size(size), isRowMajor(rowMajor)
    {
        if (size > 1) {
            // Массив: создаём handle для каждого элемента
            for (int i = 0; i < size; ++i) {
                std::string elemName = name + "[" + std::to_string(i) + "]";
                this->add_entry(uniform_block_handle(elemName, offset + i * arrayStride, type, 1));
            }
        }
        else {
            // Скаляр
            this->add_entry(uniform_block_handle(name, offset, type, 1));
        }
    }

    // Доступ к элементу массива
    uniform_block_handle operator[](size_t i) const {
        if (i >= size) throw std::out_of_range("Array index out of range");
        return this->entries[i];
    }

    // Если не массив — приводим к handle
    operator uniform_block_handle() const {
        return this->entries[0];
    }

    void print(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ') << this->name;
        if (size > 1) os << "[" << size << "]";
        os << " (" << toString(type) << ") @ offset " << offset;
        if (arrayStride > 0 && size > 1) {
            os << " [stride=" << arrayStride << "]";
        }
        if (this->program != -1) {
            os << " [program=" << this->program << "]";
        }
        os << "\n";
        // Выводим элементы массива
        for (const auto& elem : this->entries) {
            os << std::string(indent + 2, ' ') << "  - " << elem << "\n";
        }
    }
};