#define STB_IMAGE_IMPLEMENTATION
#pragma warning(disable:4996)
#include <iostream>

#include "stb_image.h"
#include "object.h"
#include "shader.h"
#include "camera.h"
#include "player.h"


#define PI 3.141592
static bool First_person_view = false;
static bool Top_view = false;
static bool CW = false;
static bool Timer_Control = false;
Player player;
Camera camera;
Camera top_cam;
Object cube, sphere;
Background background;
Emolga emolga;

std::vector<Object*> tst;

void Init()
{
//	background.Build("resource/cube.obj");
//	background.LoadTexture();
	//Emolga
	emolga.Build("resource/Emolga.obj");
	emolga.LoadTexture();

	cube.Build("resource/cube.obj");
	cube.LoadTexture();
	
	sphere.Build("resource/sphere.obj");
	sphere.LoadTexture();


	tst.push_back(&emolga);
	tst.push_back(&cube);
	tst.push_back(&sphere);

//	tst.push_back(&background);
}

//카메라 구조체 만들기


//조명 관리 함수들 만들기

void drawScene()
{

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glUseProgram(shaderProgramID);
	glClearColor(0.5f, 0.5f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	int Texture_mode_Location = glGetUniformLocation(shaderProgramID, "Use_Texture_Mode"); // true => 텍스쳐 O //false => 텍스쳐 X
	
	int lightPosLocation = glGetUniformLocation(shaderProgramID, "lightPos");
	int lightColorLocation = glGetUniformLocation(shaderProgramID, "lightColor"); //--- lightColor 값 전달: (1.0, 1.0, 1.0) 백색
	int objColorLocation = glGetUniformLocation(shaderProgramID, "objectColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
	int ambientLightLocation = glGetUniformLocation(shaderProgramID, "ambientLight");
	int viewLocation = glGetUniformLocation(shaderProgramID, "view"); //--- 뷰잉 변환 설정
	int Model_Transform = glGetUniformLocation(shaderProgramID, "model"); // 안 움직이는 부분
	int projectionLocation = glGetUniformLocation(shaderProgramID, "projection");


	glUniform3f(lightPosLocation, 0.0f, 0.0f, 0.0f);
	glUniform3f(lightColorLocation, 1.0f, 1.0f, 1.0f);
	glUniform3f(ambientLightLocation, 1.0f, 1.0f, 1.0f);
	
	if (Top_view) // 이부분 함수로 간단화 하기
	{

	top_cam.Direction = { 0.0f,0.0f,0.0f };
	top_cam.Pos = { 0.0f, 50.0f, 0.0f };
	top_cam.Up = { 0.0f, 0.0f, -1.0f };
	top_cam.Apply(viewLocation); // 카메라 위치변화 업데이트
	}
	else if (First_person_view)
	{
		camera.Apply(viewLocation); // 카메라 위치변화 업데이트
	}
	else if (First_person_view == false)
	{
		camera.Apply(viewLocation); // 카메라 위치변화 업데이트

	}



	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), (float)1 / (float)1, 0.1f, 200.0f);
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
	


	glm::mat4 transfrom = glm::mat4(1.0f);


	for (Object* object : tst) // 도형 전체 그리기
	{
		object->Draw(Model_Transform, objColorLocation, player);
	}

	if (First_person_view)
	{
	cube.Draw(Model_Transform, objColorLocation, camera);

	}
	glutSwapBuffers();
}

void Timer(int value)
{
	//update 함수 만들어서 월드 전체를 업데이트 하기

	emolga.X_angle += 5.0f;
	emolga.Y_angle += 5.0f;
	emolga.Z_angle += 5.0f;
	sphere.Y_angle += 5.0f;
	glutPostRedisplay();
	if (Timer_Control)
	{
		glutTimerFunc(60, Timer, 1);

	}

}

void Player_Timer(int value)
{
	player.Move();
	camera.update(player, First_person_view);


	glutPostRedisplay();
	glutTimerFunc(60, Player_Timer, 1);
	
}

GLvoid Keyboard(unsigned char key, int x, int y)
{


	switch (key)
	{
	case 'w': 	case 'W':
	case 's': 	case 'S':
	case 'a':	case 'A':
	case 'd':	case 'D':
		player.Set_Move(key, 0);
		break;


	case '1':
		Timer_Control = !Timer_Control;
		glutTimerFunc(60, Timer, 1);
		break;

	case 'c':	case 'C':
		First_person_view = !First_person_view;
		break;

	case 'T':	case 't':
		Top_view = !Top_view;
		break;


	case '+':
	case '-':
		camera.zoom(key);
		break;


	case 'z':
		CW = !CW;
		if (CW)
			glFrontFace(GL_CW);
		else
			glFrontFace(GL_CCW);
		break;

	case 'q':
		exit(1);
		break;

	case '3': case '4':
		player.Set_Move(key, 0);
		break;

	default:
		break;

	}

	glutPostRedisplay();


}
GLvoid Up_Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w': 	case 'W':
	case 's': 	case 'S':
	case 'a':	case 'A':
	case 'd':	case 'D':
		player.Set_Move(key, 1);
		break;

	}
	glutPostRedisplay();
}

GLvoid Special_Keyboard(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_RIGHT:
		player.change_direction('R');
		break;
	case GLUT_KEY_LEFT:
		player.change_direction('L');
		break;

	default:
		break;
	}

	glutPostRedisplay();

}

void Mouse_Move(int wmx, int wmy)
{
	//player.Set_Direction(wmx, wmy);
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("Wire_plz");

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	Init();
	make_vertexShaders();
	make_fragmentShaders();
	make_shaderProgram();


	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(Keyboard);
	glutKeyboardUpFunc(Up_Keyboard);
	glutSpecialFunc(Special_Keyboard);


	glutPassiveMotionFunc(Mouse_Move);

	glutTimerFunc(60, Player_Timer, 1);
	glutMainLoop();

}

