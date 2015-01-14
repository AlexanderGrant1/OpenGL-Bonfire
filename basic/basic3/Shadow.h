#pragma once

#include "wrapper_glfw.h"

/* Include GLM core and matrix extensions*/
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
class Shadow
{
public:
	static void shadow_matrix(glm::vec4 position, glm::vec4 pl, glm::mat4 &shadow_proj);
};