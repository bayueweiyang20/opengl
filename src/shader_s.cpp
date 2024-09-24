#include "learnopengl/shader_s.h"




Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    // 1. ���ļ�·���л�ȡ����/Ƭ����ɫ��
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ��֤ifstream��������׳��쳣
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try 
    {
        // ���ļ�
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // ��ȡ�ļ��Ļ������ݵ���������
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // �ر��ļ�������
        vShaderFile.close();
        fShaderFile.close();
        // ת����������string
        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char * fShaderCode = fragmentCode.c_str();
    // 2. ������ɫ��
    unsigned int vertex, fragment;
    // ������ɫ��
    vertex = glCreateShader(GL_VERTEX_SHADER); // ID���ò��ṩ����Ϊ������ɫ
    glShaderSource(vertex, 1, &vShaderCode, NULL); // ��ɫ��Դ�븽�ӵ���ɫ������1��ʾ�ַ�������
    glCompileShader(vertex); // ����Դ��
    checkCompileErrors(vertex, "VERTEX"); // ����ɫ����ĺ��������Լ����ɫ������ɫ�������Ƿ��д���
    // Ƭ����ɫ��
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    // ������ɫ��->��ɫ������
    ID = glCreateProgram(); // ����һ���������
    glAttachShader(ID, vertex); // ���Ӷ�����ɫ��
    glAttachShader(ID, fragment); // ����Ƭ����ɫ��
    glLinkProgram(ID); // ����
    checkCompileErrors(ID, "PROGRAM");
    // ������֮��Ͳ���Ҫ������ɫ����Ƭ����ɫ����
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

// ������ɫ������
void Shader::use() 
{ 
    glUseProgram(ID); 
}

// uniform���ߺ���
//glGetUniformLocation��ȡָ��uniform������λ�ã���glUniform1i������ֵ���õ���λ��
void Shader::setBool(const std::string &name, bool value) const
{         
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
}
//----
void Shader::setInt(const std::string &name, int value) const
{ 
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
}
//----
void Shader::setFloat(const std::string &name, float value) const
{ 
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
}

// ��������ɫ������ɫ���������
void Shader::checkCompileErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

