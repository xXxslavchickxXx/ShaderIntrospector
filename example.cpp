#include <iostream>
#include <shaderProgram/ShaderProgram.h>
#include <GLFW/glfw3.h>

#include <uniformReflector/uniformReflector.h>

int main() {
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(800, 600, "title", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    glewInit();

    glClearColor(0.3, 0.3, 0.8, 1.f);

    auto program = shader::ShaderProgram::from_path("assets/shaders/testV.glsl", "assets/shaders/testF.glsl");
    
    auto uniref = shader::uniform_reflector(program.getId());
    auto& unientry = uniref["jopa"];

    uniref["jopa"] = 4;
    uniref["jopa"][1] = 7;

    std::cout << unientry;
}