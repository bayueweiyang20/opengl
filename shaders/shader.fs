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

   float specularStrength = 0.5; // ����ǿ��
   vec3 viewDir = normalize(viewPos-FragPos); // ���߷�����
   vec3 reflectDir = reflect(-lightDir, norm); // �����
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); // �ȼ������߷����뷴�䷽��ĵ�ˣ���ȷ�������Ǹ�ֵ����32�Ǹ߹�ķ����
   vec3 specular = specularStrength * spec * lightColor; // ���㾵�����

   vec3  result = (ambient + diffuse + specular) * objectColor;
   FragColor = vec4(result, 1.0);
}