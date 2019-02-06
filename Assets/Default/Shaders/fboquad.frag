#version 330 core

in vec2 UV;

out vec4 color;

uniform sampler2D renderedTexture;
uniform sampler2D depthTexture;


float LinearizeDepth(vec2 uv)
{
  float n = 0.2; // camera z near
  float f = 100.0; // camera z far
  float z = texture2D(depthTexture, uv).x;
  return (2.0 * n) / (f + n - z * (f - n));
}

void main(){
        color = texture2D( renderedTexture,UV);
        //float d=LinearizeDepth(UV);
       //color=vec4(d,d,d,1);
        //color=vec4(UV,0,1);
}
