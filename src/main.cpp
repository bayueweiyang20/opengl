#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

#include "learnopengl/shader_s.h"

//回调函数，在每次窗口大小被调整的时候被调用，使视口大小跟随窗口大小变化
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//输入控制，若按下了返回键(Esc)，程序在下次循环关闭
void processInput(GLFWwindow *window);

//设置长宽
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

int main() {

    glfwInit();//初始化GLFW，为后续GLFW操作做全局设置
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);//主版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);//次版本号
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//指定 OpenGL 上下文的 配置文件

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);//创建一个窗口并设置窗口大小和名称
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);//通知GLFW将我们窗口的上下文设置为当前线程的主上下文
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//告诉GLFW我们希望每当窗口调整大小的时候调用

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))//GLAD是用来管理OpenGL的函数指针，在调用任何OpenGL的函数之前我们需要初始化GLAD
    {
     std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    Shader ourShader("./shaders/shader.vs", "./shaders/shader.fs");

    //顶点输入
     float vertices[] = {
        // 位置              // 颜色
        0.5f, -0.5f, 0.0f,  0.5f, 0.0f, 0.0f,   // 右下
       -0.5f, -0.5f, 0.0f,  0.0f, 0.5f, 0.0f,   // 左下
        0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 0.5f    // 顶部
    };
    // unsigned int indices[] = {  // 注意索引从0开始!
    //     0, 1, 3,  // 第一个三角形
    //     1, 2, 3   // 第二个三角形
    // };

    //VAO存储顶点属性，VBO存储顶点数据
    unsigned int VBO, VAO;
    //unsigned int EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    //glGenBuffers(1, &EBO);
    //绑定VAO
    glBindVertexArray(VAO);

    //复制顶点数组到缓冲中供OpenGL使用
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //把索引复制到缓冲里
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //设置顶点属性指针
    //位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);//启用顶点属性（默认禁用）
    //颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    // 对 glVertexAttribPointer 的调用将 VBO 注册为顶点属性绑定的顶点缓冲对象，因此可以安全地取消绑定
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    //通常情况不会取消绑定 VAO（还有 VBO）
    glBindVertexArray(0); 


    //渲染循环
    while(!glfwWindowShouldClose(window))
    {
        //输入
        processInput(window);//按键控制，Esc退出程序

        //渲染
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//在新渲染前需要清屏
        glClear(GL_COLOR_BUFFER_BIT);
        //画一个三角形
        ourShader.use();//激活着色程序

        double  timeValue = glfwGetTime();
        float xValue = static_cast<float>(sin(timeValue) / 2.0f);
        float yValue = static_cast<float>(cos(timeValue) / 2.0f);
        int vertexXYLocation = glGetUniformLocation(ourShader.ID, "xy");
        glUniform2f(vertexXYLocation, xValue, yValue);
        float rValue = static_cast<float>(sin(timeValue) / 2.0f);
        float gValue = static_cast<float>(sin(timeValue) / 2.0f);
        float bValue = static_cast<float>(sin(timeValue) / 2.0f);
        int vertexRGBLocation = glGetUniformLocation(ourShader.ID, "rgb");
        glUniform3f(vertexRGBLocation, abs(rValue), abs(gValue), abs(bValue));

        glBindVertexArray(VAO); //目前只有一个VAO，不需要每次都绑定
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//绘制模式、顶点个数、索引类型、EBO偏移量
        glDrawArrays(GL_TRIANGLES, 0, 3);//第一个参数表示绘制类型（三角形），0表示起始索引，3表示顶点数

        //检查并调用事件，交换缓冲
        glfwSwapBuffers(window);//交换颜色缓冲，用来绘制
        glfwPollEvents();//检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数    
    }

    //释放资源
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    //glDeleteBuffers(1, &EBO);
    glDeleteProgram(ourShader.ID);


    glfwTerminate();//渲染结束后释放资源
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);//设置视口，即渲染窗口的尺寸，前两个参数控制窗口左下角的位置。第三个和第四个参数控制渲染窗口的宽度和高度
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}