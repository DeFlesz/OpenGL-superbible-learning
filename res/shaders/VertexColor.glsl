#shader vertex
#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;

out vec4 vs_color;

layout (location = 0) uniform mat4 projection;
uniform mat4 transform;

void main()
{
  gl_Position = projection * transform * vec4(position, 1.0);

  vs_color = color;
}

#shader fragment
#version 450 core

in vec4 vs_color;

out vec4 FragColor;

void main()
{
  FragColor = vs_color;
}