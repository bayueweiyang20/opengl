#version 450 core

out vec4 FragColor;
in vec2 TexCoord;

uniform vec3 objectColor;
uniform vec3 lightColor;



void main()
{
   FragColor = vec4(lightColor * objectColor, 1.0);
}