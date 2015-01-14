/*
A collection of functions used for loading textures using the SOIL library
I used your multi texture code to understand how to load in more texture(s)
*/

#pragma import SOIL.lib
#include "TextureLoader.h"
#pragma comment(lib, "glloadD.lib")
#pragma comment(lib, "opengl32.lib")

#include "wrapper_glfw.h"
#include <iostream>
#include "SOIL.h"
/* Include GLM core and matrix extensions*/
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

TextureLoader::TextureLoader(GLuint prog)
{
	program = prog;
}

//Loads a texture and returns the texture ID
GLuint TextureLoader::Load(char* path)
{
	GLuint texID;
	//Taken directly from blackboard examples
	try
	{
		glActiveTexture(GL_TEXTURE0);

		texID = SOIL_load_OGL_texture(path, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);

		if (texID == 0)
		{
			printf("TexID SOIL loading error: '%s'\n", SOIL_last_result());
		}

		int loc = glGetUniformLocation(program, "tex1");
		if (loc >= 0) glUniform1i(loc, 0);
	}
	catch (std::exception &e)
	{
		printf("\nImage file loading failed.");
	}
	return texID;
}

//Used to load a secondary bump texture
GLuint TextureLoader::LoadBump(char* path)
{
	GLuint texID;
	//Taken directly from blackboard examples
	try
	{
		glActiveTexture(GL_TEXTURE1);

		texID = SOIL_load_OGL_texture(path, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);

		if (texID == 0)
		{
			printf("TexID SOIL loading error: '%s'\n", SOIL_last_result());
		}

		int loc = glGetUniformLocation(program, "bump_texture");
		if (loc >= 0) glUniform1i(loc, 1);
	}
	catch (std::exception &e)
	{
		printf("\nImage file loading failed.");
	}
	return texID;
}