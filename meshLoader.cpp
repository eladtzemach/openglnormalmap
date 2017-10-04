#include <vector>
#include <cstring>
#include <string>
#include <stdio.h>
#include <glm/glm.hpp>
#include "meshLoader.h"
#include <map>
using namespace std;
bool similarIndex( 
		glm::vec2 &uvInput, glm::vec3 &verticesInput, glm::vec3 &normalsInput, 
	vector<glm::vec3> &verticesOutput, vector<glm::vec2> &uvOutput, vector<glm::vec3> &normalsOutput,
	short &result ){
	for (int i=0; i<verticesOutput.size(); i++ ){
		if (similarVertex(uvInput.x, uvOutput[i].x)&&similarVertex(uvInput.y,uvOutput[i].y) &&similarVertex(normalsInput.x,normalsOutput[i].x)&&similarVertex(normalsInput.y,normalsOutput[i].y)  &&similarVertex(normalsInput.z,normalsOutput [i].z)&&similarVertex(verticesInput.x,verticesOutput[i].x)  &&similarVertex(verticesInput.y,verticesOutput[i].y)&&similarVertex(verticesInput.z,verticesOutput[i].z)){
			result = i;
			return true; }}
	return false;}
bool similarVertex(float vertex1, float vertex2){
	return fabs( vertex1-vertex2 ) < 0.01f;
}
bool meshLoad (const char * fileDir,vector<glm::vec3> &verticesOutput, vector<glm::vec3> &normalsOutput, vector<glm::vec2> &uvOutput) {
	vector<glm::vec3> vertices; 
		vector<glm::vec3> normals;
	vector<glm::vec2> uvs;
	FILE * file = fopen(fileDir, "r");
	vector<unsigned int> vertexIndices, normalIndices, uvIndices;
	while (1) {
		char fileHeader[128];
		int openedFile = fscanf(file, "%s", fileHeader);
		if (openedFile == EOF) break;
		if (strcmp(fileHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			vertices.push_back(vertex);
		}
		else if (strcmp(fileHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			normals.push_back(normal);
		}
		else if (strcmp(fileHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			uvs.push_back(uv);
		}
		
		 else if (strcmp(fileHeader,"f")==0){
	string vertex1,vertex2,vertex3;
		unsigned int vertexIndex[3],uvIndex[3],normalIndex[3];
	int scan = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
	vertexIndices.push_back(vertexIndex[0]);
		vertexIndices.push_back(vertexIndex[1]);
	vertexIndices.push_back(vertexIndex[2]);
				normalIndices.push_back(normalIndex[0]);
normalIndices.push_back(normalIndex[1]);
normalIndices.push_back(normalIndex[2]);
   uvIndices.push_back(uvIndex[0]);
	uvIndices.push_back(uvIndex[1]);
		uvIndices.push_back(uvIndex[2]);}
		else{ char buffer[1000];
			fgets(buffer, 1000, file);}}
for(unsigned int i=0;i<vertexIndices.size(); i++){
		unsigned int vertexIndex = vertexIndices[i];
		glm::vec3 vertex=vertices[vertexIndex-1];
		verticesOutput.push_back(vertex);
			unsigned int normalIndex = normalIndices[i];
	glm::vec3 normal=normals[normalIndex-1];
	normalsOutput.push_back(normal);
		unsigned int uvIndex = uvIndices[i];		
		glm::vec2 uv = uvs[uvIndex-1];
	uvOutput.push_back(uv);
		}return true;}

void vboIndex( vector<glm::vec3> &tangentsInput, vector<glm::vec3> &tangentsOutput, vector<glm::vec3> &bitangentsInput, vector<glm::vec3> &bitangentsOutput,
	vector<glm::vec2> &uvInput, vector<glm::vec2> &uvOutput, vector<glm::vec3> &verticesInput, vector<glm::vec3> &verticesOutput,
	vector<glm::vec3> &normalsInput, vector<glm::vec3> &normalsOutput, vector<unsigned short> &indicesOutput){
	for (int i=0; i<verticesInput.size();i++){
		short index;
		bool vertexFound = similarIndex(uvInput[i],verticesInput[i],normalsInput[i],verticesOutput,uvOutput,normalsOutput,index);
		if (vertexFound){
			indicesOutput.push_back(index);
			tangentsOutput[index] += tangentsInput[i];
			bitangentsOutput[index] += bitangentsInput[i];}
		else{ 
						normalsOutput.push_back( normalsInput[i]);
			verticesOutput.push_back( verticesInput[i]);
						tangentsOutput.push_back( tangentsInput[i]);
									bitangentsOutput.push_back( bitangentsInput[i]);
			uvOutput.push_back(uvInput[i]);
			indicesOutput.push_back((short)verticesOutput.size() - 1 );}}}
