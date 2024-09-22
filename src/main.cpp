#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

//回调函数，在每次窗口大小被调整的时候被调用，使视口大小跟随窗口大小变化
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//输入控制，若按下了返回键(Esc)，程序在下次循环关闭
void processInput(GLFWwindow *window);

//设置长宽
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

//顶点着色器与片元着色器
const char *vertexShaderSource = "#version 460 core\n"           //版本号
    "layout (location = 0) in vec3 aPos;\n"                      //位置变量的属性位置值为 0
    "layout (location = 1) in vec3 aColor;\n"                    //颜色变量的属性位置值为 1
    "out vec3 ourColor;\n"
    "uniform vec2 xy;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x + xy.x, aPos.y + xy.y, aPos.z, 1.0);\n"      //顶点着色器的输出
    "   ourColor = aColor;\n"                  //将ourColor设置为我们从顶点数据那里得到的输入颜色
    "}\0";
const char *fragmentShaderSource = "#version 460 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"                                     //全局变量，可在程序中设置
    "void main()\n"
    "{\n"
    "   FragColor = vec4(ourColor, 1.0);\n"             //RGB和透明度
    "}\n\0";

int main() {

    glfwInit();//初始化GLFW，为后续GLFW操作做全局设置
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);//主版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);//次版本号
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


    //创建我们的着色器
    //创建顶点着色器
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);//ID引用并提供类型为顶点着色
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);//着色器源码附加到着色器对象，1表示字符串数量
    glCompileShader(vertexShader);//编译源码
    //检查编译是否成功
    int success;
    char infoLog[512];//存储错误信息
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);//检查是否成功函数
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);//编译失败就获取错误信息
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;//输出错误信息
    }
    //创建片段着色器
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);//ID引用并提供类型为片段着色
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);//着色器源码附加到着色器对象，1表示字符串数量
    glCompileShader(fragmentShader);//编译源码
    //检查编译是否成功
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);//检查是否成功函数
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);//编译失败就获取错误信息
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;//输出错误信息
    }
    //链接着色器
    unsigned int shaderProgram = glCreateProgram();//创建一个程序对象
    glAttachShader(shaderProgram, vertexShader);//附加顶点着色器
    glAttachShader(shaderProgram, fragmentShader);//附加片段着色器
    glLinkProgram(shaderProgram);//链接
    //检测链接着色器程序是否成功
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    //链接完之后就不需要顶点着色器和片段着色器了
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //顶点输入
     float vertices[] = {
        // 位置              // 颜色
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
       -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
        0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
    };
    // unsigned int indices[] = {  // 注意索引从0开始!
    //     0, 1, 3,  // 第一个三角形
    //     1, 2, 3   // 第二个三角形
    // };

    //VAO存储顶点属性，VBO存储顶点数据
    unsigned int VBO, VAO,EBO;
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
        glUseProgram(shaderProgram);//激活着色程序

        double  timeValue = glfwGetTime();
        float xValue = static_cast<float>(sin(timeValue) / 2.0f);
        float yValue = static_cast<float>(cos(timeValue) / 2.0f);
        int vertexXYLocation = glGetUniformLocation(shaderProgram, "xy");
        glUniform2f(vertexXYLocation, xValue, yValue);

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
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);


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