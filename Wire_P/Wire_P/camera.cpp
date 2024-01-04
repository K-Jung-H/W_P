#include "camera.h"
//카메라 회전, 이동 함수 작성하기

void action_camera(Camera& c, unsigned char key)
{
	switch (key)
	{
	case 'w':
		c.Pos.z += 0.5f;
		c.Direction.z += 0.5f;
		break;

	case 'a':
		c.Pos.x -= 0.5f;
		c.Direction.x -= 0.5f;
		break;

	case 's':
		c.Pos.z -= 0.5f;
		c.Direction.z -= 0.5f;
		break;

	case 'd':
		c.Pos.x += 0.5f;
		c.Direction.x += 0.5f;
		break;

	case 'x':
		c.Pos.y += 0.5f;
		c.Direction.y += 0.5f;
		break;
	default:
		break;
	}
}


void setting_camera(const Camera c, int viewLocation)
{
	glm::mat4 view = glm::lookAt(c.Pos, c.Direction, c.Up);
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
}