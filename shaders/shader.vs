#version 450 core 

layout (location = 0) in vec3 aPos; // λ�ñ���������λ��ֵΪ 0
layout (location = 1) in vec3 aColor; // ��ɫ����������λ��ֵΪ 1

out vec3 ourColor;

uniform vec2 xy;

void main()
{
    gl_Position = vec4(aPos.x + xy.x, aPos.y + xy.y, aPos.z, 1.0); // ������ɫ�������
    ourColor = aColor; // ��ourColor����Ϊ���ǴӶ�����������õ���������ɫ
}