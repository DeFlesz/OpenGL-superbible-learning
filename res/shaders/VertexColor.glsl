#shader vertex
#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 uv;

out vec4 vs_color;
out vec2 vs_uv;

layout (location = 0) uniform mat4 projection;
uniform mat4 transform;

void main()
{
  gl_Position = projection * transform * vec4(position, 1.0);

  vs_color = vec4(position, 0.25) * 4;
  vs_uv = uv;
}

#shader fragment
#version 450 core

in vec4 vs_color;
in vec2 vs_uv;

out vec4 FragColor;

uniform sampler2D u_Texture;

void main()
{
  FragColor = texture(u_Texture, vs_uv);
  // FragColor = vs_color;
}