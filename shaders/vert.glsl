#version 430 core

layout(location = 0) in vec2 aPos;
uniform float pointSize;

void main()
{
    gl_Position = vec4(aPos, 0.0, 1.0);
    gl_PointSize = pointSize;
}
