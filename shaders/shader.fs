#version 450 core

out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
   float ambientStrength = 0.1;
   vec3  ambient = ambientStrength * lightColor;

   vec3  norm = normalize(Normal);
   vec3  lightDir = normalize(lightPos - FragPos);
   float diff = max(dot(norm, lightDir), 0.0);
   vec3  diffuse = diff * lightColor;

   float specularStrength = 0.5; // 镜面强度
   vec3 viewDir = normalize(viewPos-FragPos); // 视线反方向
   vec3 reflectDir = reflect(-lightDir, norm); // 反射光
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); // 先计算视线方向与反射方向的点乘（并确保它不是负值），32是高光的反射度
   vec3 specular = specularStrength * spec * lightColor; // 计算镜面分量

   vec3  result = (ambient + diffuse + specular) * objectColor;
   FragColor = vec4(result, 1.0);
}