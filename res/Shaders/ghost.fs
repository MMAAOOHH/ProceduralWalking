#version 330 core

#define S(a, b, t) smoothstep(a, b, t)
#define B(a, b, blur, t) S(a-blur, a+blur, t)*S(b+blur, b-blur, t)
#define sat(x) clamp(x, 0., 1.)

in vec2 TexCoords;
out vec4 color;

uniform vec2 u_resolution;
uniform float u_radius;
uniform vec3 u_color;
uniform sampler2D image;
uniform vec2 u_screenResolution;
uniform vec2 u_mousePos;
uniform float u_time;

float remap01(float a, float b, float t) {
	return sat((t-a)/(b-a));
}

float remap(float a, float b, float c, float d, float t) {
	return sat((t-a)/(b-a)) * (d-c) + c;
}

vec2 within(vec2 uv, vec4 rect) 
{
	return (uv - rect.xy)/(rect.zw - rect.xy);
}

vec4 rectOffsetY(vec4 rect, float offset)
{
    return vec4(rect.x, rect.y + offset, rect.z, rect.w + offset);
}

vec4 Eye(vec2 uv, float side, vec2 m) 
{
    uv -= 0.5;
    uv.x *= side;
    
	float d = length(uv);
    
    vec4 col = vec4(0);
    col.a = S(0.5, 0.1, d);									// eye mask
    
    d = length(uv-m*.45);									// offset pupile to look at mouse cursor
    
    float pupilSize = 0.2;
    float pupilMask = S(pupilSize, pupilSize * 0.8, d);

    col.rgb = mix(col.rgb, vec3(1), pupilMask);		// blend in pupil
    
    return col;
}


vec4 Head(vec2 uv) 
{
	vec4 col = vec4(1);
    float d = length(uv);
    col.a = smoothstep(.5, .49, d);
    return col;
}

vec4 Ghost(vec2 uv, vec2 m) 
{
    vec4 col = vec4(0.0);
   if(length(uv)<.5)
   {					
        float side = sign(uv.x);
        uv.x = abs(uv.x);
        vec4 head = Head(uv);
        col = mix(col, head, head.a);
           
        if(length(uv - vec2(0.2, 0.1)) < 0.135) 
        {
            vec4 rect = vec4(0, -0.05, 0.4, 0.25);
            vec4 eye = Eye(within(uv, rect), side, m);
            col = mix(col, eye, eye.a);
        }
    }
    return col;
}



float wobble(float time, float p, float amplitude, float frequence, float speed)
{
    return amplitude * sin(p * frequence + time * speed);
}

void main()
{
    vec2 uv = TexCoords.xy;
    uv -= .5;
    uv.x *= u_resolution.x/u_resolution.y;
    
    vec2 uvscreen = TexCoords / u_screenResolution.xy;
    vec2 mouse = u_mousePos.xy / u_screenResolution.xy;
    mouse -= .5;
    
    // Eyepos 
    /*
    float d = dot(uv, uv);
    uv -= mouse*sat(.15-d);
    */
    //Wobble
    /*
    float t = u_time;
    float a = 0.01;
	float f = 0.5;
	float s = 4.0;

    uv.x += wobble(t, uvscreen.x, a, f, s);
    uv.y += wobble(t, uvscreen.y, a, f, s);
    */
    // out    
	color = Head(uv) * vec4(u_color, 1.0);
}