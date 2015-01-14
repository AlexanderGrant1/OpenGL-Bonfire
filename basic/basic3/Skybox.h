#pragma once

#include "wrapper_glfw.h"

/* Include GLM core and matrix extensions*/
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <vector>
class Skybox
{
private:
	GLuint model_id, position_buffer_object, element_buffer, normals_buffer_object, texture_buffer_object;
	GLuint texID;
	glm::vec3 scale;
	glm::vec3 position;
public:
	Skybox::Skybox(GLuint skyboxTexture, GLuint modelId);
	void Render();
};