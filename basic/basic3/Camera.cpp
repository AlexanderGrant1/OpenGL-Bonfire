/*
The camera used for looking around the scene
*/

#include "wrapper_glfw.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Camera.h"
#include <iostream>
#include <vector>

Camera::Camera(GLuint viewID, GLuint projectionID, glm::vec3 startPosition, glm::vec3 startLookAtPosition)
{
	lookAtPosition = startLookAtPosition;
	position = startPosition;
	view_id = viewID;
	projection_id = projectionID;
	x_rotation = 0;
	y_rotation = 0;
	aspect_ratio = 1.333f;
	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	projection = glm::perspective(30.0f, aspect_ratio, 0.1f, 10000.0f);

	// Camera matrix
	view = glm::lookAt(
		position, // Camera position
		glm::vec3(0,0,0), // Lookat position
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);
}

//Pans the camera up
void Camera::MoveUp()
{
	if (position.y < 100)
	{
		position.y += 2.5f;
	}
}

//Pans the camera down
void Camera::MoveDown()
{
	if (position.y >= 18)
	{
		position.y -= 2.5f;
	}
}

//Moves the camera forward
void Camera::MoveForward()
{
	if (position.z > 52)
	{
		position.z -= 0.5f;
	}
}

//Moves the camera backward
void Camera::MoveBackward()
{
	if (position.z < 100)
	{
		position.z += 0.5f;
	}
}

//Rotates the camera left
void Camera::RotateLeft()
{
	x_rotation += 1;
}

//Rotates the camera right
void Camera::RotateRight()
{
	x_rotation -= 1;
}

//Returns the camera's projection matrix
glm::mat4 Camera::GetProjection()
{
	return projection;
}

//Returns the camera's view matrix
glm::mat4 Camera::GetView()
{
	return view;
}

//Used for the skybox, doesn't take into account camera rotation so that the skybox doesn't rotate with the camera
glm::mat4 Camera::GetSkyboxView()
{
	glm::mat4 skybox = glm::mat4(1);
	skybox = glm::lookAt(
		position, // Camera is at (0,0,4), in World Space
		lookAtPosition, // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);
	return skybox;
}

//Returns the aspect ratio
GLfloat Camera::GetAspectRatio()
{
	return aspect_ratio;
}

//Sets the camera's aspect ratio
void Camera::SetAspectRatio(GLfloat aspectRatio)
{
	aspectRatio = aspectRatio;
}

glm::vec3 Camera::GetLookAtPosition()
{
	return lookAtPosition;
}

glm::vec3 Camera::GetCameraPosition()
{
	return position;
}

void Camera::Lookat(glm::vec3 position)
{
	lookAtPosition = position;
}

//Refreshes the view
void Camera::Update()
{
	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	projection = glm::perspective(30.0f, aspect_ratio, 0.1f, 10000.0f);
	//system("cls");
	//std::cout << "camera pos: " << position.x << ", " << position.y << ", " << position.z << std::endl;
	//std::cout << "lookat: " << lookAtPosition.x << ", " << lookAtPosition.y << ", " << lookAtPosition.z << std::endl;
	// Camera matrix
	view = glm::lookAt(
		position, // Camera is at (0,0,4), in World Space
		lookAtPosition, // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);

	view = glm::rotate(view, x_rotation, glm::vec3(0, 1, 0)); //rotating in clockwise direction around y-axis

	glUniformMatrix4fv(view_id, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(projection_id, 1, GL_FALSE, &projection[0][0]);
}