/*
Represents a cylinder

The lecture slides were used as a reference for the general idea however all of the code is my own unless otherwise referenced

Assignment2:

This now supports shadows
*/

#include "wrapper_glfw.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Cylinder.h"
#include <iostream>
#include <vector>
#include "Shadow.h"
Cylinder::Cylinder(GLuint modelId, glm::vec3 startPosition, glm::vec3 cylinderScale, GLuint textureID, GLuint triangleCount)
{
	model_id = modelId;
	position = startPosition;
	scale = cylinderScale;
	texture_id = textureID;
	GLuint triangles = triangleCount;
	Initialise(triangles);
}

//Initialises the cylinder with vertex positions, normals and a texture
//Triangle count is the number of triangles in each of the two cylinder circles
void Cylinder::Initialise(GLuint triangleCount)
{
	//Create the 2 circles for the cylinder
	CreateFlatFaces(triangleCount, 5.f);
	//Creates the sides of the cylinder
	CreateSides(triangleCount, 5.f);

	GLuint num_vertices = (pVertices.size() / 3);

	int single_circle_vertex_part_count = (pVertices.size() + 6) / 4;
	int start_of_side_vertices = pVertices.size() / 2;
	//Assign the triangle strip indices
	for (int i = start_of_side_vertices + 1; i < start_of_side_vertices + single_circle_vertex_part_count; i++)
	{
		pIndices.push_back((i - 3 + single_circle_vertex_part_count) /3);
		pIndices.push_back(i/3);
	}

	glGenBuffers(1, &position_buffer_object);
	glBindBuffer(GL_ARRAY_BUFFER, position_buffer_object);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* pVertices.size(), &pVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &normals_buffer_object);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, normals_buffer_object);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, pNormals.size() * sizeof(GLfloat), &pNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &texture_buffer_object);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, texture_buffer_object);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, pTexCoords.size() * sizeof(GLfloat), &pTexCoords[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &element_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, pIndices.size() * sizeof(GLuint), &pIndices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//Adds a vertex to the cylinder
void Cylinder::AddVertex(GLfloat x, GLfloat y, GLfloat z)
{

	pVertices.push_back(x);
	pVertices.push_back(y);
	pVertices.push_back(z);
}

//Adds a normal to the cylinder
void Cylinder::AddNormal(GLfloat x, GLfloat y, GLfloat z)
{
	pNormals.push_back(x);
	pNormals.push_back(y);
	pNormals.push_back(z);
}

//Creates the flat faces of the cylinder
//Triangles is the number of triangles for each face
//Height is the distance between the faces
void Cylinder::CreateFlatFaces(GLuint triangles, GLfloat height)
{
	//Define the central vertex
	AddVertex(0, height, 0);
	//Assign a normal to the central vertex
	AddNormal(0, 1, 0);

	pTexCoords.push_back(0.5);
	pTexCoords.push_back(0.5);

	int start = pVertices.size();
	const float DEG2RAD = 3.14159 / 180;
	GLuint angle_step = 360 / triangles - 1;
#pragma region Top face
	//Top flat face
	for (int angle = angle_step; angle <= 360; angle += angle_step)
	{
		GLfloat x = cos(angle * DEG2RAD);
		GLfloat z = sin(angle* DEG2RAD);

		AddVertex(x, height, z);

		pTexCoords.push_back((x * 0.5) + 0.5);
		pTexCoords.push_back((z * 0.5) + 0.5);//Referenced http://stackoverflow.com/questions/8762826/texture-mapping-a-circle-made-using-gl-polygon

		//Assign 'up' normals to each of the face vertices
		AddNormal(0, 1, 0);
	}
	//Connect the last triangle fan to the first to complete the circle
	AddVertex(pVertices[start], pVertices[start + 1], pVertices[start + 2]);
	pTexCoords.push_back((pVertices[start] * 0.5) + 0.5);
	pTexCoords.push_back((pVertices[start + 2] * 0.5) + 0.5);
	//Assign a normal to the final triangle
	AddNormal(0, 1, 0);
#pragma endregion
#pragma region bottom face
	AddVertex(0, 0, 0);
	//Assign a normal to the central vertex
	AddNormal(0, -1, 0);

	pTexCoords.push_back(0.5);
	pTexCoords.push_back(0.5);

	start = pVertices.size();
	//Bottom flat face
	for (int angle = angle_step; angle <= 360; angle += angle_step)
	{
		GLfloat x = cos(angle * DEG2RAD);
		GLfloat z = sin(angle* DEG2RAD);

		AddVertex(x, 0, z);

		pTexCoords.push_back((x * 0.5) + 0.5);
		pTexCoords.push_back((z * 0.5) + 0.5);//Referenced http://stackoverflow.com/questions/8762826/texture-mapping-a-circle-made-using-gl-polygon

		//Assign 'up' normals to each of the face vertices
		AddNormal(0, -1, 0);
	}
	//Connect the last triangle fan to the first to complete the circle
	AddVertex(pVertices[start], pVertices[start + 1], pVertices[start + 2]);
	pTexCoords.push_back((pVertices[start] * 0.5) + 0.5);
	pTexCoords.push_back((pVertices[start + 2] * 0.5) + 0.5);
	//Assign a normal to the final triangle
	AddNormal(0, -1, 0);
#pragma endregion
}

//Creates the vertices used for the sides of the cylinder
//triangles is the number of triangles to be used
//height is the height of the cylinder
void Cylinder::CreateSides(GLuint triangles, GLfloat height)
{
	pTexCoords.push_back(0);
	pTexCoords.push_back(1);
	int start = pVertices.size();
	const float DEG2RAD = 3.14159 / 180;
	GLuint angle_step = 360 / triangles - 1;
#pragma region Top face
	//Top vertices
	for (int angle = angle_step; angle <= 360; angle += angle_step)
	{
		GLfloat rad_angle = DEG2RAD * angle;
		GLfloat x = cos(rad_angle);
		GLfloat z = sin(rad_angle);

		AddVertex(x, height, z);
		pTexCoords.push_back(rad_angle);
		pTexCoords.push_back(1);
		//Assign side normals to each of the side vertices
		AddNormal(x, 0, z);
	}
	//Connect the last triangle fan to the first to complete the circle
	AddVertex(pVertices[start], pVertices[start + 1], pVertices[start + 2]);
	//Assign a normal to the final triangle
	AddNormal(pVertices[start], 0, pVertices[start + 2]);
#pragma endregion
#pragma region bottom face
	pTexCoords.push_back(0);
	pTexCoords.push_back(0);
	start = pVertices.size();
	//Bottom vertices
	for (int angle = angle_step; angle <= 360; angle += angle_step)
	{
		GLfloat rad_angle = DEG2RAD * angle;
		GLfloat x = cos(rad_angle);
		GLfloat z = sin(rad_angle);

		AddVertex(x, 0, z);
		pTexCoords.push_back(rad_angle);
		pTexCoords.push_back(0);
		//Assign a side normal to the vertice
		AddNormal(x, 0, z);
	}
	//Connect the last triangle fan to the first to complete the circle
	AddVertex(pVertices[start], pVertices[start + 1], pVertices[start + 2]);
	//Assign a normal to the final triangle
	AddNormal(pVertices[start], 0, pVertices[start + 2]);
#pragma endregion
}

//Renders, scales and translates the cylinder to its position
void Cylinder::Render(glm::mat4 transformation)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id);//Bind the cylinder's texture
	glUniformMatrix4fv(model_id, 1, GL_FALSE, &transformation[0][0]);
	glBindBuffer(GL_ARRAY_BUFFER, position_buffer_object);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, normals_buffer_object);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, texture_buffer_object);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLE_FAN, 0, pVertices.size() / 12 + 1);//Top flat face of the cylinder
	glDrawArrays(GL_TRIANGLE_FAN, pVertices.size() / 12 + 1, pVertices.size() / 12 + 1);//Bottom flat face of the cylinder
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
	glDrawElements(GL_TRIANGLE_STRIP, pIndices.size(), GL_UNSIGNED_INT, (GLvoid*)(0));//Triangle strips used for the side of the cylinder
}
//Draw the shadow of the cylinder using blending
void Cylinder::DrawShadow(glm::mat4 shadowTransformation)
{
	glEnable(GL_BLEND);
	glUniformMatrix4fv(model_id, 1, GL_FALSE, &shadowTransformation[0][0]);

	glBindBuffer(GL_ARRAY_BUFFER, position_buffer_object);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, normals_buffer_object);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, texture_buffer_object);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLE_FAN, 0, pVertices.size() / 12 + 1);//Top flat face of the cylinder
	glDrawArrays(GL_TRIANGLE_FAN, pVertices.size() / 12 + 1, pVertices.size() / 12 + 1);//Bottom flat face of the cylinder
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
	glDrawElements(GL_TRIANGLE_STRIP, pIndices.size(), GL_UNSIGNED_INT, (GLvoid*)(0));//Triangle strips used for the side of the cylinder
	glDisable(GL_BLEND);
}
