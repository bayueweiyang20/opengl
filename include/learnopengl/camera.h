#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// 定义摄像头方向
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// 摄像头有关的默认值
const float YAW         = -90.0f; // 偏航角
const float PITCH       =  0.0f;  // 俯仰角
const float SPEED       =  2.5f;  // 摄像头移动速度
const float SENSITIVITY =  0.1f;  // 鼠标灵敏度
const float ZOOM        =  45.0f; // 缩放大小


class Camera
{
public:
    // 摄像机属性
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // 欧拉角
    float Yaw;
    float Pitch;
    // 摄像头变动参数
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // 用向量初始化
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
    // 用数值初始化
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    // 返回观察矩阵（根据方位和向上向量）
    glm::mat4 GetViewMatrix();

    // 处理键盘输入。接受输入参数，形式为由相机定义的枚举（用于将其从窗口系统中抽象出来）。
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);

    // 处理鼠标移动的输入
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

    // 处理鼠标滚动的输入，控制缩放
    void ProcessMouseScroll(float yoffset);

private:
    // 更新摄像头三个方向向量
    void updateCameraVectors();
};
#endif