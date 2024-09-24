#version 450 core 

layout (location = 0) in vec3 aPos; // 位置变量的属性位置值为 0
layout (location = 1) in vec3 aColor; // 颜色变量的属性位置值为 1

out vec3 ourColor;

uniform vec2 xy;

void main()
{
    gl_Position = vec4(aPos.x + xy.x, aPos.y + xy.y, aPos.z, 1.0); // 顶点着色器的输出
    ourColor = aColor; // 将ourColor设置为我们从顶点数据那里得到的输入颜色
}