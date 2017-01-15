#version 410
layout (location = 0) in vec3 position; // The position variable has attribute position 0
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec3 camera;
flat out vec4 center;
out float bright;
out float red;
out float blue;
out float green;
uniform float brightnessConstant;
void main()
{
    const float spacing=2.0f;
    const float maxSize = 25.0f;
    gl_PointSize=10;
    red = abs(position.z)/200;
    blue = abs(position.z)/200;
    green = sqrt(1-pow(red,2)-pow(blue,2));
    vec3 newposition = vec3((position.x/spacing),(position.y/spacing),(position.z/spacing));
    center = projection*model*view*vec4(newposition,1.0); // See how we directly give a vec3 to vec4's constructor
     float radius = distance(newposition,camera);
     if(radius<maxSize)
   {
     bright=2.0*log(30/radius);
   }
   else{
      bright=0.25f*log(100/radius);
}
  bright=1.0f;
    gl_Position=center;
}
