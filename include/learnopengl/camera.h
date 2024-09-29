#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// ��������ͷ����
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// ����ͷ�йص�Ĭ��ֵ
const float YAW         = -90.0f; // ƫ����
const float PITCH       =  0.0f;  // ������
const float SPEED       =  2.5f;  // ����ͷ�ƶ��ٶ�
const float SENSITIVITY =  0.1f;  // ���������
const float ZOOM        =  45.0f; // ���Ŵ�С


class Camera
{
public:
    // ���������
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // ŷ����
    float Yaw;
    float Pitch;
    // ����ͷ�䶯����
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // ��������ʼ��
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
    // ����ֵ��ʼ��
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    // ���ع۲���󣨸��ݷ�λ������������
    glm::mat4 GetViewMatrix();

    // ����������롣���������������ʽΪ����������ö�٣����ڽ���Ӵ���ϵͳ�г����������
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);

    // ��������ƶ�������
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

    // ���������������룬��������
    void ProcessMouseScroll(float yoffset);

private:
    // ��������ͷ������������
    void updateCameraVectors();
};
#endif