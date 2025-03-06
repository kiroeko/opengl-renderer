#version 460 core
layout (location = 0) in vec3 aPos;

uniform mat4 modelm;
uniform mat4 viewm;
uniform mat4 projectionm;

void main()
{
    gl_Position = projectionm * viewm * modelm * vec4(aPos, 1.0f);
}