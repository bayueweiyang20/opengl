#version 450 core

out vec4 FragColor;

in vec2 TexCoord;

uniform float mixValue;

uniform sampler2D texture1; // 采样器
uniform sampler2D texture2; // 采样器


void main()
{
   //FragColor = texture(ourTexture, TexCoord);  //采样纹理的颜色
   FragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(1.0 - TexCoord.x, TexCoord.y)), mixValue);
}