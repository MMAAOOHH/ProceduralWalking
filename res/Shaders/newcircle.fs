#version 330 core

in vec2 TexCoords;
out vec4 color;

uniform vec2 u_resolution;
uniform float u_radius;
uniform vec3 u_color;
uniform sampler2D image;

float circle(vec2 position, float radius)
{
  return 1 - step(radius, length(position - vec2(0.5)));
}

void main()
{
  vec2 position = TexCoords / u_resolution;

  vec3 col = vec3(0);
  float c = circle(position, u_radius);
  col = vec3(c);

  color = vec4(col, 0)  * vec4(u_color, 0) * texture(image, TexCoords) ;
}