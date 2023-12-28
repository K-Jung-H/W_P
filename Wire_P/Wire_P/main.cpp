#define STB_IMAGE_IMPLEMENTATION
#pragma warning(disable:4996)
#include <iostream>

#include "stb_image.h"
#include "object.h"


#define PI 3.141592



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
		glGenVertexArrays(1, &cube.Vao);
		glGenBuffers(1, &cube.Vbo_p);
		glGenBuffers(1, &cube.Vbo_n);
		glGenBuffers(1, &cube.Vbo_vt);
		glBindVertexArray(cube.Vao);

		// 버텍스 데이터 전달
		glBindBuffer(GL_ARRAY_BUFFER, cube.Vbo_p);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * cube.vertices.size(), cube.vertices.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //--- 위치 속성
		glEnableVertexAttribArray(0);

		// 노멀 데이터 전달
		glBindBuffer(GL_ARRAY_BUFFER, cube.Vbo_n);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Normal) * cube.normals.size(), cube.normals.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //--- 노말 속성
		glEnableVertexAttribArray(1);


		// 텍스처 좌표 속성
		glBindBuffer(GL_ARRAY_BUFFER, cube.Vbo_vt);
		glBufferData(GL_ARRAY_BUFFER, sizeof(TexCoord) * cube.texCoords.size(), cube.texCoords.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0); //--- 텍스처 좌표 속성
		glEnableVertexAttribArray(2);
	}
}

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f); // 카메라 위치
glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, -1.0f);      // 초기 카메라 바라보는 방향
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

void drawScene()
{

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glUseProgram(shaderProgramID);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	int lightColorLocation = glGetUniformLocation(shaderProgramID, "lightColor"); //--- lightColor 값 전달: (1.0, 1.0, 1.0) 백색
	int objColorLocation = glGetUniformLocation(shaderProgramID, "objectColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
	int objviewLocation = glGetUniformLocation(shaderProgramID, "viewPos");
	int ambientLightLocation = glGetUniformLocation(shaderProgramID, "ambientLight");
	glUniform3f(ambientLightLocation, 1.0, 1.0, 1.0);


	unsigned int viewLocation = glGetUniformLocation(shaderProgramID, "view"); //--- 뷰잉 변환 설정
	unsigned int Model_Transform = glGetUniformLocation(shaderProgramID, "model"); // 안 움직이는 부분


	glm::mat4 view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);


	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), (float)600 / (float)600, 0.1f, 200.0f);
	unsigned int projectionLocation = glGetUniformLocation(shaderProgramID, "projection");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);


	glm::mat4 transfrom = glm::mat4(1.0f);
	glUniformMatrix4fv(Model_Transform, 1, GL_FALSE, glm::value_ptr(transfrom));




	glBindVertexArray(cube.Vao);
	glUniform3f(objColorLocation, 1.0, 0.0, 0.0);
	glDrawArrays(GL_TRIANGLES, 0, 36);

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

	Init();
	make_vertexShaders();
	make_fragmentShaders();
	make_shaderProgram();


	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);
	glutDisplayFunc(drawScene);

	glutKeyboardFunc(Keyboard);


	glutMainLoop();

}

