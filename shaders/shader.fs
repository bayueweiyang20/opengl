#version 450 core
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float     shininess;
}; 
struct Light {
    vec3 position; // ʹ�ö����Ͳ�����Ҫ��
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
    // ������
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords)); // ��������
    // ������
    vec3  norm = normalize(Normal);
    vec3  lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords)); // ��������
    // �����
    vec3 viewDir = normalize(viewPos-FragPos); // ���߷�����
    vec3 reflectDir = reflect(-lightDir, norm); // �����
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); // �ȼ������߷����뷴�䷽��ĵ�ˣ���ȷ�������Ǹ�ֵ����32�Ǹ߹�ķ����
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords)); // ���㾵�����
    // ˥��
    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    ambient  *= attenuation;  
    diffuse   *= attenuation;
    specular *= attenuation;
    // �����
    //vec3 emission = texture(material.emission, TexCoords).rgb;
    //vec3 emission = texture(material.emission,vec2(TexCoords.x,TexCoords.y+matrixmove)).rgb;

    vec3  result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}