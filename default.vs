#version 410
layout (location = 0) in vec3 position; // The position variable has attribute position 0
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec3 camera;
flat out vec4 center;
out float bright;
uniform float brightnessConstant;
void main()
{
    const float spacing=10.0f;
    gl_PointSize=10;
    vec3 newposition = vec3((position.x/spacing),(position.y/spacing),(position.z/spacing));
    center = projection*model*view*vec4(newposition,1.0); // See how we directly give a vec3 to vec4's constructor
    bright = 1/dot((newposition-camera),(newposition-camera));
    bright = bright * bright * brightnessConstant;
    if (bright > 1)
      bright = 1.0f;
    if (bright < 0)
      bright = 0.0f;
    gl_Position=center;
}
