#define STB_IMAGE_IMPLEMENTATION
#pragma warning(disable:4996)
#include <iostream>

#include "stb_image.h"
#include "object.h"


#define PI 3.141592

static float R_angles = 0;

void make_vertexShaders();
void make_fragmentShaders();
char* filetobuf(const char* file);

GLchar* vertexSource, * fragmentSource; //--- 소스코드 저장 변수
GLuint vertexShader, fragmentShader; //--- 세이더 객체
GLuint shaderProgramID; //--- 셰이더 프로그램


void make_shaderProgram()
{
	make_vertexShaders(); //--- 버텍스 세이더 만들기
	make_fragmentShaders(); //--- 프래그먼트 세이더 만들기
	//-- shader Program
	shaderProgramID = glCreateProgram();
	glAttachShader(shaderProgramID, vertexShader);
	glAttachShader(shaderProgramID, fragmentShader);
	glLinkProgram(shaderProgramID);
	//--- 세이더 삭제하기
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//--- Shader Program 사용하기
	glUseProgram(shaderProgramID);
}
void make_vertexShaders()
{
	vertexSource = filetobuf("vertex.glsl");
	//--- 버텍스 세이더 객체 만들기
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, 0);
	//--- 버텍스 세이더 컴파일하기
	glCompileShader(vertexShader);
	//--- 컴파일이 제대로 되지 않은 경우: 에러 체크
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cout << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}
void make_fragmentShaders()
{
	fragmentSource = filetobuf("fragment.glsl");
	//--- 프래그먼트 세이더 객체 만들기
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, 0);
	//--- 프래그먼트 세이더 컴파일
	glCompileShader(fragmentShader);
	//--- 컴파일이 제대로 되지 않은 경우: 컴파일 에러 체크
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cout << "ERROR: fragment shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}
char* filetobuf(const char* file)
{
	FILE* fptr;
	long length;
	char* buf;
	fptr = fopen(file, "rb"); // Open file for reading 
	if (!fptr) // Return NULL on failure 
		return NULL;
	fseek(fptr, 0, SEEK_END); // Seek to the end of the file 
	length = ftell(fptr); // Find out how many bytes into the file we are 
	buf = (char*)malloc(length + 1); // Allocate a buffer for the entire length of the file and a null terminator 
	fseek(fptr, 0, SEEK_SET); // Go back to the beginning of the file 
	fread(buf, length, 1, fptr); // Read the contents of the file in to the buffer 
	fclose(fptr); // Close the file 
	buf[length] = 0; // Null terminator 
	return buf; // Return the buffer 
}

/*
	glTexImage2D(GL_TEXTURE_2D, 0, 3, widthImage, heightImage, 0, GL_RGB, GL_UNSIGNED_BYTE, data); // 없는 사진
	glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); // 알파값 있는 사진
*/


void Init()
{

	if (LoadOBJ("cube.obj", &cube))
	{
		// VAO 설정
		glGenVertexArrays(1, &cube.VAO);
		glBindVertexArray(cube.VAO);

		glGenBuffers(1, &cube.VBO);


		glBindBuffer(GL_ARRAY_BUFFER, cube.VBO); //하나의 버퍼에 모든 데이터 저장
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * cube.vertexData.size(), cube.vertexData.data(), GL_STATIC_DRAW);

		// 버텍스 속성 설정 -> 어떻게 저장 되어있는지
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, vertex));
		glEnableVertexAttribArray(0);

		// 노멀 속성 설정 -> 어떻게 저장 되어있는지
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, normal));
		glEnableVertexAttribArray(1);

		// 텍스처 좌표 속성 -> 어떻게 저장 되어있는지
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, texCoord));
		glEnableVertexAttribArray(2);

		// VAO 언바인드
		glBindVertexArray(0);
	}
}

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f); // 카메라 위치
glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, -1.0f);      // 초기 카메라 바라보는 방향
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

void drawScene()
{

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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
	transfrom = glm::rotate(transfrom, glm::radians(R_angles), glm::vec3(0.0, 1.0, 0.0));
	glUniformMatrix4fv(Model_Transform, 1, GL_FALSE, glm::value_ptr(transfrom));




	glBindVertexArray(cube.VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube.EBO);
	glDrawElements(GL_TRIANGLES, cube.vertexData.size(), GL_UNSIGNED_INT, 0);
//	glDrawArrays(GL_TRIANGLES, 0, cube.vertexData.size());
	glBindVertexArray(0);

	glutSwapBuffers();
}


GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{

	case 'w':
		cameraPos.z += 0.5f;
		break;
	case 'a':
		R_angles += 0.5f;
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
	glutInitWindowSize(1000, 600);
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

