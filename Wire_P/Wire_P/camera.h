#pragma once
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>


struct Camera
{
	glm::vec3 Pos = glm::vec3(0.0f, 0.0f, 0.0f); // 카메라 위치
	glm::vec3 Direction = glm::vec3(0.0f, 0.0f, 1.0f); // 초기 카메라 바라보는 방향
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);


};


void action_camera(Camera& c, unsigned char key);
void setting_camera(Camera c, int viewLocation);