#version 410
out vec4 color;
in float red;
in float green;
in float blue;
in float bright;
void main()
{
  color=vec4(bright*red,bright*green,bright*blue,1.0f);
}
