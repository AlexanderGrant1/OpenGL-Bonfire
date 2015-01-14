#pragma once

#include "wrapper_glfw.h"

/* Include GLM core and matrix extensions*/
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <vector>
class TextureLoader
{
private:
	GLuint program;
public:
	TextureLoader::TextureLoader(GLuint prog);
	GLuint Load(char* path);
	GLuint LoadBump(char* path);
};