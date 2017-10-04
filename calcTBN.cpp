#include <vector>
#include <glm/glm.hpp>
#include "calcTBN.h"

void tanCalc(vector<glm::vec3> &vertices, vector<glm::vec3> &normals, vector<glm::vec2> &uvs, vector<glm::vec3> &tangents, vector<glm::vec3> &bitangents)
{
	for (unsigned int i = 0; i < vertices.size(); i += 3) {
		glm::vec3 &vertex0 = vertices[i + 0];
		glm::vec3 &vertex1 = vertices[i + 1];
		glm::vec3 &vertex2 = vertices[i + 2];
		glm::vec2 &uv0 = uvs[i + 0];
		glm::vec2 &uv1 = uvs[i + 1];
		glm::vec2 &uv2 = uvs[i + 2];
		glm::vec3 edgePosition1 = vertex1 - vertex0; //calculate edge 1 of traingle
		glm::vec3 edgePosition2 = vertex2 - vertex0; //calculate edge 2 of traingle
		glm::vec2 edgeUV1 = uv1 - uv0; //calculate uv edge 1 of traingle
		glm::vec2 edgeUV2 = uv2 - uv0; //calculate uv edge 2 of traingle
		float r = 1.0f / (edgeUV1.x * edgeUV2.y - edgeUV1.y * edgeUV2.x);
		glm::vec3 tangent = (edgePosition1 * edgeUV2.y - edgePosition2 * edgeUV1.y)*r; //calculate tangent
		glm::vec3 bitangent = (edgePosition2 * edgeUV1.x - edgePosition1 * edgeUV2.x)*r; //calculate bitangent
		tangents.push_back(tangent); //tangent for triangle vertex 1
		bitangents.push_back(bitangent);//bitangent for triangle vertex 1
		tangents.push_back(tangent); //tangent for triangle vertex 2
		bitangents.push_back(bitangent);//bitangent for triangle vertex 2
		tangents.push_back(tangent); //tangent for triangle vertex 3
		bitangents.push_back(bitangent);
	}//bitangent for triangle vertex 3
	for (unsigned int i = 0; i < vertices.size(); i += 1) {
		glm::vec3& normal = normals[i];
		glm::vec3& tangent = tangents[i];
		glm::vec3& bitangent = bitangents[i];
		tangent = glm::normalize(tangent - normal * glm::dot(normal, tangent)); //orthogonalization
	}
}

