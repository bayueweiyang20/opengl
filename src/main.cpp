#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cmath>

#include "learnopengl/shader_s.h"
#include "learnopengl/stb_image.h"

//回调函数，在每次窗口大小被调整的时候被调用，使视口大小跟随窗口大小变化
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//输入控制，若按下了返回键(Esc)，程序在下次循环关闭
void processInput(GLFWwindow *window);

//设置长宽
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

// 混合的第二个纹理透明度
float mixValue = 0.2f;

int main() {

    glfwInit(); // 初始化GLFW，为后续GLFW操作做全局设置
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // 主版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5); // 次版本号
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 指定 OpenGL 上下文的 配置文件

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL); // 创建一个窗口并设置窗口大小和名称
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // 通知GLFW将我们窗口的上下文设置为当前线程的主上下文
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // 告诉GLFW我们希望每当窗口调整大小的时候调用

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) // GLAD是用来管理OpenGL的函数指针，在调用任何OpenGL的函数之前我们需要初始化GLAD
    {
     std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    Shader ourShader("./shaders/shader.vs", "./shaders/shader.fs");

    //顶点输入
    float vertices[] = {
         // 位置               // 颜色             // 纹理
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // 右上
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // 右下
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // 左下
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // 左上 
    };
    unsigned int indices[] = {  // 注意索引从0开始!
        0, 1, 3,  // 第一个三角形
        1, 2, 3   // 第二个三角形
    };

    // VAO存储顶点属性，VBO存储顶点数据
    unsigned int VBO, VAO;
    unsigned int EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // 绑定VAO
    glBindVertexArray(VAO);

    // 复制顶点数组到缓冲中供OpenGL使用
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 把索引复制到缓冲里
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 设置顶点属性指针
    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); // 启用顶点属性（默认禁用）
    // 颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    //纹理属性
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // 加载和创建纹理
    unsigned int texture1,texture2;
    glGenTextures(1, &texture1); // ID引用
    glBindTexture(GL_TEXTURE_2D, texture1); // 绑定该纹理
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // 加载并生成纹理1
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // OpenGL和图片定义的y轴是反的，所以我们这里需要翻个方向
    unsigned char *data = stbi_load("./images/container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); // 纹理目标、多级渐远纹理级别、存储格式、长宽、0...
        glGenerateMipmap(GL_TEXTURE_2D); // 为当前绑定的纹理对象附加上纹理图像
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    // 加载并生成纹理2
    glGenTextures(1, &texture2); // ID引用
    glBindTexture(GL_TEXTURE_2D, texture2); // 绑定该纹理
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    data = stbi_load("./images/awesomeface.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); // 这里使用GL_RGBA，多一个透明度
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // 定义哪个uniform采样器对应哪个纹理单元
    ourShader.use(); // 不要忘记在设置uniform变量之前激活着色器程序！
    // 手动设置
    glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
    // 或者使用着色器类设置
    ourShader.setInt("texture2", 1);

    // 对 glVertexAttribPointer 的调用将 VBO 注册为顶点属性绑定的顶点缓冲对象，因此可以安全地取消绑定
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    // 通常情况不会取消绑定 VAO（还有 VBO）
    glBindVertexArray(0); //释放图像内存


    // 渲染循环
    while(!glfwWindowShouldClose(window))
    {
        // 输入
        processInput(window); // 按键控制，Esc退出程序

        // 渲染
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 在新渲染前需要清屏
        glClear(GL_COLOR_BUFFER_BIT);

        // 绑定多个纹理
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // 设置混合度
        ourShader.setFloat("mixValue", mixValue);

        // 生成变换
        glm::mat4 transform = glm::mat4(1.0f); 
        transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f)); // 旋转，角度随时间变化
        transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f)); // 位移
        // 变换2
        glm::mat4 transform2 = glm::mat4(1.0f); 
        transform2 = glm::translate(transform2, glm::vec3(-0.5f, 0.5f, 0.0f)); // 位移
        transform2 = glm::scale(transform2, glm::vec3(abs(sin(glfwGetTime())) , abs(sin(glfwGetTime())), abs(sin(glfwGetTime())))); // 缩放
        

        // 画一个三角形
        ourShader.use(); // 激活着色程序

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

        glBindVertexArray(VAO); // 目前只有一个VAO，不需要每次都绑定
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//绘制模式、顶点个数、索引类型、EBO偏移量
        //glDrawArrays(GL_TRIANGLES, 0, 3); // 第一个参数表示绘制类型（三角形），0表示起始索引，3表示顶点数

        //第二个图形
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform2));
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        // 检查并调用事件，交换缓冲
        glfwSwapBuffers(window);// 交换颜色缓冲，用来绘制
        glfwPollEvents();// 检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数    
    }

    //释放资源
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(ourShader.ID);


    glfwTerminate(); // 渲染结束后释放资源
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height); // 设置视口，即渲染窗口的尺寸，前两个参数控制窗口左下角的位置。第三个和第四个参数控制渲染窗口的宽度和高度
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // 退出键判断
        glfwSetWindowShouldClose(window, true);

     if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        mixValue += 0.002f; // 增加二纹理透明度，该值为变化速度
        if(mixValue >= 1.0f)
            mixValue = 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        mixValue -= 0.002f; // 减少二纹理透明度
        if (mixValue <= 0.0f)
            mixValue = 0.0f;
    }
}