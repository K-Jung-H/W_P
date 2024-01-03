#define STB_IMAGE_IMPLEMENTATION
#pragma warning(disable:4996)
#include <iostream>

#include "stb_image.h"
#include "object.h"
#include "shader.h"


#define PI 3.141592

static float X_angles = 0;
static float Y_angles = 0;
static float Z_angles = 0;
static bool CW = false;

Cube cube;
Cube emolga;
void Init()
{
	//Emolga
	BuildOBJ("Emolga.obj", &emolga);
	BuildOBJ("cube.obj", &cube);
	
}

//카메라 구조체 만들기
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f); // 카메라 위치
glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, -1.0f);      // 초기 카메라 바라보는 방향
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);


//조명 관리 함수들 만들기

void drawScene()
{
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glUseProgram(shaderProgramID);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	int lightPosLocation = glGetUniformLocation(shaderProgramID, "lightPos");
	glUniform3f(lightPosLocation, 0.0f, 0.0f, 0.0f);
	int lightColorLocation = glGetUniformLocation(shaderProgramID, "lightColor"); //--- lightColor 값 전달: (1.0, 1.0, 1.0) 백색
	glUniform3f(lightColorLocation, 1.0f, 1.0f, 1.0f);

	int objColorLocation = glGetUniformLocation(shaderProgramID, "objectColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
	//int objviewLocation = glGetUniformLocation(shaderProgramID, "viewPos");
	int ambientLightLocation = glGetUniformLocation(shaderProgramID, "ambientLight");
	glUniform3f(ambientLightLocation, 1.0f, 1.0f, 1.0f);
	

	unsigned int viewLocation = glGetUniformLocation(shaderProgramID, "view"); //--- 뷰잉 변환 설정
	unsigned int Model_Transform = glGetUniformLocation(shaderProgramID, "model"); // 안 움직이는 부분


	glm::mat4 view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);


	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), (float)600 / (float)600, 0.1f, 200.0f);
	unsigned int projectionLocation = glGetUniformLocation(shaderProgramID, "projection");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);


	glm::mat4 transfrom = glm::mat4(1.0f);
	transfrom = glm::rotate(transfrom, glm::radians(X_angles), glm::vec3(1.0, 0.0, 0.0));
	transfrom = glm::rotate(transfrom, glm::radians(Z_angles), glm::vec3(0.0, 1.0, 0.0));
	transfrom = glm::rotate(transfrom, glm::radians(Y_angles), glm::vec3(0.0, 0.0, 1.0));
	glUniformMatrix4fv(Model_Transform, 1, GL_FALSE, glm::value_ptr(transfrom));


	glUniform3f(objColorLocation, 0.0f, 0.0f, 1.0f);



	glBindVertexArray(cube.VAO);
	glDrawArrays(GL_TRIANGLES, 0, cube.vertexData.size());
	glBindVertexArray(0);

	glUniform3f(objColorLocation, 0.0f, 1.0f, 1.0f);
	glBindVertexArray(emolga.VAO);
	glDrawArrays(GL_TRIANGLES, 0, emolga.vertexData.size());
	glBindVertexArray(0);
	
	glutSwapBuffers();
}


GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '1':
		X_angles += 2.5f;
		break;
	case '2':
		Y_angles += 2.5f;
		break;
	case '3':
		Z_angles += 2.5f;
		break;

	case 'w':
		cameraPos.z += 0.5f;
		break;
	case 'a':
		break;
	case 's':
		cameraPos.z -= 0.5f;
		break;
	case 'd':
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

