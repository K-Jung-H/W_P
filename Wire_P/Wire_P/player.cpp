#include "player.h"

void windowToNDC(int mouseX, int mouseY, float& ndcX, float& ndcY)
{
	// 윈도우 좌표를 NDC 좌표로 변환
	ndcX = (2.0f * mouseX) / windowWidth - 1.0f;
	ndcY = 1.0f - (2.0f * mouseY) / windowHeight;
}


bool Mouse_area_out(float mx, float my)
{

	if (mx > 0.05f || mx < -0.05f)
	{
		return true;
	}
	if (my > 0.05f || my < -0.05f)
	{
		return true;
	}
	return false;
}


Player::Player()
{
	P_front = false;
	P_left = false;
	P_right = false;
	P_back = false;
	pos = { 0.0f, 0.0f, 0.0f };
	direction = { 0.0f, 0.0f, -1.0f };
	angle = 180.0f;
	speed = 1;

	mouse.x_point = 0;
	mouse.y_point = 0;
	mouse.last_x_point = 0;
	mouse.last_y_point = 0;

	mouse.last_dx = 0;
	mouse.last_dy = 0;
	mouse.angle_spin_speed = 2.0f;

}
glm::vec3 Player::Get_pos()
{
	return pos;
}
glm::vec3 Player::Get_dir()
{
	return direction;
}
void Player::Set_Move(unsigned char key, int state) // state: 0 = 키 다운, 1 = 키 업
{
	bool Key_State = (state == 0); // 0이면 true, 1이면 false
	switch (key)
	{
	case 'w':
		P_front = Key_State;
		break;

	case 'a':
		P_left = Key_State;
		break;

	case 's':
		P_back = Key_State;
		break;

	case 'd':
		P_right = Key_State;
		break;

	case '3':
		pos.y -= 0.5f;
		break;
	case '4':
		pos.y += 0.5f;
		break;


	default:
		break;
	}
}

void Player::Set_Direction(int wmx, int wmy)
{
	float nx, ny;
	windowToNDC(wmx, wmy, nx, ny);
	mouse.x_point = nx;
	mouse.y_point = ny;

	float dx = std::abs(mouse.x_point - mouse.last_x_point);
	float dy = std::abs(mouse.y_point - mouse.last_y_point);

	if (std::abs(mouse.last_dx) > epsilon)
	{
		if (mouse.x_point > mouse.last_x_point)
		{
			glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-mouse.angle_spin_speed), glm::vec3(0.0f, 1.0f, 0.0f));
			glm::vec4 rotatedDirection = rotationMatrix * glm::vec4(direction, 0.0f);
			direction = glm::vec3(rotatedDirection);
		}
		else if (mouse.x_point < mouse.last_x_point)
		{
			glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(mouse.angle_spin_speed), glm::vec3(0.0f, 1.0f, 0.0f));
			glm::vec4 rotatedDirection = rotationMatrix * glm::vec4(direction, 0.0f);
			direction = glm::vec3(rotatedDirection);
		}
	}

	if (std::abs(mouse.last_dy) > epsilon)
	{
		if (mouse.y_point > mouse.last_y_point) // 마우스를 위로 이동
		{
			if (direction.y < 3.0f) {
				glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(mouse.angle_spin_speed), glm::normalize(glm::cross(direction, glm::vec3(0.0f, 1.0f, 0.0f))));
				glm::vec4 rotatedDirection = rotationMatrix * glm::vec4(direction, 0.0f);
				direction = glm::vec3(rotatedDirection);
			}
		}
		else if (mouse.y_point < mouse.last_y_point)// 마우스를 아래로 이동
		{
			if (direction.y > -3.0f)
			{
				glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-mouse.angle_spin_speed), glm::normalize(glm::cross(direction, glm::vec3(0.0f, 1.0f, 0.0f))));
				glm::vec4 rotatedDirection = rotationMatrix * glm::vec4(direction, 0.0f);
				direction = glm::vec3(rotatedDirection);
			}
		}

	}

	mouse.last_x_point = mouse.x_point;
	mouse.last_y_point = mouse.y_point;

	mouse.last_dx = dx;
	mouse.last_dy = dy;

	if (Mouse_area_out(mouse.x_point, mouse.y_point)) //마우스가 화면 중앙 영역에서 나갔을 경우에, dx를 0으로 초기화하고 화면 중앙으로 마우스 이동
	{
		//std::cout << "Mouse_Reset" << std::endl;
		mouse.last_dx = 0;
		mouse.last_dy = 0;
		glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);

	}
}


void Player::Move()
{
	glm::vec3 Move_Direction = glm::normalize(glm::vec3(direction.x, 0.0f, direction.z));

	if (P_front) {
		pos.x += speed * Move_Direction.x;
		pos.z += speed * Move_Direction.z;
	}
	if (P_back) {
		pos.x -= speed * Move_Direction.x;
		pos.z -= speed * Move_Direction.z;
	}

	if (P_right) {
		glm::vec3 rightVec = glm::normalize(glm::cross(Move_Direction, glm::vec3(0.0f, 1.0f, 0.0f)));
		pos.x += speed * rightVec.x;
		pos.z += speed * rightVec.z;
	}
	if (P_left) {
		glm::vec3 leftVec = glm::normalize(glm::cross(Move_Direction, glm::vec3(0.0f, 1.0f, 0.0f)));
		pos.x -= speed * leftVec.x;
		pos.z -= speed * leftVec.z;
	}
	//std::cout << "player pos: " << pos.x << ", " << pos.y << ", " << pos.z << std::endl;

}
float Player::Get_angle()
{
	return angle;
}


void Player::change_direction(char n)
{
	if (n == 'L')
	{
		//angle += mouse.angle_spin_speed;
		angle = fmod(angle + mouse.angle_spin_speed + 360.0f, 360.0f);

		
	}
	else if (n == 'R')
	{
		//angle -= mouse.angle_spin_speed;
		angle = fmod(angle - mouse.angle_spin_speed + 360.0f, 360.0f);

	}


	//std::cout <<"angle: " << angle << std::endl;
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
	
	glm::vec4 rotatedDirection = rotationMatrix * glm::vec4(0.0f,0.0f,1.0f, 0.0f);
	direction = glm::vec3(rotatedDirection.x, rotatedDirection.y, rotatedDirection.z);

	std::cout<<"p: " << direction.x << ", " << direction.y << ", " << direction.z << std::endl;


}
