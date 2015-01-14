#pragma once

#include "wrapper_glfw.h"

/* Include GLM core and matrix extensions*/
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
class Camera
{
private:
	glm::mat4 view;
	glm::mat4 projection;
	glm::vec3 position;
	glm::vec3 lookAtPosition;

	GLuint view_id;
	GLuint projection_id;

	GLfloat x_rotation;
	GLfloat y_rotation;
	GLfloat aspect_ratio;
public:
	Camera::Camera(GLuint viewID, GLuint projectionID, glm::vec3 startPosition, glm::vec3 startLookAtPosition);
	glm::mat4 GetView();
	glm::mat4 GetSkyboxView();
	glm::mat4 GetProjection();
	GLfloat GetAspectRatio();
	void SetAspectRatio(GLfloat aspectRatio);
	void MoveUp();
	void MoveDown();
	void MoveForward();
	void MoveBackward();
	void RotateLeft();
	void RotateRight();
	void Update();
	void Lookat(glm::vec3 position);
	glm::vec3 GetLookAtPosition();
	glm::vec3 GetCameraPosition();
};