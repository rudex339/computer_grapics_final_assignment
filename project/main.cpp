// Reading Obj file
#define  _CRT_SECURE_NO_WARNINGS

#include "shader.h"
#include "computer_grapics_tool.h"
#include <random>

using namespace std;

#define PI 3.14

GLuint g_window_w = 600;
GLuint g_window_h = 600;

void Display();
void Reshape(int w, int h);
void Keyboard(unsigned char key, int x, int y);
void spckeycallback(int key, int x, int y);
GLvoid TimerFunction(int value);
void InitBuffer();

std::random_device rd;
class Cube {
private:
	
	GLfloat t_y, t_x, t_z;
	GLfloat sc_y;
	GLfloat change_sc_y;
	int state;
public:
	Cube() {
		
		sc_y =10.0f;
		change_sc_y = 0;
		state = 0;
	}
	void reset_Cube(GLfloat in_t_x, GLfloat in_t_y, GLfloat in_t_z) {
		state = 1;		
		t_y = in_t_y; t_x = in_t_x; t_z = in_t_z;
		sc_y = 10.0f;
	}
	void draw(GLint VAO) {
		glm::mat4 sc = glm::mat4(1.0f);
		sc = glm::scale(sc, glm::vec3(1.0, sc_y, 1.0));
		glm::mat4 Tx = glm::mat4(1.0f); 
		Tx = glm::translate(Tx, glm::vec3(t_x, t_y, t_z));

		unsigned int modelLocation = glGetUniformLocation(s_program[0], "modelTransform"); //--- 버텍스 세이더에서 모델링 변환 위치 가져오기
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Tx * sc));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	void change_sc_z(int a) {
		std::uniform_real_distribution <float> uid(0.02f, 0.5f);
		if (a == 0)
			change_sc_y = uid(rd);
		else
			change_sc_y = 0;
	}
	void make_road() {
		change_sc_y = 0;
		sc_y = 10.0f;
	}
	void update() {
		if (state == 0) {
			sc_y += change_sc_y;
			if ((sc_y > 15.0f) || (sc_y < 5.0f)) {
				change_sc_y = -change_sc_y;
			}
		}
		else if (state == 1) {
			t_y += 0.5f;
			if (t_y >= -10.0f) {
				state = 0;
			}
		}
	}
	void change_state(int input) {
		state = input;
	}
};
class Object {
private:
	GLuint VAO; GLuint VBO_position; GLuint VBO_color;
	GLint point_num;
	Cube*cube_list;
	int cube_num;

	int board[25][25] = { 0, };
	int xn; int yn;
	int on_maze;
public:
	Object() {
		point_num = 0;
		cube_list = NULL;
		on_maze = 0;
	}
	void in_object_list() {		

		if (cube_list != NULL) {
			delete(cube_list);
		}

		cout <<"input num (5<=num<=25)(ex: 5 10)= ";
		cin >> xn >> yn;
		if (xn < 5)xn = 5;
		else if (xn > 25)xn = 25;
		if (yn < 5)yn = 5;
		else if (yn > 25)yn = 25;
		cube_num = xn * yn;
		cube_list = new Cube[cube_num];
		glm::mat4 Tx = glm::mat4(1.0f);
		for (int i = 0; i < yn; i++) {
			for (int j = 0; j < xn; j++) {
				cube_list[i * xn + j].reset_Cube(2.0f * ((float)(-xn / 2) + (float)j) + 0.5f, -50.0f, 2.0f * ((float)(-yn / 2) + (float)i) + 0.5f);
			}
		}
	}

	void make_maze() {
		std::uniform_int_distribution <int> uid(0, 200);
		int save = 0;
		int loop = 1;
		int path_count = 1;
		int length = 0;
		int now_x = 0, now_y = 1;
		int first_direction = (uid(rd) % 2); // 0 가로, 1 세로
		int direction = 0; // 0->0 왼쪽, 0->1 오른쪽, 1->0 위쪽, 1->1 아래쪽 
		while (1) {

			if (first_direction % 2 == 0) {
				direction = (uid(rd) % 10);
				if (now_x == 0) {
					direction = 1;
				}
				else if (now_x == xn - 1) {
					direction = 0;
				}

				// 왼쪽
				if (direction % 4 == 0) {
					length = (uid(rd) % 4) + 2;
					for (int i = 0; i < length; ++i) {
						if (now_x == 0) {
							board[now_y][now_x] = path_count;
							break;
						}
						else {
							board[now_y][now_x] = path_count;
							now_x--;
							path_count++;
						}
					}
				}

				//오른쪽
				else {
					length = (uid(rd) % 4) + 2;
					for (int i = 0; i < length; ++i) {
						if (now_x == yn - 1) {
							board[now_y][now_x] = path_count;
							break;
						}
						else {
							board[now_y][now_x] = path_count;
							now_x++;
							path_count++;
						}
					}
				}
			}

			else {
				direction = (uid(rd) % 10);

				if (now_y == 0) {
					direction = 1;
				}
				else if (now_y == xn - 1) {
					direction = 0;
				}

				//위
				if (direction % 4 == 0) {
					length = (uid(rd) % 4) + 2;
					for (int i = 0; i < length; ++i) {
						if (now_y == 0) {
							board[now_y][now_x] = path_count;
							break;
						}
						board[now_y][now_x] = path_count;
						now_y--;
						path_count++;

					}
				}

				//아래
				else {
					length = (uid(rd) % 4) + 2;
					for (int i = 0; i < length; ++i) {
						if (now_y == yn - 1) {
							board[now_y][now_x] = path_count;
							break;
						}
						board[now_y][now_x] = path_count;
						now_y++;
						path_count++;

					}
				}
			}

			if (now_x == xn - 1 && now_y == yn - 1) {
				board[now_y][now_x] = path_count;
				break;
			}

			first_direction++;
		}

		for (int i = 0; i < yn; i++) {
			for (int j = 0; j < xn; j++) {
				if (board[i][j] == 0) {
					cube_list[i * xn + j].make_road();
				}
			}
		}
	}
	void init_buffer() {
		//std::uniform_real_distribution <float> uid(0.0f, 1.0f);
		objRead objReader;
		point_num = objReader.loadObj_normalize_center("cube.obj");
		float color[36][3] = {
			{1.0,0.0,1.0},{1.0,0.0,1.0},{1.0,0.0,1.0},{1.0,0.0,1.0},{1.0,0.0,1.0},{1.0,0.0,1.0},
			{1.0,0.0,1.0},{1.0,0.0,1.0},{1.0,0.0,1.0},{1.0,0.0,1.0},{1.0,0.0,1.0},{1.0,0.0,1.0},
			{1.0,1.0,1.0},{1.0,1.0,1.0},{1.0,1.0,1.0},{1.0,1.0,1.0},{1.0,1.0,1.0},{1.0,1.0,1.0},
			{1.0,1.0,1.0},{1.0,1.0,1.0},{1.0,1.0,1.0},{1.0,1.0,1.0},{1.0,1.0,1.0},{1.0,1.0,1.0},
			{1.0,1.0,1.0},{1.0,1.0,1.0},{1.0,1.0,1.0},{1.0,1.0,1.0},{1.0,1.0,1.0},{1.0,1.0,1.0},
			{1.0,1.0,1.0},{1.0,1.0,1.0},{1.0,1.0,1.0},{1.0,1.0,1.0},{1.0,1.0,1.0},{1.0,1.0,1.0}
		};
		
		glUseProgram(s_program[0]);
		GLint pAttribute = glGetAttribLocation(s_program[0], "aPos");
		GLint cAttribute = glGetAttribLocation(s_program[0], "in_Color");

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO_position);
		glGenBuffers(1, &VBO_color);
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_position);
		glBufferData(GL_ARRAY_BUFFER, point_num * sizeof(glm::vec3), &objReader.outvertex[0], GL_STATIC_DRAW);

		glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(pAttribute);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
		glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(glm::vec3), color[0], GL_STATIC_DRAW);

		glVertexAttribPointer(cAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(cAttribute);
	}
	void delete_buffer() {
	}
	void draw() {
		for (int i = 0; i < yn; i++) {
			for (int j = 0; j < xn; j++) {
				cube_list[i * xn + j].draw(VAO);
			}
		}
	}
	void update() {
		for (int i = 0; i < yn; i++) {
			for (int j = 0; j < xn; j++) {
				if (board[i][j] != 0 && on_maze == 1) {
					cube_list[i * xn + j].make_road();
				}
				cube_list[i * xn + j].update();
			}
		}
	}
	void handle_key(char key) {//23
		switch (key) {
		case 'm':
			for (int i = 0; i < cube_num; i++) {
				cube_list[i].change_sc_z(0);
			}
			break;
		case 'M':
			for (int i = 0; i < cube_num; i++) {
				cube_list[i].change_sc_z(1);
			}
			break;
		case 'r':
			on_maze = 1;
			make_maze();
			break;
		case 'v':
			break;
		}
	}
	void reset() {

	}
};
class World {
private:
	bool change_proj;
	glm::vec3 cameraPos;
	glm::vec3 cameraTarget;
	glm::vec3 up;

	Object object;
	int x_num, y_num;
	float turn_camera;
	float rady_camera;
public:
	World() {
		change_proj = false;
		cameraPos = glm::vec3(0, 40, 40);
		cameraTarget = glm::vec3(0, 0, 0);
		up = glm::vec3(0.0f, 1.0f, 0.0f);
		x_num = 10; y_num = 10;

		object.in_object_list();
		turn_camera = 0.0f;
		rady_camera = 0.0f;
	}
	void init_buffer() {
		object.init_buffer();
	}
	
	void draw() {

		glUseProgram(s_program[0]);

		glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
		glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
		glm::vec3 cameraup = glm::cross(cameraDirection, cameraRight);
		glm::mat4 view = glm::mat4(1.0f);

		view = glm::lookAt(cameraPos, cameraDirection, cameraup);

		unsigned int viewLocation = glGetUniformLocation(s_program[0], "viewTransform");
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

		glm::mat4 projection = glm::mat4(1.0f);
		unsigned int projectionLocation = glGetUniformLocation(s_program[0], "projectionTransform");
		if (change_proj) {
			projection = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, -1000.0f, 1000.0f);
			glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
		}
		else {
			projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
			projection = glm::translate(projection, glm::vec3(0.0f, 0.0, -2.0));
			glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
		}
		object.draw();

		cameraDirection = glm::normalize(glm::vec3(0, 0, 80) - cameraTarget);
		cameraRight = glm::normalize(glm::cross(glm::vec3(0.0f, 0.0f, -1.0f), cameraDirection));
		cameraup = glm::cross(cameraDirection, cameraRight);
		view = glm::mat4(1.0f);

		view = glm::lookAt(cameraPos, cameraDirection, cameraup);

		viewLocation = glGetUniformLocation(s_program[0], "viewTransform");
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

		projection = glm::mat4(1.0f);
		 projectionLocation = glGetUniformLocation(s_program[0], "projectionTransform");
			projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
			projection = glm::translate(projection, glm::vec3(10.0f, 0.0, -10.0));
			glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
			object.draw();
	}
	void update() {
		glm::mat4 Tx = glm::mat4(1.0f);
			cameraPos = glm::vec3(40*sin(rady_camera), 40, 40 * cos(rady_camera));
			rady_camera += 0.01f * turn_camera;
		object.update();
	}
	void handle_key(char key) {//23
		switch (key) {
		case 'o':
			change_proj = true;
			break;
		case 'p':
			change_proj = false;
			break;
		case 'y':
			turn_camera += 1.0f;
			break;
		case 'Y':
			turn_camera -= 1.0f;
			break;

		case 'c':
			reset();
			object.reset();
			break;
		}
		object.handle_key(key);
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