#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float radius;

void main(){

	vec3 scaledPos = aPos * radius;
	gl_Position = projection * view * model * vec4(scaledPos, 1.0f);
};
