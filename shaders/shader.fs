#version 450 core

out vec4 FragColor;

in vec3 ourColor;

uniform vec3 rgb; // 全局变量，可在程序中设置

void main()
{
   FragColor = vec4(ourColor + rgb, 1.0); // RGB和透明度
}