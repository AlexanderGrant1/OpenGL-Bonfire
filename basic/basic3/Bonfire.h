#pragma once

#include "wrapper_glfw.h"

/* Include GLM core and matrix extensions*/
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Cylinder.h"
class Bonfire
{
private:
	GLuint model_id;
	glm::vec3 position;
	GLuint log_texture;
	std::vector<Cylinder*> logs;
	std::vector<glm::mat4> log_transformations;
public:
	Bonfire(GLuint modelId, glm::vec3 bonfirePosition, GLuint logTexture);
	void Initialise();
	void Render();
};