#pragma once
#include "computer_grapics_tool.h"
#include "shader.h"

class Ground {
public:
	glm::vec3 pos;
	glm::vec3 point_pos;

	GLuint VAO;
	GLuint n_vertex;

	Ground() {
		std::uniform_real_distribution <float> uid(-40.0f, 40.0f);
		pos = glm::vec3(0.0f, -5.0f, 0.0f);
		point_pos = glm::vec3(uid(rd), 0.0f, uid(rd));
		
	}
	void init() {
		objRead objReader;
		GLuint VBO[3];
		n_vertex= objReader.loadObj_normalize_center("object/cube.obj");

		glUseProgram(s_program[0]);
		GLint pAttribute = glGetAttribLocation(s_program[0], "aPos");
		GLint nAttribute = glGetAttribLocation(s_program[0], "aNormal");
		GLint cAttribute = glGetAttribLocation(s_program[0], "in_Color");

		glGenVertexArrays(1, &VAO);
		glGenBuffers(2, VBO);
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		glBufferData(GL_ARRAY_BUFFER, objReader.outvertex.size() * sizeof(glm::vec3), &objReader.outvertex[0], GL_STATIC_DRAW);

		glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(pAttribute);
		glVertexAttribPointer(nAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(nAttribute);

	}
	void update() {}
	void draw() {
		glUseProgram(s_program[0]);
		unsigned int objColorLocation = glGetUniformLocation(s_program[0], "objectColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
		glUniform3f(objColorLocation, 1.0, 1.0, 1.0);
		
		glm::mat4 Tx = glm::mat4(1.0f);
		glm::mat4 sc = glm::mat4(1.0f);
		glm::mat4 TR = glm::mat4(1.0f);//--- 합성 변환 행렬

		sc = glm::scale(sc, glm::vec3(50.0, 1.0, 50.0));
		Tx = glm::translate(Tx, glm::vec3(pos.x, pos.y, pos.z)); //--- x축으로 이동 행렬

		TR = Tx * sc;

		unsigned int modelLocation = glGetUniformLocation(s_program[0], "modelTransform"); //--- 버텍스 세이더에서 모델링 변환 위치 가져오기
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, n_vertex);

		sc = glm::scale(glm::mat4(1.0f), glm::vec3(1.0, 1.0, 1.0));
		Tx = glm::translate(glm::mat4(1.0f), glm::vec3(point_pos.x, point_pos.y, point_pos.z)); //--- x축으로 이동 행렬

		TR = Tx * sc;

		modelLocation = glGetUniformLocation(s_program[0], "modelTransform"); //--- 버텍스 세이더에서 모델링 변환 위치 가져오기
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));

		objColorLocation = glGetUniformLocation(s_program[0], "objectColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
		glUniform3f(objColorLocation, 0.0, 0.0, 1.0);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, n_vertex);
	}
	void collide_p() {
		std::uniform_real_distribution <float> uid(-40.0f, 40.0f);
		point_pos = glm::vec3(uid(rd), 0.0f, uid(rd));
	}

	glm::vec3 get_pos_p() {
		glm::vec3 P = glm::vec3(point_pos.x, point_pos.z, 0.5f);
		return P;
	}
	~Ground() {};
private:
};
