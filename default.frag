#version 410
out vec4 color;
flat in vec4 center;
uniform sampler2D tex;
void main()
{

  vec2 N;
  N =gl_PointCoord*2.0-vec2(1.0f);
 float mag = dot(N, N);
if (mag > 4.0f) discard;


    color=vec4(1.0f,0.0f,0.0f,1.0f);




}
