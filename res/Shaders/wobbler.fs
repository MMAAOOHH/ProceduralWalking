#version 330 core

in vec2 TexCoords;
in vec2 f_Position;

out vec4 color;

uniform vec2 u_resolution;
uniform vec3 u_color;
uniform sampler2D image;
uniform float u_time;
uniform vec2 u_screenResolution;


vec4 circle(vec2 uv)
{
    vec4 col = vec4(1);
    float d = length(uv);
    col.a = smoothstep(0.5, 0.49, d);
    return col;
}

float wobble(float time, float p, float amplitude, float frequence, float speed)
{
    return amplitude * sin(p * frequence + time * speed);
}

void main()
{

  vec2 uv = TexCoords.xy;
  uv -= 0.5;
  uv.x *= u_resolution.x / u_resolution.y; 

  vec2 uvscreen = TexCoords;
  // Wobble
  float t = u_time;
  float a = 0.01;
  float f = 0.5;
  float s = 6.0;

  uv.x += wobble(t, uvscreen.x, a, f, s);
  uv.y += wobble(t, uvscreen.y, a, f, s);

  color = circle(uv) * vec4(u_color, 1.0) * texture(image, uv);

}