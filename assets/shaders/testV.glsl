#version 420 core

in vec3 aPos[3];
in vec4 aColor;

layout(std140) uniform CameraBlock {
    mat4 uView[3];
    mat4 uProj;
    vec3 uCameraPos;
    float uTime;
} mas[3];

layout(std140) uniform SomeBlock {
    mat4 uView;
    mat4 uProj;
};

out vec4 outColor;

uniform mat4 projection;
uniform int jopa[2];

void main() {
    outColor = aColor;
    gl_Position = mas[0].uProj * mas[0].uView[0] * projection * vec4(aPos[0], 1.0);
    mas[1];
    mas[2];
    jopa;
}