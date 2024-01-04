#define STB_IMAGE_IMPLEMENTATION
#pragma warning(disable:4996)
#include <iostream>

#include "stb_image.h"
#include "object.h"
#include "shader.h"
#include "camera.h"


#define PI 3.141592


static bool CW = false;

Camera camera;
Object cube;
Object emolga;

void Init()
{
	//Emolga
	BuildOBJ("resource/Emolga.obj", &emolga);
	BuildOBJ("resource/cube.obj", &cube);
	
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
	

	setting_camera(camera, viewLocation);

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), (float)1 / (float)1, 0.1f, 200.0f);
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, emolga.texture[0]);

	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, emolga.texture[1]);


	glm::mat4 transfrom = glm::mat4(1.0f);
	DrawOBJ(emolga, Model_Transform, objColorLocation, transfrom);
	//DrawOBJ(cube, Model_Transform, objColorLocation, transfrom);

	glutSwapBuffers();
}


GLvoid Keyboard(unsigned char key, int x, int y)
{

		action_camera(camera, key);
	switch (key)
	{
	case '+':
		emolga.temp += 1;
		break;

	case '-':
		emolga.temp -= 1;
		break;

	case '1':
		emolga.X_angle += 2.5f;
		break;

	case '2':
		emolga.Y_angle += 2.5f;
		break;

	case '3':
		emolga.Z_angle += 2.5f;
		break;

	case '4':
		Check_Data(cube);
		break;

	case 'q':
		exit(1);
		break;

	case 'z':
		CW = !CW;
		if (CW)
			glFrontFace(GL_CW);
		else
			glFrontFace(GL_CCW);
		break;

	default:
		break;

	}

	glutPostRedisplay();


}
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(600, 600);
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


	glutMainLoop();

}

