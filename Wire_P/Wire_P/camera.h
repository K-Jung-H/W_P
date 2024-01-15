#pragma once
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <algorithm>
#include "player.h"
#define windowWidth 1000
#define windowHeight 600
 
class Player;

class Camera
{
public:
	
	glm::vec3 Pos;
	glm::vec3 Direction;
	glm::vec3 Up;
	float Distance;

	Camera();


	void Apply(int viewLocation);
	void update(Player, bool);
	void update_pos(Player, bool);
	void update_dir(Player, bool);
	void zoom(char);
};
