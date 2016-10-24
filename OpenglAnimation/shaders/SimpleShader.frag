#version 330 core

flat in int hide;
// Ouput data
out vec4 color;
in vec2 uv;
in vec3 normal;
in vec3 lightDir;

uniform sampler2D diffuse;

void main()
{
	float brightness = dot(lightDir, normal);
	vec4 dif = texture2D(diffuse, uv.xy);
	dif *= brightness;
	dif.w = texture2D(diffuse, uv).a;
	color = dif;
}
