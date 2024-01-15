#include "camera.h"
#include <algorithm>
//카메라 회전, 이동 함수 작성하기


Camera::Camera()
{
	Pos = glm::vec3(0.0f, 5.0f, 0.0f); // 카메라 위치

	// 초기 카메라 응시하는 점 
	// 방향  아님, lookat함수에는 방향이 아니라 바라보는 점의 좌표를 인자로 함
	Direction = glm::vec3(0.0f, 0.0f, 0.0f); 

	Up = glm::vec3(0.0f, 1.0f, 0.0f);

	Distance = 5.0f;
}

void Camera::update(Player p, bool view_mode)
{
	update_pos(p, view_mode);
	update_dir(p, view_mode);
}

void Camera::update_pos(Player p, bool view_mode)
{
	if (view_mode) // 1인칭 시점
	{
		Pos = p.Get_pos();
		Pos.y += 1.0f;
	}
	else if (view_mode == false) // 3인칭 시점
	{
		Pos = p.Get_pos() - (p.Get_dir() * Distance);
		Pos.y = p.Get_pos().y + 3.0f;
	}
	
}
void Camera::update_dir(Player p, bool view_mode)
{
	if (view_mode) // 1인칭 시점
	{
		Direction = Pos + p.Get_dir();
	}
	else if (view_mode == false) // 3인칭 시점
	{
		Direction = p.Get_pos();

	}
}

void Camera::zoom(char key)
{
	switch (key)
	{
	case '+':
		Distance -= 0.5f;
		break;
	case '-':
		Distance += 0.5f;
		break;
	default:
		break;
	}

	Distance = std::clamp(Distance, 2.0f, 10.0f);
}

void Camera::Apply(int viewLocation)
{
	//std::cout << "camera pos: " << Pos.x << ", " << Pos.y << ", " << Pos.z << std::endl;
	glm::mat4 view = glm::lookAt(Pos, Direction, Up);
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
}


/*
void Camera::update_angle(int wmx, int wmy)
{
	float nx, ny;
	windowToNDC(wmx, wmy, nx, ny);
	mouse_x = nx; 
	mouse_y = ny;

	float dx = std::abs(mouse_x - mouse_last_x);
	float dy = std::abs(mouse_y - mouse_last_y);

	if (std::abs(last_dx) > epsilon) 
	{
		if (mouse_x > mouse_last_x) 
		{
			glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-camera_spin_speed), Up);
			glm::vec4 rotatedDirection = rotationMatrix * glm::vec4(Direction, 0.0f);
			Direction = glm::vec3(rotatedDirection);
		}
		else if (mouse_x < mouse_last_x) 
		{
			glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(camera_spin_speed), Up);
			glm::vec4 rotatedDirection = rotationMatrix * glm::vec4(Direction, 0.0f);
			Direction = glm::vec3(rotatedDirection);
		}
	}

	if (std::abs(last_dy) > epsilon)
	{
		if (mouse_y > mouse_last_y) // 마우스를 위로 이동
		{
			if (Direction.y < 3.0f) {
				glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(camera_spin_speed), glm::normalize(glm::cross(Direction, Up)));
				glm::vec4 rotatedDirection = rotationMatrix * glm::vec4(Direction, 0.0f);
				Direction = glm::vec3(rotatedDirection);
			}
		}
		else if (mouse_y < mouse_last_y)// 마우스를 아래로 이동
		{
			if (Direction.y > -3.0f) 
			{
				glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-camera_spin_speed), glm::normalize(glm::cross(Direction, Up)));
				glm::vec4 rotatedDirection = rotationMatrix * glm::vec4(Direction, 0.0f);
				Direction = glm::vec3(rotatedDirection);
			}
		}

	}

	mouse_last_x = mouse_x;
	mouse_last_y = mouse_y;

	last_dx = dx;
	last_dy = dy;

	if (Mouse_area_out(mouse_x, mouse_y)) //마우스가 화면 중앙 영역에서 나갔을 경우에, dx를 0으로 초기화하고 화면 중앙으로 마우스 이동
	{
		//std::cout << "Mouse_Reset" << std::endl;
		last_dx = 0;
		last_dy = 0;
		glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);

	}
}
*/
