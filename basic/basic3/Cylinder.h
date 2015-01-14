#pragma once

#include "wrapper_glfw.h"

/* Include GLM core and matrix extensions*/
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <vector>
class Cylinder
{
private:
	void AddVertex(GLfloat x, GLfloat y, GLfloat z);
	void AddNormal(GLfloat x, GLfloat y, GLfloat z);
	void CreateSides(GLuint triangles, GLfloat height);
	void Initialise(GLuint triangleCount);
	void CreateFlatFaces(GLuint triangles, GLfloat height);
protected:
	std::vector<GLfloat> pVertices;
	std::vector<GLfloat> pTexCoords;
	std::vector<GLuint> pIndices;
	std::vector<GLfloat> pNormals;
	glm::vec3 position;
	glm::vec3 scale;
	GLuint model_id, position_buffer_object, element_buffer, normals_buffer_object, texture_buffer_object, texture_id;
	void Cylinder::DrawCylinder();
public:
	Cylinder::Cylinder(GLuint modelId, glm::vec3 startPosition, glm::vec3 cylinderScale, GLuint textureID, GLuint triangleCount = 50);
	void Move(GLfloat xInc, GLfloat yInc, GLfloat zInc);
	void Render(glm::mat4 transformation);
	void DrawShadow(glm::mat4 transformation);
};