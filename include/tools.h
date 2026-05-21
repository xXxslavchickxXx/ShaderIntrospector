#pragma once

#include <GL/glew.h>
#include <iostream>

namespace shader {
    inline bool program_is_valid(GLuint program) {
        if (program == 0) return false;

        if (!glIsProgram(program)) return false;

        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

        return linkStatus == GL_TRUE;
    }

    inline bool check_program(GLuint program) {
        if (!program_is_valid(program)) {
            std::cerr << "this program doesn't exist: " << program;
            return false;
        }
        return true;
    }
}