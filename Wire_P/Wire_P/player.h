#pragma once
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include "camera.h"

class Camera;
const float epsilon = 1e-6;

struct Mouse
{
	float x_point;
	float y_point;
	float last_x_point;
	float last_y_point;

	float last_dx;
	float last_dy;
	float angle_spin_speed;
};

class Player
{
private:
	bool P_front;
	bool P_back;
	bool P_left;
	bool P_right;
	
	glm::vec3 pos;

	float angle;
	glm::vec3 direction;

	float speed;

public:

	Mouse mouse;

	void Set_Move(unsigned char key, int state);
	void Set_Direction(int x, int y);

	void change_direction(char n);
	void Move();
	glm::vec3 Get_pos();
	glm::vec3 Get_dir();
	float Get_angle();

	Player();
};