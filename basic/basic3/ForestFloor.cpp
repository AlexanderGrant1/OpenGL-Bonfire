//Forest floor
//Consists of the shrubs in the scene
//Responsible for rendering the shrubs on the forest floor and their shadows

#include "ForestFloor.h"

ForestFloor::ForestFloor(GLuint modelID, GLuint shadowID, GLuint shrubTexture, glm::vec3 * terrainVertices, glm::vec3 * terrainNormals, GLuint xSize, GLuint zSize)
{
	model_id = modelID;
	shadow_id = shadowID;
	shrub_texture = shrubTexture;
	terrain_vertices = terrainVertices;
	terrain_normals = terrainNormals;
	x_size = xSize;
	z_size = zSize;
}

//Place the shrubs on the forest floor procedurally using the c++ rand function
void ForestFloor::Place()
{
	//Place shrubs
	for (GLuint x = x_size * 0.35; x < x_size * 0.75; x += x_size*0.1)
	{
		int min = 0;
		int max = 0;
		if (x == x_size * 0.35)
		{
			min = 80;
			max = 90;
		}
		else if (x == x_size * 0.45)
		{
			min = 75;
			max = 80;
		}
		else if (x >= x_size * 0.65)
		{
			min = 80;
			max = 85;
		}
		else
		{
			min = 90;
			max = 95;
		}
		int top_index = min + (rand() % (int)(max - min + 1));
		shrubs.push_back(new Shrub(model_id, terrain_vertices[(x*x_size) + top_index], terrain_normals[(x*x_size) + top_index], shrub_texture));

		int min2 = 0;
		int max2 = 0;

		if (x == x_size * 0.35)
		{
			min2 = 100;
			max2 = 110;
		}
		else if (x >= x_size * 0.65)
		{
			min2 = 100;
			max2 = 105;
		}
		else
		{
			min2 = 110;
			max2 = 115;
		}
		int bottom_index = min2 + (rand() % (int)(max2 - min2 + 1));
		shrubs.push_back(new Shrub(model_id, terrain_vertices[(x*x_size) + bottom_index], terrain_normals[(x*x_size) + top_index], shrub_texture));
	}

	for (GLuint x = x_size * 0.35; x < x_size * 0.75; x += x_size*0.1)
	{
		int min = 90;
		int max = 100;
		int top_index = min + (rand() % (int)(max - min + 1));
		glm::vec3 shrub_pos = terrain_vertices[(x*x_size) + top_index];
		

		int min2 = 100;
		int max2 = 110;

		int bottom_index = min2 + (rand() % (int)(max2 - min2 + 1));
		shrubs.push_back(new Shrub(model_id, terrain_vertices[(x*x_size) + bottom_index], terrain_normals[(x*x_size) + bottom_index], shrub_texture));
	}

	for (int i = 0; i < shrubs.size(); i++)
	{
		shrubs[i]->Generate(1, 0);
	}
}
//Render all shrubs in the forest
void ForestFloor::Render()
{
	for (int i = 0; i < shrubs.size(); i++)
	{
		shrubs[i]->Draw();
	}
	glUniform1ui(shadow_id, 1);//Tell fragment shader that this is a shadow
	for (int i = 0; i < shrubs.size(); i++)
	{
		shrubs[i]->DrawShadows();
	}
	glUniform1ui(shadow_id, 0);
}