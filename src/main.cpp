#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cmath>

#include "learnopengl/shader_s.h"
#include "learnopengl/stb_image.h"
#include "learnopengl/camera.h"

// �ص���������ÿ�δ��ڴ�С��������ʱ�򱻵��ã�ʹ�ӿڴ�С���洰�ڴ�С�仯
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//���ص�����
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
// �����ֻص�����
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
// ������ƣ��������˷��ؼ�(Esc)���������´�ѭ���ر�
void processInput(GLFWwindow *window);

// ���ó���
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

// ����ͷ
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;

// ��Դλ��
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// ��ϵĵڶ�������͸����
float mixValue = 0.2f;

// ������һ֡��Ⱦ��ʱ��
float deltaTime = 0.0f; // ��ǰ֡����һ֡��ʱ���
float lastFrame = 0.0f; // ��һ֡��ʱ��

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
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // �ص�����������GLFW����ϣ��ÿ�����ڵ�����С��ʱ�����
    glfwSetCursorPosCallback(window, mouse_callback); // ���ص�����
    glfwSetScrollCallback(window, scroll_callback); // ���ֻص�����

     // ��׽��꣬����glfw��׽���
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) // GLAD����������OpenGL�ĺ���ָ�룬�ڵ����κ�OpenGL�ĺ���֮ǰ������Ҫ��ʼ��GLAD
    {
     std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // ����ȫ��״̬
    glEnable(GL_DEPTH_TEST); // ��Ȳ���

    Shader ourShader("./shaders/shader.vs", "./shaders/shader.fs");
    Shader lampShader("./shaders/lampshader.vs", "./shaders/lampshader.fs");

    //��������
    float vertices[] = {
         // λ��               // ����             
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
     
    };
    // ��������壬������ռ䲻ͬ��λ����
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f)  
    };

    // VAO�洢�������ԣ�VBO�洢��������
    unsigned int VBO, VAO;
    //unsigned int EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    //glGenBuffers(1, &EBO);
    // ��VAO
    glBindVertexArray(VAO);

    // ���ƶ������鵽�����й�OpenGLʹ��
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // ���ö�������ָ��
    // λ������
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); // ���ö������ԣ�Ĭ�Ͻ��ã�

     // ��ԴlightVAO
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    // ֻ��Ҫ��VBO�����ٴ�����VBO�����ݣ���Ϊ���ӵ�VBO�������Ѿ���������ȷ�������嶥������
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // ���õ�������Ķ������ԣ������ǵĵ���˵����ֻ��λ�����ݣ�
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    // ��Ⱦѭ��
    while(!glfwWindowShouldClose(window))
    {
        // ����ÿ֡��ʱ������ƽ�ⲻͬ�û��ƶ�����ͷ���豸��ʾ�ٶ�
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // ����
        processInput(window); // �������ƣ�Esc�˳�����

        // ��Ⱦ
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // ������Ⱦǰ��Ҫ����
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // �����Ȳ���

        ourShader.use(); // ������ɫ����
        ourShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        ourShader.setVec3("lightColor",  1.0f, 1.0f, 1.0f);
        // �۲����
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("view", view);
        // ͶӰ����
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection); // ͶӰ��������б仯���������ǽ��䶨��������
        
        glBindVertexArray(VAO);
        // ���������ͬλ�õ�������
        for(unsigned int i = 0; i < 1; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i; 
            model = glm::rotate(model, (glm::radians(angle) + (float)glfwGetTime() * glm::radians(50.0f)), glm::vec3(1.0f, 0.3f, 0.5f));
            ourShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        lampShader.use();
        lampShader.setMat4("projection", projection);
        lampShader.setMat4("view", view);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        lampShader.setMat4("model", model);
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        // ��鲢�����¼�����������
        glfwSwapBuffers(window);// ������ɫ���壬��������
        glfwPollEvents();// �����û�д���ʲô�¼�������������롢����ƶ��ȣ������´���״̬�������ö�Ӧ�Ļص�����    
    }

    //�ͷ���Դ
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // glDeleteBuffers(1, &EBO);
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

    //W��A��S��D��������ͷ����
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// һ������ƶ����ͻ���ûص�����
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// һ�����ֹ������͵��øûص�����
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}