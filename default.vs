#version 410
layout (location = 0) in vec2 position; // The position variable has attribute position 0
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
flat out vec4 center;
void main()
{
    gl_PointSize=10;
    center = projection*model*view*vec4((position.x/10),(position.y/10),-1.0f,1.0); // See how we directly give a vec3 to vec4's constructor
    gl_Position=center;
}
