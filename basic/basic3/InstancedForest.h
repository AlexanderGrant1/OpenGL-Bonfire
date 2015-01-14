#pragma once

#include "wrapper_glfw.h"

/* Include GLM core and matrix extensions*/
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "Lib3ds/mesh.h"
#include "Lib3ds/file.h"
#include <vector>
class InstancedForest
{
private:
	GLuint x_size;
	GLuint z_size;
	GLuint tree_texture;
	GLuint model_id;
	GLuint position_buffer_object, vertex_buffer_object, normals_buffer_object, texture_buffer_object, instance_transformation_buffer_object;
	GLuint total_tree_faces;
	GLuint tree_count;
	glm::vec3 * terrain_vertices;
	Lib3dsFile* tree_model;
	std::vector<glm::vec3> tree_positions;
	void GetTreeFaces();
	void CalculateTreeCount();
	bool isMiddleSection(int row, int col, int middle_col, int middle_row);
	glm::vec3* ConvertToOpenGLCoordinates(glm::vec3* vertices);
public:
	InstancedForest(GLuint modelID, Lib3dsFile * treeModel, GLuint treeTexture, glm::vec3 * terrainVertices, GLuint xsize, GLuint zsize);
	~InstancedForest();
	void Plant();
	void RenderTrees();
	void CreateTreeVBO();
};