#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cmath>

#include "learnopengl/shader_s.h"
#include "learnopengl/stb_image.h"

//�ص���������ÿ�δ��ڴ�С��������ʱ�򱻵��ã�ʹ�ӿڴ�С���洰�ڴ�С�仯
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//������ƣ��������˷��ؼ�(Esc)���������´�ѭ���ر�
void processInput(GLFWwindow *window);

//���ó���
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

// ��ϵĵڶ�������͸����
float mixValue = 0.2f;

int main() {

    glfwInit(); // ��ʼ��GLFW��Ϊ����GLFW������ȫ������
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // ���汾��
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5); // �ΰ汾��
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // ָ�� OpenGL �����ĵ� �����ļ�

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL); // ����һ�����ڲ����ô��ڴ�С������
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // ֪ͨGLFW�����Ǵ��ڵ�����������Ϊ��ǰ�̵߳���������
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // ����GLFW����ϣ��ÿ�����ڵ�����С��ʱ�����

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) // GLAD����������OpenGL�ĺ���ָ�룬�ڵ����κ�OpenGL�ĺ���֮ǰ������Ҫ��ʼ��GLAD
    {
     std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    Shader ourShader("./shaders/shader.vs", "./shaders/shader.fs");

    //��������
    float vertices[] = {
         // λ��               // ��ɫ             // ����
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // ����
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // ����
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // ����
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // ���� 
    };
    unsigned int indices[] = {  // ע��������0��ʼ!
        0, 1, 3,  // ��һ��������
        1, 2, 3   // �ڶ���������
    };

    // VAO�洢�������ԣ�VBO�洢��������
    unsigned int VBO, VAO;
    unsigned int EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // ��VAO
    glBindVertexArray(VAO);

    // ���ƶ������鵽�����й�OpenGLʹ��
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // ���������Ƶ�������
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // ���ö�������ָ��
    // λ������
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); // ���ö������ԣ�Ĭ�Ͻ��ã�
    // ��ɫ����
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    //��������
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // ���غʹ�������
    unsigned int texture1,texture2;
    glGenTextures(1, &texture1); // ID����
    glBindTexture(GL_TEXTURE_2D, texture1); // �󶨸�����
    // Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // ���ز���������1
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // OpenGL��ͼƬ�����y���Ƿ��ģ���������������Ҫ��������
    unsigned char *data = stbi_load("./images/container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); // ����Ŀ�ꡢ�༶��Զ�����𡢴洢��ʽ������0...
        glGenerateMipmap(GL_TEXTURE_2D); // Ϊ��ǰ�󶨵�������󸽼�������ͼ��
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    // ���ز���������2
    glGenTextures(1, &texture2); // ID����
    glBindTexture(GL_TEXTURE_2D, texture2); // �󶨸�����
    // Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    data = stbi_load("./images/awesomeface.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); // ����ʹ��GL_RGBA����һ��͸����
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // �����ĸ�uniform��������Ӧ�ĸ�����Ԫ
    ourShader.use(); // ��Ҫ����������uniform����֮ǰ������ɫ������
    // �ֶ�����
    glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
    // ����ʹ����ɫ��������
    ourShader.setInt("texture2", 1);

    // �� glVertexAttribPointer �ĵ��ý� VBO ע��Ϊ�������԰󶨵Ķ��㻺�������˿��԰�ȫ��ȡ����
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    // ͨ���������ȡ���� VAO������ VBO��
    glBindVertexArray(0); //�ͷ�ͼ���ڴ�


    // ��Ⱦѭ��
    while(!glfwWindowShouldClose(window))
    {
        // ����
        processInput(window); // �������ƣ�Esc�˳�����

        // ��Ⱦ
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // ������Ⱦǰ��Ҫ����
        glClear(GL_COLOR_BUFFER_BIT);

        // �󶨶������
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // ���û�϶�
        ourShader.setFloat("mixValue", mixValue);

        // ���ɱ任
        glm::mat4 transform = glm::mat4(1.0f); 
        transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f)); // ��ת���Ƕ���ʱ��仯
        transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f)); // λ��
        // �任2
        glm::mat4 transform2 = glm::mat4(1.0f); 
        transform2 = glm::translate(transform2, glm::vec3(-0.5f, 0.5f, 0.0f)); // λ��
        transform2 = glm::scale(transform2, glm::vec3(abs(sin(glfwGetTime())) , abs(sin(glfwGetTime())), abs(sin(glfwGetTime())))); // ����
        

        // ��һ��������
        ourShader.use(); // ������ɫ����

        unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

        // double  timeValue = glfwGetTime();
        // float xValue = static_cast<float>(sin(timeValue) / 2.0f);
        // float yValue = static_cast<float>(cos(timeValue) / 2.0f);
        // int vertexXYLocation = glGetUniformLocation(ourShader.ID, "xy");
        // glUniform2f(vertexXYLocation, xValue, yValue);
        // float rValue = static_cast<float>(sin(timeValue) / 2.0f);
        // float gValue = static_cast<float>(sin(timeValue) / 2.0f);
        // float bValue = static_cast<float>(sin(timeValue) / 2.0f);
        // int vertexRGBLocation = glGetUniformLocation(ourShader.ID, "rgb");
        // glUniform3f(vertexRGBLocation, abs(rValue), abs(gValue), abs(bValue));

        glBindVertexArray(VAO); // Ŀǰֻ��һ��VAO������Ҫÿ�ζ���
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//����ģʽ������������������͡�EBOƫ����
        //glDrawArrays(GL_TRIANGLES, 0, 3); // ��һ��������ʾ�������ͣ������Σ���0��ʾ��ʼ������3��ʾ������

        //�ڶ���ͼ��
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform2));
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        // ��鲢�����¼�����������
        glfwSwapBuffers(window);// ������ɫ���壬��������
        glfwPollEvents();// �����û�д���ʲô�¼�������������롢����ƶ��ȣ������´���״̬�������ö�Ӧ�Ļص�����    
    }

    //�ͷ���Դ
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(ourShader.ID);


    glfwTerminate(); // ��Ⱦ�������ͷ���Դ
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height); // �����ӿڣ�����Ⱦ���ڵĳߴ磬ǰ�����������ƴ������½ǵ�λ�á��������͵��ĸ�����������Ⱦ���ڵĿ�Ⱥ͸߶�
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // �˳����ж�
        glfwSetWindowShouldClose(window, true);

     if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        mixValue += 0.002f; // ���Ӷ�����͸���ȣ���ֵΪ�仯�ٶ�
        if(mixValue >= 1.0f)
            mixValue = 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        mixValue -= 0.002f; // ���ٶ�����͸����
        if (mixValue <= 0.0f)
            mixValue = 0.0f;
    }
}