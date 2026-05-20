#pragma once

#include <GL/glew.h>

namespace shader {
    inline bool program_is_valid(GLuint program) {
        if (program == 0) return false;

        if (!glIsProgram(program)) return false;

        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

        return linkStatus == GL_TRUE;
    }
}