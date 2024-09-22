#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

//�ص���������ÿ�δ��ڴ�С��������ʱ�򱻵��ã�ʹ�ӿڴ�С���洰�ڴ�С�仯
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//������ƣ��������˷��ؼ�(Esc)���������´�ѭ���ر�
void processInput(GLFWwindow *window);

//���ó���
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

//������ɫ����ƬԪ��ɫ��
const char *vertexShaderSource = "#version 460 core\n"           //�汾��
    "layout (location = 0) in vec3 aPos;\n"                      //λ�ñ���������λ��ֵΪ 0
    "layout (location = 1) in vec3 aColor;\n"                    //��ɫ����������λ��ֵΪ 1
    "out vec3 ourColor;\n"
    "uniform vec2 xy;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x + xy.x, aPos.y + xy.y, aPos.z, 1.0);\n"      //������ɫ�������
    "   ourColor = aColor;\n"                  //��ourColor����Ϊ���ǴӶ�����������õ���������ɫ
    "}\0";
const char *fragmentShaderSource = "#version 460 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"                                     //ȫ�ֱ��������ڳ���������
    "void main()\n"
    "{\n"
    "   FragColor = vec4(ourColor, 1.0);\n"             //RGB��͸����
    "}\n\0";

int main() {

    glfwInit();//��ʼ��GLFW��Ϊ����GLFW������ȫ������
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);//���汾��
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);//�ΰ汾��
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//ָ�� OpenGL �����ĵ� �����ļ�

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);//����һ�����ڲ����ô��ڴ�С������
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);//֪ͨGLFW�����Ǵ��ڵ�����������Ϊ��ǰ�̵߳���������
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//����GLFW����ϣ��ÿ�����ڵ�����С��ʱ�����

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))//GLAD����������OpenGL�ĺ���ָ�룬�ڵ����κ�OpenGL�ĺ���֮ǰ������Ҫ��ʼ��GLAD
    {
     std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    //�������ǵ���ɫ��
    //����������ɫ��
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);//ID���ò��ṩ����Ϊ������ɫ
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);//��ɫ��Դ�븽�ӵ���ɫ������1��ʾ�ַ�������
    glCompileShader(vertexShader);//����Դ��
    //�������Ƿ�ɹ�
    int success;
    char infoLog[512];//�洢������Ϣ
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);//����Ƿ�ɹ�����
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);//����ʧ�ܾͻ�ȡ������Ϣ
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;//���������Ϣ
    }
    //����Ƭ����ɫ��
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);//ID���ò��ṩ����ΪƬ����ɫ
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);//��ɫ��Դ�븽�ӵ���ɫ������1��ʾ�ַ�������
    glCompileShader(fragmentShader);//����Դ��
    //�������Ƿ�ɹ�
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);//����Ƿ�ɹ�����
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);//����ʧ�ܾͻ�ȡ������Ϣ
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;//���������Ϣ
    }
    //������ɫ��
    unsigned int shaderProgram = glCreateProgram();//����һ���������
    glAttachShader(shaderProgram, vertexShader);//���Ӷ�����ɫ��
    glAttachShader(shaderProgram, fragmentShader);//����Ƭ����ɫ��
    glLinkProgram(shaderProgram);//����
    //���������ɫ�������Ƿ�ɹ�
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    //������֮��Ͳ���Ҫ������ɫ����Ƭ����ɫ����
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //��������
     float vertices[] = {
        // λ��              // ��ɫ
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // ����
       -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // ����
        0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // ����
    };
    // unsigned int indices[] = {  // ע��������0��ʼ!
    //     0, 1, 3,  // ��һ��������
    //     1, 2, 3   // �ڶ���������
    // };

    //VAO�洢�������ԣ�VBO�洢��������
    unsigned int VBO, VAO,EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    //glGenBuffers(1, &EBO);
    //��VAO
    glBindVertexArray(VAO);

    //���ƶ������鵽�����й�OpenGLʹ��
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //���������Ƶ�������
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //���ö�������ָ��
    //λ������
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);//���ö������ԣ�Ĭ�Ͻ��ã�
    //��ɫ����
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    // �� glVertexAttribPointer �ĵ��ý� VBO ע��Ϊ�������԰󶨵Ķ��㻺�������˿��԰�ȫ��ȡ����
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    //ͨ���������ȡ���� VAO������ VBO��
    glBindVertexArray(0); 


    //��Ⱦѭ��
    while(!glfwWindowShouldClose(window))
    {
        //����
        processInput(window);//�������ƣ�Esc�˳�����

        //��Ⱦ
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//������Ⱦǰ��Ҫ����
        glClear(GL_COLOR_BUFFER_BIT);
        //��һ��������
        glUseProgram(shaderProgram);//������ɫ����

        double  timeValue = glfwGetTime();
        float xValue = static_cast<float>(sin(timeValue) / 2.0f);
        float yValue = static_cast<float>(cos(timeValue) / 2.0f);
        int vertexXYLocation = glGetUniformLocation(shaderProgram, "xy");
        glUniform2f(vertexXYLocation, xValue, yValue);

        glBindVertexArray(VAO); //Ŀǰֻ��һ��VAO������Ҫÿ�ζ���
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//����ģʽ������������������͡�EBOƫ����
        glDrawArrays(GL_TRIANGLES, 0, 3);//��һ��������ʾ�������ͣ������Σ���0��ʾ��ʼ������3��ʾ������

        //��鲢�����¼�����������
        glfwSwapBuffers(window);//������ɫ���壬��������
        glfwPollEvents();//�����û�д���ʲô�¼�������������롢����ƶ��ȣ������´���״̬�������ö�Ӧ�Ļص�����    
    }

    //�ͷ���Դ
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);


    glfwTerminate();//��Ⱦ�������ͷ���Դ
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);//�����ӿڣ�����Ⱦ���ڵĳߴ磬ǰ�����������ƴ������½ǵ�λ�á��������͵��ĸ�����������Ⱦ���ڵĿ�Ⱥ͸߶�
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}