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

        
    constexpr size_t get_gl_type_size(GLenum type) {
        switch (type) {
            // Базовые скаляры
        case GL_FLOAT:          return sizeof(GLfloat);
        case GL_INT:            return sizeof(GLint);
        case GL_UNSIGNED_INT:   return sizeof(GLuint);
        case GL_BOOL:           return sizeof(GLboolean); // В шейдерах обычно как 4 байта, но sizeof(GLboolean) безопаснее
        case GL_DOUBLE:         return sizeof(GLdouble);

            // Векторы (Вектор из N элементов = N * размер скаляра)
        case GL_FLOAT_VEC2:     return sizeof(GLfloat) * 2;
        case GL_FLOAT_VEC3:     return sizeof(GLfloat) * 3;
        case GL_FLOAT_VEC4:     return sizeof(GLfloat) * 4;

        case GL_INT_VEC2:       return sizeof(GLint) * 2;
        case GL_INT_VEC3:       return sizeof(GLint) * 3;
        case GL_INT_VEC4:       return sizeof(GLint) * 4;

        case GL_UNSIGNED_INT_VEC2: return sizeof(GLuint) * 2;
        case GL_UNSIGNED_INT_VEC3: return sizeof(GLuint) * 3;
        case GL_UNSIGNED_INT_VEC4: return sizeof(GLuint) * 4;

        case GL_BOOL_VEC2:      return 4 * 2; // Внутри буферов BOOL обычно выравнивается по 4 байта
        case GL_BOOL_VEC3:      return 4 * 3;
        case GL_BOOL_VEC4:      return 4 * 4;

            // Матрицы (Количество колонок * Количество строк * размер скаляра)
            // Квадратные матрицы
        case GL_FLOAT_MAT2:     return sizeof(GLfloat) * 4;  // 2x2
        case GL_FLOAT_MAT3:     return sizeof(GLfloat) * 9;  // 3x3
        case GL_FLOAT_MAT4:     return sizeof(GLfloat) * 16; // 4x4

            // Прямоугольные матрицы (Редко, но для полноты картины)
        case GL_FLOAT_MAT2x3:   return sizeof(GLfloat) * 6;
        case GL_FLOAT_MAT2x4:   return sizeof(GLfloat) * 8;
        case GL_FLOAT_MAT3x2:   return sizeof(GLfloat) * 6;
        case GL_FLOAT_MAT3x4:   return sizeof(GLfloat) * 12;
        case GL_FLOAT_MAT4x2:   return sizeof(GLfloat) * 8;
        case GL_FLOAT_MAT4x3:   return sizeof(GLfloat) * 12;

            // Двойная точность (если используешь dmat)
        case GL_DOUBLE_MAT2:    return sizeof(GLdouble) * 4;
        case GL_DOUBLE_MAT3:    return sizeof(GLdouble) * 9;
        case GL_DOUBLE_MAT4:    return sizeof(GLdouble) * 16;

        default:
            // Сэмплеры (GL_SAMPLER_2D и т.д.) на самом деле передаются как GLint (индекс текстурного юнита)
            // Если рефлектор споткнется о текстуру в юниформ-блоке (что редкость, они обычно в глобальных юниформах)
            return sizeof(GLint);
        }
    }
}