#pragma once
/* terrain_object.h
   Example class to show how to create and render a height map
   Iain Martin November 2014
*/

#pragma once

#include "wrapper_glfw.h"
#include <vector>
#include <glm/glm.hpp>

class terrain_object
{
public:
	terrain_object(GLuint modelID, GLuint textureID, GLuint forestBumpTexture);
	~terrain_object();

	void calculateNoise(GLfloat freq, GLfloat scale);
	void createTerrain(GLuint xp, GLuint yp, GLfloat xs, GLfloat ys);
	void calculateNormals();
	void calculateTextureCoordinates();

	void createObject();
	void drawObject(int drawmode);

	glm::vec3 *vertices;
	glm::vec3 *normals;
	std::vector<GLuint> elements;
	std::vector<glm::vec2> tex_coords;
	GLfloat* noise;

	GLuint vbo_mesh_vertices;
	GLuint vbo_mesh_normals;
	GLuint vbo_tex_coords;
	GLuint ibo_mesh_elements;
	GLuint attribute_v_coord;
	GLuint attribute_v_normal;
	GLuint attribute_tex_coord;
	GLuint texture_id;
	GLuint bump_texture;

	GLuint xsize;
	GLuint zsize;
	GLfloat width;
	GLfloat height;
	GLuint perlin_octaves;
	GLfloat height_scale;
	GLuint model_id;
};

