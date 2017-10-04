#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string.h>
#include <GL/glew.h>
#include <IMAGE.h>
#include <GL/glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <meshLoader.h>
#include <calcTBN.h>
using namespace glm;
using namespace std;
GLFWwindow* window;

int main( void )
{
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}
	glfwWindowHint(GLFW_SAMPLES, 1);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(512,512,"CSI4130 Final Project", NULL, NULL);
	glfwMakeContextCurrent(window);
	glewExperimental = true; 
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); 
	glEnable(GL_CULL_FACE);
	GLuint vao1;
	glGenVertexArrays(1, &vao1);
	glBindVertexArray(vao1);
	GLuint program;
	GLuint vsID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fsID = glCreateShader(GL_FRAGMENT_SHADER);
	const char * vertex_file_path = "NormalMapping.vs";
	const char * fragment_file_path = "NormalMapping.fs";
	string vsCode;
	ifstream vsFile(vertex_file_path,ios::in);
	if (vsFile.is_open()) {
		string Line = "";
		while (getline(vsFile, Line))
			vsCode += "\n" + Line;
		vsFile.close();
	}
	string fsCode;
	ifstream fsFile(fragment_file_path,ios::in);
	if (fsFile.is_open()) {
		string Line = "";
		while (getline(fsFile, Line))
			fsCode += "\n" + Line;
		fsFile.close();
	}
	GLint Result = GL_FALSE;
	char const * vsPointer = vsCode.c_str();
	glShaderSource(vsID, 1, &vsPointer, NULL);
	glCompileShader(vsID);
	char const * fsPointer = fsCode.c_str();
	glShaderSource(fsID, 1, &fsPointer, NULL);
	glCompileShader(fsID);
	program = glCreateProgram();
	glAttachShader(program, vsID);
	glAttachShader(program, fsID);
	glLinkProgram(program);

	GLuint NormalTexture;
	IMAGE normalMapImage;
	normalMapImage.Load("normal2.bmp");
	normalMapImage.ExpandPalette();

	//Convert normal map to texture
	glGenTextures(1, &NormalTexture);
	glBindTexture(GL_TEXTURE_2D, NormalTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, normalMapImage.width, normalMapImage.height,
		0, normalMapImage.format, GL_UNSIGNED_BYTE, normalMapImage.data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	vector<unsigned short> indices;
	vector<glm::vec3> indexedVertices;
	vector<glm::vec2> indexedUvs;
	vector<glm::vec3> indexedNormals;
	vector<glm::vec3> indexedTangents;
	vector<glm::vec3> indexedBitangents;
	vector<glm::vec3> vertices;
	vector<glm::vec3> normals;
	vector<glm::vec2> uvs;
	vector<glm::vec3> tangents;
	vector<glm::vec3> bitangents;
	bool res = meshLoad("cube.obj", vertices, normals, uvs);
	tanCalc(vertices, normals, uvs, tangents, bitangents);
	vboIndex(tangents, indexedTangents, bitangents, indexedBitangents, uvs, indexedUvs, vertices, indexedVertices, normals, indexedNormals, indices);
//load buffers to gpu
	GLuint verticesBuffer, normalsBuffer, uvsBuffer, tangentsBuffer, bitangentsBuffer, indicesBuffer;
	glGenBuffers(1, &verticesBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, verticesBuffer);
	glBufferData(GL_ARRAY_BUFFER, indexedVertices.size() * sizeof(glm::vec3), &indexedVertices[0], GL_STATIC_DRAW);
	glGenBuffers(1, &normalsBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
	glBufferData(GL_ARRAY_BUFFER, indexedNormals.size() * sizeof(glm::vec3), &indexedNormals[0], GL_STATIC_DRAW);
	glGenBuffers(1, &uvsBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvsBuffer);
	glBufferData(GL_ARRAY_BUFFER, indexedUvs.size() * sizeof(glm::vec2), &indexedUvs[0], GL_STATIC_DRAW);
	glGenBuffers(1, &tangentsBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, tangentsBuffer);
	glBufferData(GL_ARRAY_BUFFER, indexedTangents.size() * sizeof(glm::vec3), &indexedTangents[0], GL_STATIC_DRAW);
	glGenBuffers(1, &bitangentsBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, bitangentsBuffer);
	glBufferData(GL_ARRAY_BUFFER, indexedBitangents.size() * sizeof(glm::vec3), &indexedBitangents[0], GL_STATIC_DRAW);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glGenBuffers(1, &indicesBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);
	glUseProgram(program);
	GLuint lightID = glGetUniformLocation(program, "lightWorld");
	GLuint mvpID = glGetUniformLocation(program, "MVP");
	GLuint modelID = glGetUniformLocation(program, "Model");
	GLuint viewID = glGetUniformLocation(program, "View");
	GLuint modelView2ID = glGetUniformLocation(program, "ModelView2");
	do{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(program);
		glm::mat4 projection=glm::ortho(-5.0f / 2.0f, 5.0f / 2.0f,-5.0f / 2.0f, 5.0f / 2.0f,4.0f, 8.0f);
		glm::mat4 view = glm::lookAt(glm::vec3(2.0f, 2.0f, -(8.0f + 4.0f) / 2.0f),glm::vec3(0, 0, 0),glm::vec3(0, 1.0f, 0));
		glm::mat4 model = glm::mat4(1.0);
		glm::mat4 modelView = view * model;
		glm::mat3 modelView2 = glm::mat3(modelView);
		glm::mat4 MVP = projection * view * model;
		glm::vec3 lightPos = glm::vec3(0, 0, 4);
		glUniformMatrix4fv(mvpID, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(viewID, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(viewID, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix3fv(modelView2ID, 1, GL_FALSE, &modelView2[0][0]);
		glUniform3f(lightID, lightPos.x, lightPos.y, lightPos.z);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, verticesBuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvsBuffer);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(3);
		glBindBuffer(GL_ARRAY_BUFFER, tangentsBuffer);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(4);
		glBindBuffer(GL_ARRAY_BUFFER, bitangentsBuffer);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, (void*)0);
		glUseProgram(0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0 );
	glfwTerminate();
	return 0;
}

