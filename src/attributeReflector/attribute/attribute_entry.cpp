#pragma once

#include <attributeReflector/attribute/attribute_entry.h>

#include <iostream>
#include <iomanip>
#include <toString/toString.h>

namespace shader {
    attribute_entry attribute_entry::operator[](size_t index) {
        if (!check_program(programId) || index > elements) return attribute_entry();

        if (elements > 1) {
            std::string indexedName = name + "[" + std::to_string(index) + "]";
            GLint indexedLoc = glGetUniformLocation(programId, indexedName.c_str());
            return attribute_entry(indexedName, indexedLoc, type, elements, programId);
        }

        return *this;
    }

    attribute_entry attribute_entry::at(size_t index) {
        if (!check_program(programId)) return attribute_entry();

        if (index > elements) {
            throw std::out_of_range("out of range uniforms\n");
        }

        if (elements > 1) {
            std::string indexedName = name + "[" + std::to_string(index) + "]";
            GLint indexedLoc = glGetUniformLocation(programId, indexedName.c_str());
            return attribute_entry(indexedName, indexedLoc, type, elements, programId);
        }

        return *this;
    }

    const attribute_entry attribute_entry::operator[](size_t index) const {
        if (!check_program(programId) || index > elements) return attribute_entry();

        if (elements > 1) {
            std::string indexedName = name + "[" + std::to_string(index) + "]";
            GLint indexedLoc = glGetUniformLocation(programId, indexedName.c_str());
            return attribute_entry(indexedName, indexedLoc, type, elements, programId);
        }

        return *this;
    }

    const attribute_entry attribute_entry::at(size_t index) const {
        if (!check_program(programId)) return attribute_entry();

        if (index > elements) {
            throw std::out_of_range("out of range uniforms\n");
        }

        if (elements > 1) {
            std::string indexedName = name + "[" + std::to_string(index) + "]";
            GLint indexedLoc = glGetUniformLocation(programId, indexedName.c_str());
            return attribute_entry(indexedName, indexedLoc, type, elements, programId);
        }

        return *this;
    }

    attribute_entry::attribute_entry(std::string name, GLint location, GLenum type, GLint elements, GLint programId)
        : name(name)
        , location(location)
        , type(type)
        , elements(elements)
        , programId(programId)
    {}

    void attribute_entry::bindProgram() const {
        if (!check_program(programId)) return;
        GLint currentProgram = 0;
        glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
        if (currentProgram != programId) {
            glUseProgram(programId);
        }
    }

    std::ostream& operator<<(std::ostream& os, const attribute_entry& entry) {
        // Форматируем тип
        std::string typeStr = toString(entry.type);
        if (entry.elements > 1) {
            typeStr += "[" + std::to_string(entry.elements) + "]";
        }

        // Выводим имя и тип
        os << "-> " << entry.name << " (" << typeStr << ")\n";

        // Выводим значение с отступом
        const std::string indent = "    ";

        auto print_matrix = [&]<typename Mat>(const Mat & m) {
            const int rows = Mat::length();
            const int cols = Mat::col_type::length();
            for (int i = 0; i < rows; ++i) {
                os << indent << "[";
                for (int j = 0; j < cols; ++j) {
                    if (j > 0) os << ", ";
                    os << std::fixed << std::setprecision(2) << m[i][j];
                }
                os << "]";
                if (i < rows - 1) os << "\n";
            }
        };

        auto print_vec = [&]<typename Vec>(const Vec & v) {
            os << indent << "(";
            for (int j = 0; j < Vec::length(); ++j) {
                if (j > 0) os << ", ";
                if (std::is_same_v<typename Vec::value_type, bool>) {
                    os << (v[j] ? "true" : "false");
                }
                else {
                    os << v[j];
                }
            }
            os << ")";
        };

        //if (entry.elements > 1) {
        //    for (int i = 0; i < entry.elements; ++i) {
        //        auto elem = entry[i];
        //        os << indent << "[" << i << "]" << " element\n";
        //    }
        //}

        return os;
    }
}