#version 330 core

in vec2 TexCoords;

out vec4 color;

uniform vec2 u_resolution;
uniform vec3 u_color;
uniform sampler2D image;

vec4 circle(vec2 uv)
{
    vec4 col = vec4(1);
    float d = length(uv);
    col.a = smoothstep(0.5, 0.49, d);
    return col;
}

void main()
{
	vec2 uv = TexCoords.xy;
    uv -= 0.5;
    uv.x *= u_resolution.x / u_resolution.y; 

  color = circle(uv) * vec4(u_color, 1.0) * texture(image, uv);
}