//Bonfire
//This is generated using cylinders which are positioned and rotated inwards procedurally

#include "wrapper_glfw.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Bonfire.h"
#include <iostream>
#include <vector>

Bonfire::Bonfire(GLuint modelId, glm::vec3 bonfirePosition, GLuint logTexture)
{
	model_id = modelId;
	position = bonfirePosition;
	log_texture = logTexture;
}
//Place the logs and rotate them in the right direction
void Bonfire::Initialise()
{
	const float DEG2RAD = 3.14159 / 180;
	int angle_step = 30;
	float inwards_angle = 30;
	float z_rot = 0;
	float x_rot = -30;
	float xrot[12] = { 30, 20, 10, 0, -10, -20, -30, -20, -10, 0, 10, 20};//These rotation values point the cylinders inwards towards a point slightly crookedly
	float zrot[12] = { 0, -10, -20, -30, -20, -10, 0, 10, 20, 30, 20, 10};//to give a more realistic man-made type look
	int count = 0;
	for (int angle = 0; angle < 360; angle += angle_step)
	{
		GLfloat x = cos(angle * DEG2RAD) * 3.5;
		GLfloat z = sin(angle* DEG2RAD) * 3.5;

		logs.push_back(new Cylinder(model_id, glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), log_texture, 20));
		
		int index = angle / 30;
		z_rot = xrot[index];
		x_rot = zrot[index];
		glm::mat4 transform = glm::mat4(1);
		transform = glm::translate(transform, glm::vec3(position.x + x, position.y, position.z + z));
		transform = glm::rotate(transform, z_rot, glm::vec3(0, 0, 1));
		transform = glm::rotate(transform, x_rot, glm::vec3(1, 0, 0));
		transform = glm::scale(transform, glm::vec3(0.32, 1.5, 0.32));
		log_transformations.push_back(transform);
		count++;
	}
}

//Renders the whole bonfire
void Bonfire::Render()
{
	for (int i = 0; i < logs.size(); i++)
	{
		logs[i]->Render(log_transformations[i]);
	}
}