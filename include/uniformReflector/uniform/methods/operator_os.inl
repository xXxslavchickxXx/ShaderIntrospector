#include <iomanip> // для прсижина
#include <toString/toString.h>

namespace shader {
    inline std::ostream& operator<<(std::ostream& os, const uniform_entry& entry) {
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

        if (entry.elements > 1) {
            for (int i = 0; i < entry.elements; ++i) {
                auto elem = entry[i];
                os << indent << "[" << i << "] = ";

                switch (elem.type) {
                    case GL_FLOAT: os << elem.get<float>(); break;
                    case GL_INT: os << elem.get<int>(); break;
                    case GL_UNSIGNED_INT: os << elem.get<unsigned int>(); break;
                    case GL_BOOL: os << (elem.get<bool>() ? "true" : "false"); break;

                    case GL_FLOAT_VEC2: print_vec(elem.get<glm::vec2>()); break;
                    case GL_FLOAT_VEC3: print_vec(elem.get<glm::vec3>()); break;
                    case GL_FLOAT_VEC4: print_vec(elem.get<glm::vec4>()); break;

                    case GL_FLOAT_MAT2: print_matrix(elem.get<glm::mat2>()); break;
                    case GL_FLOAT_MAT3: print_matrix(elem.get<glm::mat3>()); break;
                    case GL_FLOAT_MAT4: print_matrix(elem.get<glm::mat4>()); break;
                    default: os << "?";
                }

                if (i < entry.elements - 1) os << "\n";
            }
            return os;
        }

        switch (entry.type) {
        case GL_FLOAT: os << indent << entry.get<float>(); break;
        case GL_INT: os << indent << entry.get<int>(); break;
        case GL_UNSIGNED_INT: os << indent << entry.get<unsigned int>(); break;
        case GL_BOOL: os << indent << (entry.get<bool>() ? "true" : "false"); break;

        case GL_FLOAT_VEC2: print_vec(entry.get<glm::vec2>()); break;
        case GL_FLOAT_VEC3: print_vec(entry.get<glm::vec3>()); break;
        case GL_FLOAT_VEC4: print_vec(entry.get<glm::vec4>()); break;

        case GL_FLOAT_MAT2: print_matrix(entry.get<glm::mat2>()); break;
        case GL_FLOAT_MAT3: print_matrix(entry.get<glm::mat3>()); break;
        case GL_FLOAT_MAT4: print_matrix(entry.get<glm::mat4>()); break;

        default:
            os << indent << "<unknown>";
            break;
        }

        return os;
    }
}