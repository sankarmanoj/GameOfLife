#version 410
out vec4 color;
in float bright;
void main()
{
  color=vec4(bright*0.1f,bright*0.9f,bright*0.2f,bright*1.0f);
}
