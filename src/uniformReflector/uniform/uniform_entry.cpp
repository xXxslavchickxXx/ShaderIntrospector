#pragma once

#include <uniformReflector/uniform/uniform_entry.h>

#include <iostream>

namespace shader {
    uniform_entry uniform_entry::operator[](size_t index) {
        if (!check_program(programId) || index > elements) return uniform_entry();

        if (elements > 1) {
            std::string indexedName = name + "[" + std::to_string(index) + "]";
            GLint indexedLoc = glGetUniformLocation(programId, indexedName.c_str());
            return uniform_entry(indexedName, indexedLoc, type, elements, programId);
        }

        return *this;
    }

    uniform_entry uniform_entry::at(size_t index) {
        if (!check_program(programId)) return uniform_entry();

        if (index > elements) {
            throw std::out_of_range("out of range uniforms\n");
        }

        if (elements > 1) {
            std::string indexedName = name + "[" + std::to_string(index) + "]";
            GLint indexedLoc = glGetUniformLocation(programId, indexedName.c_str());
            return uniform_entry(indexedName, indexedLoc, type, elements, programId);
        }

        return *this;
    }

    const uniform_entry uniform_entry::operator[](size_t index) const {
        if (!check_program(programId) || index > elements) return uniform_entry();
        
        if (elements > 1) {
            std::string indexedName = name + "[" + std::to_string(index) + "]";
            GLint indexedLoc = glGetUniformLocation(programId, indexedName.c_str());
            return uniform_entry(indexedName, indexedLoc, type, elements, programId);
        }

        return *this;
    }

    const uniform_entry uniform_entry::at(size_t index) const {
        if (!check_program(programId)) return uniform_entry();

        if (index > elements) {
            throw std::out_of_range("out of range uniforms\n");
        }

        if (elements > 1) {
            std::string indexedName = name + "[" + std::to_string(index) + "]";
            GLint indexedLoc = glGetUniformLocation(programId, indexedName.c_str());
            return uniform_entry(indexedName, indexedLoc, type, elements, programId);
        }

        return *this;
    }

    uniform_entry::uniform_entry(std::string name, GLint location, GLenum type, GLint elements, GLint programId)
        : name(name)
        , location(location)
        , type(type)
        , elements(elements)
        , programId(programId)
    {}

    void uniform_entry::bindProgram() const {
        if (!check_program(programId)) return;
        GLint currentProgram = 0;
        glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
        if (currentProgram != programId) {
            glUseProgram(programId);
        }
    }
}