#version 450 core
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float     shininess;
}; 
struct Light {
    vec3 position; // 使用定向光就不再需要了
    // vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};


out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform float matrixmove;

void main()
{
    // 环境光
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords)); // 加上纹理
    // 漫反射
    vec3  norm = normalize(Normal);
    vec3  lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords)); // 加上纹理
    // 镜面光
    vec3 viewDir = normalize(viewPos-FragPos); // 视线反方向
    vec3 reflectDir = reflect(-lightDir, norm); // 反射光
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); // 先计算视线方向与反射方向的点乘（并确保它不是负值），32是高光的反射度
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords)); // 计算镜面分量
    // 衰减
    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    ambient  *= attenuation;  
    diffuse   *= attenuation;
    specular *= attenuation;
    // 放射光
    //vec3 emission = texture(material.emission, TexCoords).rgb;
    //vec3 emission = texture(material.emission,vec2(TexCoords.x,TexCoords.y+matrixmove)).rgb;

    vec3  result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}