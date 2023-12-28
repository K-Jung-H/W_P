#define STB_IMAGE_IMPLEMENTATION
#pragma warning(disable:4996)
#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include "stb_image.h"
#define PI 3.141592 

float vertices[] = { //--- 버텍스 속성: 좌표값(FragPos), 노말값 (Normal)
-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f,0.0f,		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,0.0f,0.0f,		 0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,0.0f,1.0f, //뒷면
0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,0.0f,1.0f,		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,1.0f,1.0f,		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,1.0f,0.0f,

-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,0.0f,0.0f,		0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,1.0f,0.0f,		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,1.0f,1.0f, //앞면
0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,	1.0f,1.0f,		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,0.0f,1.0f,		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,0.0f,0.0f,

-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,1.0f,1.0f,		 -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,0.0f,1.0f,	 -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,0.0f,0.0f, // 왼쪽면
-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,0.0f,0.0f,	 -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,1.0f,0.0f,			 -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,1.0f,1.0f,

0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,0.0f,1.0f,			 0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,1.0f,1.0f,			0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,1.0f,0.0f, //오른쪽면
0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,1.0f,0.0f,    	0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f,0.0f,		     0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,0.0f,1.0f,

-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,0.0f,0.0f,		0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,1.0f,0.0f,		 0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,1.0f,1.0f, // 바닥
0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,1.0f,1.0f,	  	 -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,0.0f,1.0f,			-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,0.0f,0.0f,

-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,0.0f,1.0f,		 0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,1.0f,1.0f,	   0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,1.0f,0.0f,// 위
0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,	1.0f,0.0f,		 -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,0.0f,0.0f,		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,0.0f,1.0f
};

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

unsigned int texture[6];
int widthImage, heightImage, numberOfChannel;
void InitTexture()
{
	glGenTextures(6, texture);

	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("spider.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	int tLocation = glGetUniformLocation(shaderProgramID, "texture1");
	glUniform1i(tLocation, 0);
	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("spider.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	tLocation = glGetUniformLocation(shaderProgramID, "texture1");
	glUniform1i(tLocation, 0);
	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("spider.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, widthImage, heightImage, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	tLocation = glGetUniformLocation(shaderProgramID, "texture1");
	glUniform1i(tLocation, 0);
	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("spider.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, widthImage, heightImage, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	tLocation = glGetUniformLocation(shaderProgramID, "texture1");
	glUniform1i(tLocation, 0);
	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("spider.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, widthImage, heightImage, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	tLocation = glGetUniformLocation(shaderProgramID, "texture1");
	glUniform1i(tLocation, 0);
	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, texture[5]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("spider.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, widthImage, heightImage, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	tLocation = glGetUniformLocation(shaderProgramID, "texture1");
	glUniform1i(tLocation, 0);
	stbi_image_free(data);
}

void ReadObj()
{

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); //--- 위치 속성
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); //--- 노말 속성
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); //--- 텍스처 좌표 속성
	glEnableVertexAttribArray(2);


	glUseProgram(shaderProgramID);
}



void drawScene()
{
	int lightColorLocation = glGetUniformLocation(shaderProgramID, "lightColor"); //--- lightColor 값 전달: (1.0, 1.0, 1.0) 백색
	int objColorLocation = glGetUniformLocation(shaderProgramID, "objectColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
	int objviewLocation = glGetUniformLocation(shaderProgramID, "viewPos");

	glutSwapBuffers();
}


GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{

	case 'w':
		break;
	case 'a':
		break;
	case 's':
		break;
	case 'd':
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
	glutCreateWindow("Example14");

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	InitTexture();
	make_vertexShaders();
	make_fragmentShaders();
	make_shaderProgram();


	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);
	glutDisplayFunc(drawScene);

	glutKeyboardFunc(Keyboard);


	glutMainLoop();

}

