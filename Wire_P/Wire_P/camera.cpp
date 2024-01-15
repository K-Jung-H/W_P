#include "camera.h"
#include <algorithm>
//ī�޶� ȸ��, �̵� �Լ� �ۼ��ϱ�


Camera::Camera()
{
	Pos = glm::vec3(0.0f, 5.0f, 0.0f); // ī�޶� ��ġ

	// �ʱ� ī�޶� �����ϴ� �� 
	// ����  �ƴ�, lookat�Լ����� ������ �ƴ϶� �ٶ󺸴� ���� ��ǥ�� ���ڷ� ��
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
	if (view_mode) // 1��Ī ����
	{
		Pos = p.Get_pos();
		Pos.y += 1.0f;
	}
	else if (view_mode == false) // 3��Ī ����
	{
		Pos = p.Get_pos() - (p.Get_dir() * Distance);
		Pos.y = p.Get_pos().y + 3.0f;
	}
	
}
void Camera::update_dir(Player p, bool view_mode)
{
	if (view_mode) // 1��Ī ����
	{
		Direction = Pos + p.Get_dir();
	}
	else if (view_mode == false) // 3��Ī ����
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
		if (mouse_y > mouse_last_y) // ���콺�� ���� �̵�
		{
			if (Direction.y < 3.0f) {
				glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(camera_spin_speed), glm::normalize(glm::cross(Direction, Up)));
				glm::vec4 rotatedDirection = rotationMatrix * glm::vec4(Direction, 0.0f);
				Direction = glm::vec3(rotatedDirection);
			}
		}
		else if (mouse_y < mouse_last_y)// ���콺�� �Ʒ��� �̵�
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

	if (Mouse_area_out(mouse_x, mouse_y)) //���콺�� ȭ�� �߾� �������� ������ ��쿡, dx�� 0���� �ʱ�ȭ�ϰ� ȭ�� �߾����� ���콺 �̵�
	{
		//std::cout << "Mouse_Reset" << std::endl;
		last_dx = 0;
		last_dy = 0;
		glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);

	}
}
*/
