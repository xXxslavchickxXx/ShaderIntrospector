#version 330 core

in vec3 aPos[3];
in vec4 aColor;

layout(std140) uniform CameraBlock {
    mat4 uView[3];
    mat4 uProj;
    vec3 uCameraPos;
    float uTime;
};

out vec4 outColor;

uniform mat4 projection;
uniform int jopa[2];

void main() {
    outColor = aColor;
    gl_Position = uProj * uView[0] * projection * vec4(aPos[0], 1.0);
    jopa;
}