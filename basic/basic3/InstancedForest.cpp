//This class renders the trees in the scene using instancing
//Shadows are not used for these trees since they have 240,000 vertices and even instancing can't help with that very much
//Forest loading code taken from http://www.donkerdump.nl/node/207
#include "InstancedForest.h"
#include <iostream>

InstancedForest::InstancedForest(GLuint modelID, Lib3dsFile * treeModel, GLuint treeTexture, glm::vec3 * terrainVertices, GLuint xsize, GLuint zsize)
{
	tree_model = treeModel;
	tree_texture = treeTexture;
	model_id = modelID;
	terrain_vertices = terrainVertices;
	x_size = xsize;
	z_size = zsize;
	CalculateTreeCount();
}

InstancedForest::~InstancedForest()
{

}
//8 trees
void InstancedForest::CalculateTreeCount()
{
	tree_count = 8;
}
//Populates total_tree_faces with the number of faces in the tree
void InstancedForest::GetTreeFaces()
{

	total_tree_faces = 0;
	Lib3dsMesh * mesh;

	// Loop through every mesh
	for (mesh = tree_model->meshes; mesh != NULL; mesh = mesh->next)
	{
		// Add the number of faces this mesh has to the total faces
		total_tree_faces += mesh->faces;
	}
}

// Copy vertices and normals to the memory of the GPU
void InstancedForest::CreateTreeVBO()
{

	// Calculate the number of faces we have in total
	GetTreeFaces();

	// Allocate memory for our vertices and normals
	Lib3dsVector * vertices = new Lib3dsVector[total_tree_faces * 3];
	Lib3dsVector * normals = new Lib3dsVector[total_tree_faces * 3];
	Lib3dsTexel * texCoords = new Lib3dsTexel[total_tree_faces * 3];

	Lib3dsMesh * mesh;
	unsigned int FinishedFaces = 0;
	// Loop through all the meshes
	for (mesh = tree_model->meshes; mesh != NULL; mesh = mesh->next)
	{
		lib3ds_mesh_calculate_normals(mesh, &normals[FinishedFaces * 3]);
		// Loop through every face
		for (unsigned int cur_face = 0; cur_face < mesh->faces; cur_face++)
		{
			Lib3dsFace * face = &mesh->faceL[cur_face];
			for (unsigned int i = 0; i < 3; i++)
			{
				memcpy(&vertices[FinishedFaces * 3 + i], mesh->pointL[face->points[i]].pos, sizeof(Lib3dsVector));
				memcpy(&texCoords[FinishedFaces * 3 + i][0], mesh->texelL[face->points[i]], sizeof(Lib3dsTexel));
			}
			FinishedFaces++;
		}
	}

	// Generate a Vertex Buffer Object and store it with our vertices
	glGenBuffers(1, &position_buffer_object);
	glBindBuffer(GL_ARRAY_BUFFER, position_buffer_object);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Lib3dsVector) * 3 * total_tree_faces * tree_count, NULL, GL_STATIC_DRAW);//add in tree count

	// Generate another Vertex Buffer Object and store the normals in it
	glGenBuffers(1, &normals_buffer_object);
	glBindBuffer(GL_ARRAY_BUFFER, normals_buffer_object);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Lib3dsVector) * 3 * total_tree_faces, normals, GL_STATIC_DRAW);

	glGenBuffers(1, &texture_buffer_object);
	glBindBuffer(GL_ARRAY_BUFFER, texture_buffer_object);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Lib3dsTexel) * 3 * total_tree_faces, texCoords, GL_STATIC_DRAW);

	//Vertex buffer object used for instancing
	glGenBuffers(1, &vertex_buffer_object);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
	glBufferData(GL_ARRAY_BUFFER, (sizeof(Lib3dsVector) * 3 * total_tree_faces), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &instance_transformation_buffer_object);
	glBindBuffer(GL_ARRAY_BUFFER, instance_transformation_buffer_object);
	glBufferData(GL_ARRAY_BUFFER, (sizeof(glm::mat4)* tree_count), NULL, GL_STATIC_DRAW);

	// Clean up our allocated memory
	delete vertices;
	delete normals;
	delete texCoords;
}
//Places the trees at points on the terrain height map
void InstancedForest::Plant()
{
	GLuint num_vertices = x_size * z_size;
	GLuint middle_col = x_size / 2;
	GLuint middle_row = z_size / 2;
	int random_row = 0;

	for (GLuint x = x_size * 0.35; x < x_size * 0.75; x+=x_size*0.1)
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
			min = 60;
			max = 80;
		}
		else if (x >= x_size * 0.65)
		{
			min = 80;
			max = 90;
		}
		else
		{
			min = 70;
			max = 80;
		}
		int top_index = min + (rand() % (int)(max - min + 1));
		tree_positions.push_back(terrain_vertices[(x*x_size) + top_index]);

		int min2 = 0;
		int max2 = 0;

		if (x == x_size * 0.35)
		{
			min2 = 100;
			max2 = 110;
		}
		else if (x >= x_size * 0.65)
		{
			min2 = 110;
			max2 = 130;
		}
		else
		{
			min2 = 110;
			max2 = 125;
		}
		int bottom_index = min2 + (rand() % (int)(max2 - min2 + 1));
		tree_positions.push_back(terrain_vertices[(x*x_size) + bottom_index]);
	}

}

//Renders all of the trees using instancing.  Transformations are applied to each tree
void InstancedForest::RenderTrees()
{
	glBindBuffer(GL_ARRAY_BUFFER, position_buffer_object);
	glBufferData(GL_ARRAY_BUFFER, tree_count * sizeof(glm::vec3), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, tree_count * sizeof(glm::vec3), &tree_positions[0]);


	glm::mat4 * instance_transforms = new glm::mat4[tree_count];
	for (int i = 0; i < tree_count; i++)
	{
		glm::mat4 transform = glm::mat4(1);
		transform = glm::translate(transform, tree_positions[i]);
		transform = glm::rotate(transform, -90.f, glm::vec3(1, 0, 0));
		instance_transforms[i] = transform;
	}

	glBindBuffer(GL_ARRAY_BUFFER, instance_transformation_buffer_object);
	glBufferData(GL_ARRAY_BUFFER, tree_count * sizeof(glm::mat4), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, tree_count * sizeof(glm::mat4), instance_transforms);

	glm::mat4 model = glm::mat4(1.0f);
	glUniformMatrix4fv(model_id, 1, GL_FALSE, &model[0][0]);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tree_texture);

	glBindBuffer(GL_ARRAY_BUFFER, position_buffer_object);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, normals_buffer_object);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, texture_buffer_object);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, instance_transformation_buffer_object);

	//http://www.informit.com/articles/article.aspx?p=2033340&seqNum=5 reference for passing a mat4 to the vertex shader
	for (int i = 0; i < 4; i++)
	{
		// Set up the vertex attribute
		glVertexAttribPointer(4 + i,              // Location
			4, GL_FLOAT, GL_FALSE,       // vec4
			sizeof(glm::mat4),                // Stride
			(void *)(sizeof(glm::vec4)* i)); // Start offset
		glEnableVertexAttribArray(4 + i);
		glVertexAttribDivisor(4 + i, 1);
	}

	glVertexAttribDivisor(0, 1); // positions - position changes with each draw which is why the second argument is 1
	glVertexAttribDivisor(1, 0); // normals - same normals for each tree
	glVertexAttribDivisor(2, 0); // texture - same texture for each tree
	glVertexAttribDivisor(3, 0); // vertices - same vertices for each tree

	glDrawArraysInstanced(GL_TRIANGLES, 0, total_tree_faces * 3, tree_count);//render all the trees using instancing
	glVertexAttribDivisor(0, 0); //reset, 0,1 causes problems with drawArrays in other parts of the program
	glVertexAttribDivisor(4, 0); // vertices - same vertices for each tree
}