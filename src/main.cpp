#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

//�ص���������ÿ�δ��ڴ�С��������ʱ�򱻵��ã�ʹ�ӿڴ�С���洰�ڴ�С�仯
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//������ƣ��������˷��ؼ�(Esc)���������´�ѭ���ر�
void processInput(GLFWwindow *window);

//���ó���
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

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

    //��Ⱦѭ��
    while(!glfwWindowShouldClose(window))
    {
        //����
        processInput(window);//�������ƣ�Esc�˳�����

        //��Ⱦ
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//������Ⱦǰ��Ҫ����
        glClear(GL_COLOR_BUFFER_BIT);

        //��鲢�����¼�����������
        glfwSwapBuffers(window);//������ɫ���壬��������
        glfwPollEvents();//�����û�д���ʲô�¼�������������롢����ƶ��ȣ������´���״̬�������ö�Ӧ�Ļص�����    
    }

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