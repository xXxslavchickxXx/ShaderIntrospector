#pragma once

#include <uniformBlockReflector/uniformBlockReflector.h>

#include <iostream>

namespace shader {
    struct uniform_block_member {
        std::string name;
        GLenum type;
        GLint offset;        // смещение внутри блока (в байтах)
        GLint size;          // количество элементов (для массивов)
        GLint arrayStride;   // шаг между элементами массива
        GLint matrixStride;  // шаг между столбцами матрицы
        bool isRowMajor;     // layout(row_major)

        // Оператор вывода для члена блока
        friend std::ostream& operator<<(std::ostream& os, const uniform_block_member& member);
    };

    struct uniform_block_info {
        std::string name;
        GLuint index;           // индекс блока в программе
        GLint binding;          // binding point
        GLint dataSize;         // размер блока в байтах
        GLint activeUniforms;   // количество активных юниформ в блоке
        std::vector<uniform_block_member> members;

        // Доступ к членам блока по имени
        const uniform_block_member& operator[](const std::string& memberName) const;
        uniform_block_member& operator[](const std::string& memberName);

        const uniform_block_member& getMember(const std::string& memberName) const;
        uniform_block_member& getMember(const std::string& memberName);

        // Доступ по индексу
        const uniform_block_member& operator[](size_t index) const;
        uniform_block_member& operator[](size_t index);

        size_t size() const { return members.size(); }
        bool empty() const { return members.empty(); }

        // Оператор вывода для блока
        friend std::ostream& operator<<(std::ostream& os, const uniform_block_info& block);
    };
}