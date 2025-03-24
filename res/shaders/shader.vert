#version 450

layout( location = 0 ) in vec2 inPosition;
layout( location = 1 ) in vec4 inColor;
layout( location = 2 ) in vec2 inTexCoord;
layout( location = 3 ) in int inSamplerId;

layout( binding = 0 ) uniform UniformBufferObject
{
  mat4 model;
  mat4 view;
  mat4 proj;
}
ubo;

layout( location = 0 ) out vec4 fragColor;
layout( location = 1 ) out vec2 fragTexCoord;
layout( location = 2 ) flat out int samplerId;

void main()
{
  gl_Position = ubo.proj * ubo.view * ubo.model * vec4( inPosition, 0.0, 1.0 );

  fragColor    = inColor;
  fragTexCoord = inTexCoord;
  samplerId    = inSamplerId;
}