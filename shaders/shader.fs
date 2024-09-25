#version 450 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1; // 采样器
uniform sampler2D texture2; // 采样器

void main()
{
   //FragColor = texture(ourTexture, TexCoord);  //采样纹理的颜色
   FragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(1.0 - TexCoord.x, TexCoord.y)), 0.2) * vec4(ourColor, 1.0); // 把得到的纹理颜色与顶点颜色混合，纹理颜色与顶点颜色相乘来混合二者颜色
}