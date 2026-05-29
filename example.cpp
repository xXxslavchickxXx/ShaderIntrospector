#include <iostream>
#include <shaderProgram/ShaderProgram.h>
#include <GLFW/glfw3.h>

#include <uniformReflector/uniformReflector.h>
#include <uniformBlockReflector/uniformBlockReflector.h>
#include <attributeReflector/attributeReflector.h>

int main() {
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(800, 600, "title", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glewInit();

    auto program = shader::ShaderProgram::from_path("assets/shaders/testV.glsl", "assets/shaders/testF.glsl");
    // Юниформ блок
    auto uniblockref = shader::uniform_block_reflector(program.getId());
    try {
        std::cout << uniblockref["CameraBlock"]["uView"][0] << '\n';
    }
    catch (std::exception& e) {
        std::cerr << e.what();
    }

    // Атрибуты
    auto attribref = shader::attribute_reflector(program.getId());
    //std::cout << attribref;

    // Юниформы
    auto uniref = shader::uniform_reflector(program.getId());
    uniref["jopa"][0] = 4;
    uniref["jopa"][1] = 7;
    uniref["projection"] = glm::mat4(1.f);
    //std::cout << uniref;
}