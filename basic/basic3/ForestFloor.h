#pragma once

#include "wrapper_glfw.h"
#include <glm/glm.hpp>
#include <vector>
#include "Shrub.h"
class ForestFloor
{
private:
	std::vector<Shrub*> shrubs;
	GLuint x_size, z_size;
	glm::vec3 * terrain_vertices;
	glm::vec3 * terrain_normals;
	GLuint shadow_id, shrub_texture, model_id;
public:
	ForestFloor(GLuint modelID, GLuint shadowID, GLuint shrubTexture, glm::vec3 * terrainVertices, glm::vec3 * terrainNormals, GLuint xSize, GLuint zSize);
	void Render();
	void Place();
};

