#ifndef MESHLOADER_H
#define MESHLOADER_H

#include <vector>
using namespace std;
bool meshLoad (const char * fileDir, vector<glm::vec3> &verticesOutput, vector<glm::vec3> &normalsOutput, vector<glm::vec2> &uvOutput);
bool similarVertex(float vertex1, float vertex2);
void vboIndex(vector<glm::vec3> &tangentsInput, vector<glm::vec3> &tangentsOutput, vector<glm::vec3> &bitangentsInput, vector<glm::vec3> &bitangentsOutput,
	vector<glm::vec2> &uvInput, vector<glm::vec2> &uvOutput, vector<glm::vec3> &verticesInput, vector<glm::vec3> &verticesOutput,
	vector<glm::vec3> &normalsInput, vector<glm::vec3> &normalsOutput, vector<unsigned short> &indicesOutput);
#endif