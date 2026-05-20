namespace shader {
    inline std::ostream& operator<<(std::ostream& os, const uniform_entry& entry) {
        switch (entry.type) {
        case GL_FLOAT: os << entry.get<float>(); break;
        case GL_INT: os << entry.get<int>(); break;
        case GL_UNSIGNED_INT: os << entry.get<unsigned int>(); break;
        case GL_BOOL: os << (entry.get<bool>() ? "true" : "false"); break;

        case GL_FLOAT_VEC2: {
            auto v = entry.get<glm::vec2>();
            os << ("(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ")"); break;
        }
        case GL_INT_VEC2: {
            auto v = entry.get<glm::ivec2>();
            os << ("(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ")"); break;
        }
        case GL_UNSIGNED_INT_VEC2: {
            auto v = entry.get<glm::uvec2>();
            os << ("(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ")"); break;
        }
        case GL_BOOL_VEC2: {
            auto v = entry.get<glm::bvec2>();
            os << ("(" + std::string(v.x ? "true" : "false") + ", " +
                std::string(v.y ? "true" : "false") + ")"); break;
        }

        case GL_FLOAT_VEC3: {
            auto v = entry.get<glm::vec3>();
            os << ("(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z) + ")"); break;
        }
        case GL_INT_VEC3: {
            auto v = entry.get<glm::ivec3>();
            os << ("(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z) + ")"); break;
        }
        case GL_UNSIGNED_INT_VEC3: {
            auto v = entry.get<glm::uvec3>();
            os << ("(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z) + ")"); break;
        }
        case GL_BOOL_VEC3: {
            auto v = entry.get<glm::bvec3>();
            os << ("(" + std::string(v.x ? "true" : "false") + ", " +
                std::string(v.y ? "true" : "false") + ", " +
                std::string(v.z ? "true" : "false") + ")"); break;
        }

        case GL_FLOAT_VEC4: {
            auto v = entry.get<glm::vec4>();
            os << ("(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " +
                std::to_string(v.z) + ", " + std::to_string(v.w) + ")"); break;
        }
        case GL_INT_VEC4: {
            auto v = entry.get<glm::ivec4>();
            os << "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " +
                std::to_string(v.z) + ", " + std::to_string(v.w) + ")"; break;
        }
        case GL_UNSIGNED_INT_VEC4: {
            auto v = entry.get<glm::uvec4>();
            os << "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " +
                std::to_string(v.z) + ", " + std::to_string(v.w) + ")"; break;
        }
        case GL_BOOL_VEC4: {
            auto v = entry.get<glm::bvec4>();
            os << "(" + std::string(v.x ? "true" : "false") + ", " +
                std::string(v.y ? "true" : "false") + ", " +
                std::string(v.z ? "true" : "false") + ", " +
                std::string(v.w ? "true" : "false") + ")"; break;
        }

        case GL_FLOAT_MAT2: {
            auto m = entry.get<glm::mat2>();
            os << "\n[" + std::to_string(m[0][0]) + ", " + std::to_string(m[0][1]) + "]\n" +
                "[" + std::to_string(m[1][0]) + ", " + std::to_string(m[1][1]) + "]"; break;
        }
        case GL_FLOAT_MAT3: {
            auto m = entry.get<glm::mat3>();
            os << "\n[" + std::to_string(m[0][0]) + ", " + std::to_string(m[0][1]) + ", " + std::to_string(m[0][2]) + "]\n" +
                "[" + std::to_string(m[1][0]) + ", " + std::to_string(m[1][1]) + ", " + std::to_string(m[1][2]) + "]\n" +
                "[" + std::to_string(m[2][0]) + ", " + std::to_string(m[2][1]) + ", " + std::to_string(m[2][2]) + "]"; break;
        }
        case GL_FLOAT_MAT4: {
            auto m = entry.get<glm::mat4>();
            os << "\n[" + std::to_string(m[0][0]) + ", " + std::to_string(m[0][1]) + ", " + std::to_string(m[0][2]) + ", " + std::to_string(m[0][3]) + "]\n" +
                "[" + std::to_string(m[1][0]) + ", " + std::to_string(m[1][1]) + ", " + std::to_string(m[1][2]) + ", " + std::to_string(m[1][3]) + "]\n" +
                "[" + std::to_string(m[2][0]) + ", " + std::to_string(m[2][1]) + ", " + std::to_string(m[2][2]) + ", " + std::to_string(m[2][3]) + "]\n" +
                "[" + std::to_string(m[3][0]) + ", " + std::to_string(m[3][1]) + ", " + std::to_string(m[3][2]) + ", " + std::to_string(m[3][3]) + "]";
            break;
        }

        case GL_FLOAT_MAT2x3: {
            auto m = entry.get<glm::mat2x3>();
            os << ("\n[" + std::to_string(m[0][0]) + ", " + std::to_string(m[0][1]) + ", " + std::to_string(m[0][2]) + "]\n" +
                "[" + std::to_string(m[1][0]) + ", " + std::to_string(m[1][1]) + ", " + std::to_string(m[1][2]) + "]"); break;
        }
        case GL_FLOAT_MAT2x4: {
            auto m = entry.get<glm::mat2x4>();
            os << "\n[" + std::to_string(m[0][0]) + ", " + std::to_string(m[0][1]) + ", " + std::to_string(m[0][2]) + ", " + std::to_string(m[0][3]) + "]\n" +
                "[" + std::to_string(m[1][0]) + ", " + std::to_string(m[1][1]) + ", " + std::to_string(m[1][2]) + ", " + std::to_string(m[1][3]) + "]";
            break;
        }
        case GL_FLOAT_MAT3x2: {
            auto m = entry.get<glm::mat3x2>();
            os << ("\n[" + std::to_string(m[0][0]) + ", " + std::to_string(m[0][1]) + "]\n" +
                "[" + std::to_string(m[1][0]) + ", " + std::to_string(m[1][1]) + "]\n" +
                "[" + std::to_string(m[2][0]) + ", " + std::to_string(m[2][1]) + "]"); break;
        }
        case GL_FLOAT_MAT3x4: {
            auto m = entry.get<glm::mat3x4>();
            os << "\n[" + std::to_string(m[0][0]) + ", " + std::to_string(m[0][1]) + ", " + std::to_string(m[0][2]) + ", " + std::to_string(m[0][3]) + "]\n" +
                "[" + std::to_string(m[1][0]) + ", " + std::to_string(m[1][1]) + ", " + std::to_string(m[1][2]) + ", " + std::to_string(m[1][3]) + "]\n" +
                "[" + std::to_string(m[2][0]) + ", " + std::to_string(m[2][1]) + ", " + std::to_string(m[2][2]) + ", " + std::to_string(m[2][3]) + "]";
            break;
        }
        case GL_FLOAT_MAT4x2: {
            auto m = entry.get<glm::mat4x2>();
            os << ("\n[" + std::to_string(m[0][0]) + ", " + std::to_string(m[0][1]) + "]\n" +
                "[" + std::to_string(m[1][0]) + ", " + std::to_string(m[1][1]) + "]\n" +
                "[" + std::to_string(m[2][0]) + ", " + std::to_string(m[2][1]) + "]\n" +
                "[" + std::to_string(m[3][0]) + ", " + std::to_string(m[3][1]) + "]"); break;
        }
        case GL_FLOAT_MAT4x3: {
            auto m = entry.get<glm::mat4x3>();
            os << ("\n[" + std::to_string(m[0][0]) + ", " + std::to_string(m[0][1]) + ", " + std::to_string(m[0][2]) + "]\n" +
                "[" + std::to_string(m[1][0]) + ", " + std::to_string(m[1][1]) + ", " + std::to_string(m[1][2]) + "]\n" +
                "[" + std::to_string(m[2][0]) + ", " + std::to_string(m[2][1]) + ", " + std::to_string(m[2][2]) + "]\n" +
                "[" + std::to_string(m[3][0]) + ", " + std::to_string(m[3][1]) + ", " + std::to_string(m[3][2]) + "]"); break;
        }

                            // Сэмплеры
        case GL_SAMPLER_2D:
        case GL_SAMPLER_3D:
        case GL_SAMPLER_CUBE:
        case GL_SAMPLER_2D_ARRAY:
        case GL_SAMPLER_CUBE_MAP_ARRAY:
            os << "texture unit " + std::to_string(entry.get<GLuint>()); break;

        case GL_IMAGE_2D:
            os << "image unit " + std::to_string(entry.get<GLuint>()); break;

        default:
            os << "<unknown type: " + std::to_string(entry.type) + ">"; break;
        }

        return os;
    }
}