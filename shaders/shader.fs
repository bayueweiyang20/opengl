#version 450 core

out vec4 FragColor;

in vec3 ourColor;

uniform vec3 rgb; // ȫ�ֱ��������ڳ���������

void main()
{
   FragColor = vec4(ourColor + rgb, 1.0); // RGB��͸����
}