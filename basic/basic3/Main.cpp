/*
 Assignment 2 - Alexander Grant
 November 2014
*/

/* Link to static libraries, could define these as linker inputs in the project settings instead
if you prefer */
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glloadD.lib")
#pragma comment(lib, "opengl32.lib")

/* Include the header to the GLFW wrapper class which
   also includes the OpenGL extension initialisation*/
#include "wrapper_glfw.h"
#include <iostream>

#include "Cylinder.h"
#include "Skybox.h"
#include "TextureLoader.h"
#include "Camera.h"
#include "terrain_object.h"
/* Include GLM core and matrix extensions*/
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "InstancedForest.h"
#include "SmokeParticleEffect.h"
#include "FireParticleEffect.h"
#include "Shrub.h"
#include "ForestFloor.h"
#include "Bonfire.h"
#include "wglext.h"
/* Define buffer object indices */
GLuint elementbuffer;

GLuint program, particle_program;		/* Identifier for the shader prgoram */
GLuint vao;			/* Vertex array (Containor) object. This is the index of the VAO that will be the container for
					   our buffer objects */

GLfloat light_x, light_y, light_z;

/* Uniforms*/
GLuint modelID, viewID, projectionID, lightposID, normalmatrixID, applyLightingID, instancedID, is_shadowID, flickerID, skyboxID, skyboxViewID, bumpID, bumpTexture, brightnessID;

Skybox* skybox;

terrain_object* terrain;

SmokeParticleEffect* smoke_particle_effect;
FireParticleEffect* fire_particle_effect;
InstancedForest* iforest;
ForestFloor* forest_floor;
Camera* camera;
Bonfire* bonfire;
GLuint frames;
GLuint bumpVal;
GLuint brightnessVal;
/*
This function is called before entering the main rendering loop.
Use it for all your initialisation stuff
*/
void init(GLWrapper *glw)
{

	light_x = 0; light_y = 0; light_z = -5.5;

	// Generate index (name) for one vertex array object
	glGenVertexArrays(1, &vao);

	// Create the vertex array object and make it current
	glBindVertexArray(vao);

	/* Load and build the vertex and fragment shaders */
	try
	{
		particle_program = glw->LoadShader("particle_object.vert", "particle_object.frag", "particle_geometry.glsl");
		program = glw->LoadShader("vertexshader.vert", "fragmentshader.frag");
	}
	catch (std::exception &e)
	{
		std::cout << "Caught exception: " << e.what() << std::endl;
		std::cin.ignore();
		exit(0);
	}

	TextureLoader textureLoader = TextureLoader(program);

	//Load in textures using the SOIL library
	GLuint skyboxTexture = textureLoader.Load("skybox.png");
	GLuint forestTexture = textureLoader.Load("forest_.png");
	GLuint forestBump = textureLoader.LoadBump("forest_bump.png");
	GLuint barkTexture = textureLoader.Load("pine.png");
	GLuint fireTexture = textureLoader.Load("fire_particle.png");
	GLuint wood_burn = textureLoader.Load("black.png");

	//Load the models
	Lib3dsFile* treeModel = lib3ds_file_load("Tree1.3ds");
	if (!treeModel)
	{
		throw("Failed to load the tree model");
	}

	/* Define uniforms to send to vertex shader */
	modelID = glGetUniformLocation(program, "model");
	viewID = glGetUniformLocation(program, "view");
	projectionID = glGetUniformLocation(program, "projection");
	lightposID = glGetUniformLocation(program, "lightpos");
	normalmatrixID = glGetUniformLocation(program, "normalmatrix");
	applyLightingID = glGetUniformLocation(program, "applyLighting");
	instancedID = glGetUniformLocation(program, "instanced");
	is_shadowID = glGetUniformLocation(program, "is_shadow");
	flickerID = glGetUniformLocation(program, "flicker");
	skyboxID = glGetUniformLocation(program, "skybox");
	skyboxViewID = glGetUniformLocation(program, "skyboxView");
	bumpID = glGetUniformLocation(program, "is_bump");
	bumpTexture = glGetUniformLocation(program, "bump_texture");
	brightnessID = glGetUniformLocation(program, "brightness");
	//Skybox setup
	skybox = new Skybox(skyboxTexture, modelID);
	camera = new Camera(viewID, projectionID, glm::vec3(0, 14.7, 80), glm::vec3(light_x, light_y - 2, light_x));
	//Terrain setup
	terrain = new terrain_object(modelID, forestTexture, forestBump);
	GLuint xSize = 200;
	GLuint zSize = 200;
	terrain->createTerrain(xSize, zSize, 200, 200);
	terrain->createObject();
	//Forest setup
	iforest = new InstancedForest(modelID, treeModel, barkTexture, terrain->vertices, 200, 200);
	iforest->CreateTreeVBO();
	iforest->Plant();
	//Particle effect setup
	fire_particle_effect = new FireParticleEffect(fireTexture, glm::vec3(0, -13, 5));
	fire_particle_effect->create(particle_program);
	//smoke_particle_effect = new SmokeParticleEffect(fireTexture, glm::vec3(0, 0, 5));
	//smoke_particle_effect->create(particle_program);
	//Forest floor setup
	forest_floor = new ForestFloor(modelID,is_shadowID,barkTexture,terrain->vertices,terrain->normals,xSize,zSize);
	forest_floor->Place();
	bonfire = new Bonfire(modelID, glm::vec3(0, -13, -5), wood_burn);
	bonfire->Initialise();
	bumpVal = 0;
	brightnessVal = 350;
}

/* Called to update the display. Note that this function is called in the event loop in the wrapper
   class because we registered display as a callback function */
void display()
{
	/* Define the background colour */
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	/* Clear the colour and frame buffers */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Enable depth test  */
	glEnable(GL_DEPTH_TEST);

	/* Make the compiled shader program current */
	glUseProgram(program);

	if (frames % 5 == 0)
	{
		glUniform1ui(flickerID, 1);
	}
	else
	{
		glUniform1ui(flickerID, 0);
	}

	// Define the light position and transform by the view matrix
	glm::vec4 lightpos = camera->GetView() * glm::vec4(light_x,light_y,light_z, 1.0);

	camera->Update();
	bonfire->Render();
	glUniform1ui(brightnessID, brightnessVal);
	glUniform1ui(skyboxID, 1);
	glUniformMatrix4fv(skyboxViewID, 1, GL_FALSE, &camera->GetSkyboxView()[0][0]);
	skybox->Render();
	glUniform1ui(skyboxID, 0);
	glUniform4fv(lightposID, 1, glm::value_ptr(lightpos));
	glUniform1ui(applyLightingID, 1);
	glUniform1ui(instancedID, 1);
	iforest->RenderTrees();
	glUniform1ui(instancedID, 0);
	glUniform1ui(bumpID, bumpVal);
	terrain->drawObject(0);
	glUniform1ui(bumpID, 0);
	forest_floor->Render();
	glUniform1ui(applyLightingID, 0);
	fire_particle_effect->drawParticles(camera->GetProjection(), camera->GetView());
	//smoke_particle_effect->drawParticles(camera->GetProjection(), camera->GetView());
	glUseProgram(0);
	frames++;
}

/* Called whenever the window is resized. The new window size is given, in pixels. */
static void reshape(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	camera->SetAspectRatio(((float)w / 640.f*4.f) / ((float)h / 480.f*3.f));
}

//I didn't write this function
void setVSync(bool sync)
{
	typedef BOOL(APIENTRY *PFNWGLSWAPINTERVALPROC)(int);
	PFNWGLSWAPINTERVALPROC wglSwapIntervalEXT = 0;
	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALPROC)wglGetProcAddress("wglSwapIntervalEXT");

	if (wglSwapIntervalEXT)
		wglSwapIntervalEXT(sync);
}

/* change view angle, exit upon ESC */
static void keyCallback(GLFWwindow* window, int key, int s, int action, int mods)
{
	/* Enable this call if you want to disable key responses to a held down key*/

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);


	if (key == 'W')
	{
		camera->MoveForward();
	}

	if (key == 'S')
	{
		camera->MoveBackward();
	}

	if (key == 'A')
	{
		camera->RotateLeft();
	}

	if (key == 'D')
	{
		camera->RotateRight();
	}

	if (key == ' ')
	{
		camera->MoveUp();
	}

	if (key == 'X')
	{
		camera->MoveDown();
	}

	if (key == 'M')
	{
		camera->Lookat(camera->GetLookAtPosition() + glm::vec3(0, 1, 0));
	}

	if (key == 'N')
	{
		camera->Lookat(camera->GetLookAtPosition() + glm::vec3(0, -1, 0));
	}

	if (key == '0')
	{
		bumpVal = 1;
	}
	if (key == '9')
	{
		bumpVal = 0;
	}

	if (key == '-')
	{
		if (brightnessVal > 100)
		{
			brightnessVal -= 25;//Decrease intensity of light to view less of the scene
		}
	}
	if (key == '=')
	{
		if (brightnessVal < 600)
		{
			brightnessVal += 25;//Increase intensity of light to view more of the scene
		}
	}
}

/* Entry point of program */
int main(int argc, char* argv[])
{
	GLWrapper *glw = new GLWrapper(1024, 768, "AC41001 Assignment 2 - Alexander Grant");
	if (!ogl_LoadFunctions())
	{
		fprintf(stderr, "ogl_LoadFunctions() failed. Exiting\n");
		return 0;
	}
	//setVSync(true);
	std::cout << "----- Controls -----" << std::endl;
	std::cout << "Move Camera Forward: W" << std::endl;
	std::cout << "Move Camera Backwards: S" << std::endl;
	std::cout << "Move Camera up: Spacebar" << std::endl;
	std::cout << "Move Camera down: X" << std::endl;
	std::cout << "Look up: M" << std::endl;
	std::cout << "Look down: N" << std::endl;
	std::cout << std::endl;
	std::cout << "Bump mapping is disabled by default since it significantly darkens the scene" << std::endl;
	std::cout << "Enable bump mapping on terrain: 0" << std::endl;
	std::cout << "Disable bump mapping on terrain: 9" << std::endl;
	std::cout << std::endl;
	std::cout << "It is hard to create a scene with low level lighting that looks consistent across monitors (due to different monitor brightness settings) so I have included light intensity controls." << std::endl;
	std::cout << "Increase light intensity: -" << std::endl;
	std::cout << "Decrease light intensity =" << std::endl;
	std::cout << std::endl;
	std::cout << "Bump mapping is disabled by default since it significantly darkens the scene" << std::endl;
	std::cout << std::endl;

	glw->setRenderer(display);
	glw->setKeyCallback(keyCallback);
	glw->setReshapeCallback(reshape);
	init(glw);
	glw->eventLoop();

	delete(glw);
	return 0;
}