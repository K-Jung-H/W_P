#pragma once
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>


struct Camera
{
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f); // 카메라 위치
	glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, -1.0f); // 초기 카메라 바라보는 방향
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);



};