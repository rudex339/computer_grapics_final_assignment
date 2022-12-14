// Reading Obj file
#define  _CRT_SECURE_NO_WARNINGS

#include "shader.h"
#include "computer_grapics_tool.h"
#include "unit.h"
#include "background.h"
#include <random>
#include <time.h>


using namespace std;

#define PI 3.14

GLuint g_window_w = 800;
GLuint g_window_h = 600;

void Display();
void Reshape(int w, int h);
void Keyboard(unsigned char key, int x, int y);
void spckeycallback(int key, int x, int y);
GLvoid TimerFunction(int value);
void InitBuffer();
bool collide(glm::vec3 o1, glm::vec3 o2) {
	if ((o1.x - o2.x) * (o1.x - o2.x) +
		(o1.y - o2.y) * (o1.y - o2.y)
		< (o1.z + o2.z) * (o1.z + o2.z))
		return true;
	return false;
}
enum State {
	play,
	result,
	gameover
};
class World {
private:
	bool change_proj;
	glm::vec3 cameraPos;
	glm::vec3 cameraTarget;
	glm::vec3 up;

	Ground ground;
	Tank object;
	int x_num, y_num;
	float turn_camera;
	float rady_camera;

	int point;

	State state;
public:
	World() {
		change_proj = true;
		cameraPos = glm::vec3(60, 30, 0);
		cameraTarget = glm::vec3(0, 0, 0);
		up = glm::vec3(0.0f, 1.0f, 0.0f);
		x_num = 10; y_num = 10;

		turn_camera = 0.0f;
		rady_camera = 0.0f;
		
		state = play;
		point = 0;
	}
	void init_buffer() {
		object.init();
		ground.init();
		unsigned int lightPosLocation = glGetUniformLocation(s_program[0], "lightPos"); //--- lightPos 값 전달: (0.0, 0.0, 5.0);
		glUniform3f(lightPosLocation, 0.0, 40.0, 10.0);
		unsigned int lightColorLocation = glGetUniformLocation(s_program[0], "lightColor"); //--- lightColor 값 전달: (1.0, 1.0, 1.0) 백색
		glUniform3f(lightColorLocation, 1.0, 1.0, 1.0);
		
	}

	void draw() {
		if (state == play) {
			glUseProgram(s_program[0]);

			int viewLoc = glGetUniformLocation(s_program[0], "viewTransform"); //--- 버텍스 세이더에서 뷰잉 변환 행렬 변수값을 받아온다.
			int projLoc = glGetUniformLocation(s_program[0], "projectionTransform"); //--- 버텍스 세이더에서 투영 변환 행렬 변수값을 받아온다.
			//--- 모델링 변환, 뷰잉 변환, 투영 변환 행렬을 설정한 후, 버텍스 세이더에 저장한다.
			glm::mat4 vTransform = glm::mat4(1.0f);
			//vTransform = glm::lookAt(cameraPos, cameraTarget, up);
			vTransform = object.GetViewMatrix(cameraPos);
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &vTransform[0][0]);
			glm::mat4 pTransform = glm::mat4(1.0f);
			pTransform = glm::perspective(glm::radians(60.0f), (float)g_window_w / (float)g_window_h, 0.1f, 200.0f);
			glUniformMatrix4fv(projLoc, 1, GL_FALSE, &pTransform[0][0]);
			object.draw();
			ground.draw();
		}
		else if (state == result) {
			cout << "gameover" << endl;
			cout << "point = " << point << endl;
		}
	}
	void update() {
		if (state == play) {
			object.update();

			if (object.get_pos().x > 45 || object.get_pos().x < -45 ||
				object.get_pos().y > 45 || object.get_pos().y < -45) {
				state = result;
			}
			if (collide(object.get_pos(), ground.get_pos_p())) {
				ground.collide_p();
				point++;
			}
		}
		else if (state == result) {
			state = gameover;
		}
	}
	void handle_key(char key) {//23
		if (state == play) {
			object.handle_key(key);
		}
		else if (state == gameover) {
			if (key == 'r') {
				reset();
			}
		}
	}
	void reset() {

	}
};

World wod;

int main(int argc, char** argv)
{
	// create window using freeglut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(g_window_w, g_window_h);
	glutInitWindowPosition(0, 0);

	glutCreateWindow("Computer Graphics");

	//////////////////////////////////////////////////////////////////////////////////////
	//// initialize GLEW
	//////////////////////////////////////////////////////////////////////////////////////
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
	{
		std::cout << "GLEW OK\n";
	}

	//////////////////////////////////////////////////////////////////////////////////////
	//// Create shader program an register the shader
	//////////////////////////////////////////////////////////////////////////////////////

	GLuint vShader[4];
	GLuint fShader[7];
	vShader[0] = MakeVertexShader("vertex.glsl", 0);			// Sun
	fShader[0] = MakeFragmentShader("fragment.glsl", 0);

	// shader Program
	s_program[0] = glCreateProgram();
	glAttachShader(s_program[0], vShader[0]);
	glAttachShader(s_program[0], fShader[0]);
	glLinkProgram(s_program[0]);
	checkCompileErrors(s_program[0], "PROGRAM");

	InitBuffer();

	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(spckeycallback);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glutTimerFunc(10, TimerFunction, 1);
	glutMainLoop();

	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);

	return 0;
}


void InitBuffer()
{
	wod.init_buffer();

	//// 5.1. VAO 객체 생성 및 바인딩
}


void Display()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	wod.draw();


	glutSwapBuffers();

}


void Reshape(int w, int h)
{
	g_window_w = w;
	g_window_h = h;
	glViewport(0, 0, w, h);
}

void Keyboard(unsigned char key, int x, int y)
{
	if (key == 'q') {
		glutLeaveMainLoop();
	}
	wod.handle_key(key);
	glutPostRedisplay();
}

void TimerFunction(int value)
{
	wod.update();

	glutPostRedisplay(); // 화면 재 출력
	glutTimerFunc(10, TimerFunction, 1);
}

void spckeycallback(int key, int x, int y) {


	glutPostRedisplay();
}