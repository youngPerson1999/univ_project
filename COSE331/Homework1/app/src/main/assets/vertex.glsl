#version 300 es

uniform mat4 worldMat, viewMat, projMat;
uniform vec3 lightPos;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out vec3 v_normal;
out vec2 v_texCoord;
out vec3 v_lightDir;

void main() {
    float scale = 1.0;

    //////////////////////////////
    /* TODO: Problem 1.
    *  Fill in the lines below.
    *  Scale the part of the teapot below XZ plane.
    */
    vec4 scaledPosition = vec4(position, 1.0);
    mat4 scalingMatrix = mat4(
        1.5, 0.0, 0.0, 0.0,
        0.0, 1.5, 0.0, 0.0,
        0.0, 0.0, 1.5, 0.0,
        0.0, 0.0, 0.0, 1.0
    );
    if (scaledPosition.y < 0.0) //y축 밑만 1.5배, 나머지는 그대로
        scaledPosition = scalingMatrix * scaledPosition;
    gl_Position = projMat * viewMat * worldMat * scaledPosition;
    v_normal = normal;
    v_texCoord = texCoord;

    //////////////////////////////

    vec3 posWS = (worldMat * vec4(position, 1.0)).xyz;
    v_lightDir = normalize(lightPos - posWS);
}
