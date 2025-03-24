#version 450

#define MAX_TEXTURES 10

layout( location = 0 ) in vec4 fragColor;
layout( location = 1 ) in vec2 fragTexCoord;
layout( location = 2 ) flat in int samplerId;

layout( binding = 1 ) uniform sampler2D texSamplers[MAX_TEXTURES];

layout( location = 0 ) out vec4 outColor;

void main()
{
  if ( samplerId > 0 )
  {
    vec4 texColor = texture( texSamplers[samplerId], fragTexCoord );
    outColor      = vec4( texColor * texColor.a ) * fragColor;
  }
  else
  {
    outColor = fragColor;
  }
}