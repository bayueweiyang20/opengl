#version 450 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture; // 采样器

void main()
{
   //FragColor = texture(ourTexture, TexCoord);  //采样纹理的颜色
   FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0); // 把得到的纹理颜色与顶点颜色混合，纹理颜色与顶点颜色相乘来混合二者颜色
}