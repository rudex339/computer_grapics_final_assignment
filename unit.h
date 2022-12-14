#pragma once
#include "computer_grapics_tool.h"
#include "shader.h"
#include <random>

std::random_device rd;

class Tank {
public:
	glm::vec3 pos;
	glm::vec3 Front;
	float yaw_under;
	float yaw_upper;

	float TurningSpeed;
	float MovementSpeed;

	GLuint VAO_upper, VAO_under;
	GLuint n_vertex_upper, n_vertex_under;

	Tank() {
		pos = glm::vec3(0.0f, 0.0f, 0.0f);

		yaw_under = -90.0f;
		yaw_upper = -90.0f;

		TurningSpeed = 10.0f;
		MovementSpeed = 0.5f;

		glm::vec3 front;
		front.x = -sin(glm::radians(yaw_under));
		front.y = 0.0f;
		front.z = -cos(glm::radians(yaw_under));
		Front = glm::normalize(front);
	}
	void init() {
		objRead objReader, objReader2;
		GLuint VBO[2];
		n_vertex_under = objReader.loadObj_normalize_center("object/tank_under.obj");

		glUseProgram(s_program[0]);
		GLint pAttribute = glGetAttribLocation(s_program[0], "aPos");
		GLint nAttribute = glGetAttribLocation(s_program[0], "aNormal");
		GLint cAttribute = glGetAttribLocation(s_program[0], "in_Color");

		glGenVertexArrays(1, &VAO_under);
		glGenBuffers(2, VBO);
		glBindVertexArray(VAO_under);

		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		glBufferData(GL_ARRAY_BUFFER, objReader.outvertex.size() * sizeof(glm::vec3), &objReader.outvertex[0], GL_STATIC_DRAW);

		glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(pAttribute);
		glVertexAttribPointer(nAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(nAttribute);

		n_vertex_upper = objReader2.loadObj_normalize_center("object/tank_upper.obj");

		glGenVertexArrays(1, &VAO_upper);
		glBindVertexArray(VAO_upper);

		glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
		glBufferData(GL_ARRAY_BUFFER, objReader2.outvertex.size() * sizeof(glm::vec3), &objReader2.outvertex[0], GL_STATIC_DRAW);

		glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(pAttribute);
		glVertexAttribPointer(nAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(nAttribute);


		
		

		glm::vec3 front;
		front.x = -sin(glm::radians(yaw_under)) * 30;
		front.y = 20;
		front.z = -cos(glm::radians(yaw_under)) * 30;
		front = pos + front;
		unsigned int viewPosLocation = glGetUniformLocation(s_program[0], "viewPos"); //--- viewPos 값 전달: 카메라 위치
		glUniform3f(viewPosLocation, front.x, front.y, front.z);


	}
	void update() {
		glm::vec3 front;
		front.x = -sin(glm::radians(yaw_under));
		front.y = 0.0f;
		front.z = -cos(glm::radians(yaw_under));
		Front = glm::normalize(front);

		unsigned int viewPosLocation = glGetUniformLocation(s_program[0], "viewPos"); //--- viewPos 값 전달: 카메라 위치
		glUniform3f(viewPosLocation, pos.x+ front.x, pos.y + front.y, pos.z + front.z);
	}
	void draw() {
		glUseProgram(s_program[0]);
		unsigned int objColorLocation = glGetUniformLocation(s_program[0], "objectColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
		glUniform3f(objColorLocation, 0.0, 0.502, 0.0);
		glm::mat4 Tx = glm::mat4(1.0f);
		glm::mat4 Tx2 = glm::mat4(1.0f);//--- 이동 행렬 선언
		glm::mat4 Rz = glm::mat4(1.0f); //--- 회전 행렬 선언
		glm::mat4 sc = glm::mat4(1.0f);
		glm::mat4 TR = glm::mat4(1.0f);//--- 합성 변환 행렬

		sc = glm::scale(sc, glm::vec3(1.2, 0.8, 2.5));
		Tx = glm::translate(Tx, glm::vec3(pos.x, pos.y, pos.z)); //--- x축으로 이동 행렬
		Rz = glm::rotate(TR, glm::radians(yaw_under), glm::vec3(0.0, 1.0, 0.0)) * Rz;

		TR = Tx * Rz * sc;

		unsigned int modelLocation = glGetUniformLocation(s_program[0], "modelTransform"); //--- 버텍스 세이더에서 모델링 변환 위치 가져오기
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));

		glBindVertexArray(VAO_under);
		glDrawArrays(GL_TRIANGLES, 0, n_vertex_under);

		Rz = glm::rotate(Tx2, glm::radians(yaw_upper), glm::vec3(0.0, 1.0, 0.0));
		Tx2 = glm::translate(Tx2, glm::vec3(0.0f, 1.2f, 0.8f)); //--- x축으로 이동 행렬


		TR = Tx * Rz*Tx2 * sc;

		modelLocation = glGetUniformLocation(s_program[0], "modelTransform"); //--- 버텍스 세이더에서 모델링 변환 위치 가져오기
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));

		glBindVertexArray(VAO_upper);
		glDrawArrays(GL_TRIANGLES, 0, n_vertex_upper);
	}
	void collide() {
		
	}
	void get_tank_camera() {

	}
	void handle_key(unsigned char key) {
		switch (key) {
		case 'w':
		case 'W':
			pos -= Front * MovementSpeed;
			break;
		case 'a':
		case 'A':
			yaw_under += TurningSpeed;
			yaw_upper += TurningSpeed;
			break;
		case 's':
		case 'S':
			pos += Front * MovementSpeed;
			break;
		case 'd':
		case 'D':
			yaw_under -= TurningSpeed;
			yaw_upper -= TurningSpeed;
			break;
		case 'j':
		case 'J':
			yaw_upper += TurningSpeed;
			break;
		case 'l':
		case 'L':
			yaw_upper -= TurningSpeed;
			break;
		}
	}

	glm::mat4 GetViewMatrix(glm::vec3 cameraPosition)
	{
		glm::vec3 front;
		front.x = -sin(glm::radians(yaw_under))*30;
		front.y = 20;
		front.z = -cos(glm::radians(yaw_under))*30;
		return glm::lookAt(pos + front, pos, glm::vec3(0.0f, 1.0f, 0.0f));
		//return glm::lookAt(glm::vec3(0.0f, 10.0f, 60.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 get_pos() {
		glm::vec3 P = glm::vec3(pos.x, pos.z, 1.0f);
		return P;
	}
	~Tank() {};
 
};

class enemy {
public:
	glm::vec3 pos;
	glm::vec3 Front;
	float yaw_under;
	float yaw_upper;

	float TurningSpeed;
	float MovementSpeed;

	GLuint VAO_upper, VAO_under;
	GLuint n_vertex_upper, n_vertex_under;

	enemy() {
		std::uniform_real_distribution <float> uid(-40.0f, 40.0f);
		pos = glm::vec3(uid(rd), 0.0f, uid(rd));

		yaw_under = -90.0f;
		yaw_upper = -90.0f;

		TurningSpeed = 10.0f;
		MovementSpeed = 0.5f;

		glm::vec3 front;
		front.x = -sin(glm::radians(yaw_under));
		front.y = 0.0f;
		front.z = -cos(glm::radians(yaw_under));
		Front = glm::normalize(front);
	}
	void init() {
		objRead objReader, objReader2;
		GLuint VBO[2];
		n_vertex_under = objReader.loadObj_normalize_center("object/tank_under.obj");

		glUseProgram(s_program[0]);
		GLint pAttribute = glGetAttribLocation(s_program[0], "aPos");
		GLint nAttribute = glGetAttribLocation(s_program[0], "aNormal");
		GLint cAttribute = glGetAttribLocation(s_program[0], "in_Color");

		glGenVertexArrays(1, &VAO_under);
		glGenBuffers(2, VBO);
		glBindVertexArray(VAO_under);

		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		glBufferData(GL_ARRAY_BUFFER, objReader.outvertex.size() * sizeof(glm::vec3), &objReader.outvertex[0], GL_STATIC_DRAW);

		glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(pAttribute);
		glVertexAttribPointer(nAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(nAttribute);

		n_vertex_upper = objReader2.loadObj_normalize_center("object/tank_upper.obj");

		glGenVertexArrays(1, &VAO_upper);
		glBindVertexArray(VAO_upper);

		glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
		glBufferData(GL_ARRAY_BUFFER, objReader2.outvertex.size() * sizeof(glm::vec3), &objReader2.outvertex[0], GL_STATIC_DRAW);

		glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(pAttribute);
		glVertexAttribPointer(nAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(nAttribute);





		glm::vec3 front;
		front.x = -sin(glm::radians(yaw_under)) * 30;
		front.y = 20;
		front.z = -cos(glm::radians(yaw_under)) * 30;
		front = pos + front;
		unsigned int viewPosLocation = glGetUniformLocation(s_program[0], "viewPos"); //--- viewPos 값 전달: 카메라 위치
		glUniform3f(viewPosLocation, front.x, front.y, front.z);


	}
	void update() {
		glm::vec3 front;
		front.x = -sin(glm::radians(yaw_under));
		front.y = 0.0f;
		front.z = -cos(glm::radians(yaw_under));
		Front = glm::normalize(front);

		unsigned int viewPosLocation = glGetUniformLocation(s_program[0], "viewPos"); //--- viewPos 값 전달: 카메라 위치
		glUniform3f(viewPosLocation, pos.x + front.x, pos.y + front.y, pos.z + front.z);
	}
	void draw() {
		glUseProgram(s_program[0]);
		unsigned int objColorLocation = glGetUniformLocation(s_program[0], "objectColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
		glUniform3f(objColorLocation, 0.0, 0.502, 0.0);
		glm::mat4 Tx = glm::mat4(1.0f);
		glm::mat4 Tx2 = glm::mat4(1.0f);//--- 이동 행렬 선언
		glm::mat4 Rz = glm::mat4(1.0f); //--- 회전 행렬 선언
		glm::mat4 sc = glm::mat4(1.0f);
		glm::mat4 TR = glm::mat4(1.0f);//--- 합성 변환 행렬

		sc = glm::scale(sc, glm::vec3(1.2, 0.8, 2.5));
		Tx = glm::translate(Tx, glm::vec3(pos.x, pos.y, pos.z)); //--- x축으로 이동 행렬
		Rz = glm::rotate(TR, glm::radians(yaw_under), glm::vec3(0.0, 1.0, 0.0)) * Rz;

		TR = Tx * Rz * sc;

		unsigned int modelLocation = glGetUniformLocation(s_program[0], "modelTransform"); //--- 버텍스 세이더에서 모델링 변환 위치 가져오기
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));

		glBindVertexArray(VAO_under);
		glDrawArrays(GL_TRIANGLES, 0, n_vertex_under);

		Rz = glm::rotate(Tx2, glm::radians(yaw_upper), glm::vec3(0.0, 1.0, 0.0));
		Tx2 = glm::translate(Tx2, glm::vec3(0.0f, 1.2f, 0.8f)); //--- x축으로 이동 행렬


		TR = Tx * Rz * Tx2 * sc;

		modelLocation = glGetUniformLocation(s_program[0], "modelTransform"); //--- 버텍스 세이더에서 모델링 변환 위치 가져오기
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));

		glBindVertexArray(VAO_upper);
		glDrawArrays(GL_TRIANGLES, 0, n_vertex_upper);
	}
	void collide() {

	}
	void get_tank_camera() {

	}

	glm::vec3 get_pos() {
		glm::vec3 P = glm::vec3(pos.x, pos.z, 1.0f);
		return P;
	}

};