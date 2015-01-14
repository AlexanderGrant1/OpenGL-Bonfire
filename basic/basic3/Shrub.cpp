//Represents a shrub in the scene
//Shadows supported for this object.
//For this code I adapted your 2D L system code and created my own grammar operators(?) to support 3D
//3D is done using cylinders instead of lines
//I also changed this code to make it more efficient.  The shrub only needs to be generated once.

#include "Shrub.h"
#include <iostream>
#include "Shadow.h"

Shrub::Shrub(GLuint modelID, glm::vec3 shrubPosition, glm::vec3 terrainNormal, GLuint texture)
{
	model_id = modelID;
	position = shrubPosition;
	terrain_normal = terrainNormal;
	x = 0;
	y = -0.5f;
	texture_id = texture;
	lstring = "F[zFxFzF][xFxFzF][+F+F-F][-F-F+F]\0";
	Initialise();
}

Shrub::~Shrub()
{

}
//Initialises the shrub with a position and scale
//Also initialises the shrub's shadow transformations with a scale and position
void Shrub::Initialise()
{
	//Main model

	glm::mat4 initial = glm::mat4(1);
	initial = glm::translate(initial, position);
	initial = glm::scale(initial, glm::vec3(0.1, 0.1, 0.1));
	pen.push(initial);

	//Shadow

	Shadow* shadow = new Shadow();

	glm::mat4 shadowMatrix = glm::mat4(1);
	shadow->shadow_matrix(glm::vec4(position,0),glm::vec4(terrain_normal,0), shadowMatrix);

	glm::mat4 shadow_initial = glm::mat4(1);
	shadow_initial = glm::translate(shadow_initial, position + glm::vec3(0,0.3,0));
	shadow_initial = glm::scale(shadow_initial, glm::vec3(0.1, 0.1, 0.1));
	shadow_initial *= shadowMatrix;
	shadow_pen.push(shadow_initial);
}
//Draws the shrub
void Shrub::Draw()
{
	for (int i = 0; i < parts.size(); i++)
	{
		parts[i]->Render(transformations[i]);
	}
}
//Draws shadows for the parts of the shrub
void Shrub::DrawShadows()
{
	for (int i = 0; i < parts.size(); i++)
	{
		parts[i]->DrawShadow(shadow_transformations[i]);
	}
}

void Shrub::addBranch(int l)
{
	glm::mat4 transform = pen.top();
	transformations.push_back(transform);
	Cylinder* cylinder = new Cylinder(model_id, glm::vec3(0, 0, 0), glm::vec3(0.1, 1, 0.1), texture_id, 10);
	parts.push_back(cylinder);


	glm::mat4 shadow_transform = shadow_pen.top();
	shadow_transformations.push_back(shadow_transform);
}
//Generates a shrub using the grammar defined in the constructor
void Shrub::Generate(int level, int leaves) {
	int current;
	current = 0;
	while (lstring[current])
	{
		switch (lstring[current])
		{
		case 'F':

			if (level == 0)
			{
				if (leaves && ((lstring[current + 1] == ']') || (lstring[current + 1] == 0)))
					addBranch(0);
				else
					addBranch(1);

				pen.top() = glm::translate(pen.top(), glm::vec3(0, 4.5f, 0));
				shadow_pen.top() = glm::translate(shadow_pen.top(), glm::vec3(0, 4.5f, 0));
			}
			else
			{
				if ((lstring[current + 1] == ']') || (lstring[current + 1] == 0))
					Generate(level - 1, leaves);
				else
					Generate(level - 1, 0);
			}

			break;
		case '[':
			pen.push(glm::scale(pen.top(), glm::vec3(0.7, 0.7, 0.7)));
			shadow_pen.push(glm::scale(shadow_pen.top(), glm::vec3(0.7, 0.7, 0.7)));
			break;
		case ']':
			pen.pop();
			shadow_pen.pop();
			break;
		case 'z'://'+' equivalent for the z axis
			// rotate 45 degrees
			pen.top() = glm::rotate(pen.top(), -45.f + x, glm::vec3(1.f, 0, 0));
			shadow_pen.top() = glm::rotate(shadow_pen.top(), -45.f + x, glm::vec3(1.f, 0, 0));
			break;
		case 'x'://'-' equivalent for the z axis
			// rotate 45 degrees
			pen.top() = glm::rotate(pen.top(), 30 + x, glm::vec3(1.f, 0, 0));
			shadow_pen.top() = glm::rotate(shadow_pen.top(), 30 + x, glm::vec3(1.f, 0, 0));
			break;
		case '+':
			// rotate 45 degrees
			pen.top() = glm::rotate(pen.top(), -45.f + x, glm::vec3(0, 0, 1.f));
			shadow_pen.top() = glm::rotate(shadow_pen.top(), -45.f + x, glm::vec3(0, 0, 1.f));
			break;
		case '-':
			pen.top() = glm::rotate(pen.top(), 30.f + y, glm::vec3(0, 0, 1.f));
			shadow_pen.top() = glm::rotate(shadow_pen.top(), 30.f + y, glm::vec3(0, 0, 1.f));
			break;
		default:
			break;
		}
		current++;
	}
}


