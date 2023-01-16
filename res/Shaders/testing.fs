#version 330 core

in vec2 TexCoords;
out vec4 color;

uniform vec3 u_color;
uniform vec2 u_resolution;

vec4 circle(vec2 uv)
{
    vec4 col = vec4(1);
    float d = length(uv);
    col.a = smoothstep(0.5, 0.49, d);
    return col;
}

vec4 body(vec2 uv)
{
    vec4 col = vec4(0);
    vec4 base = circle(uv);
    col = mix(col, base, base.a);

    if(length(uv - vec2(0.2,0.1)) < 0.1)
        {
            vec4 eye = circle(uv) * vec4(1,1,0,0);
            col = mix(col, eye, eye.a);
        }

    return col;

}

void main()
{
	vec2 uv = TexCoords.xy / u_resolution.xy;
    uv -= 0.5;
    uv.x *= u_resolution.x / u_resolution.y;

    color = body(uv) * vec4(u_color, 1.0);
}