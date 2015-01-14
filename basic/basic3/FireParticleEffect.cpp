//Represents the main bonfire particle effect
//I *heavily* adapted the particle effect class put onto blackboard in both the shaders and application code.
//Passes points to the geometry shader which are then turned into triangle strips which give the square appearance
//The geometry shader defines the texture coordinates for the texturing
//The fragment shader then does some pixel fragment culling based on alpha values of textures

#include "FireParticleEffect.h"
#include <algorithm>
#include <glm/gtx/norm.hpp>
#include "soil.h"
#include "Particle.h"
FireParticleEffect::FireParticleEffect(GLuint fireTexture, glm::vec3 pos)
{
	LastUsedParticle = 0;
	fire_texture = fireTexture;
	position = pos;
}


FireParticleEffect::~FireParticleEffect()
{
}

void FireParticleEffect::SortParticles(){
	std::sort(&ParticlesContainer[0], &ParticlesContainer[MaxParticles]);
}


void FireParticleEffect::create(GLuint program)
{
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	programID = program;
	glUseProgram(programID);

	particle_positions = new GLfloat[MaxParticles * 4];

	for (int i = 0; i<MaxParticles; i++){
		ParticlesContainer[i].life = -1.0f;
		ParticlesContainer[i].cameradistance = -1.0f;
	}

	glGenBuffers(1, &particles_position_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	GLuint ViewProjMatrixID = glGetUniformLocation(programID, "VP");

	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	/* Define the uniform variables */
	defineUniforms();

	lastTime = glfwGetTime();
}


/* Update the particle animation and draw the particles */
void FireParticleEffect::drawParticles(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix)
{
	double currentTime = glfwGetTime();
	double delta = currentTime - lastTime;
	lastTime = currentTime;

	glBindVertexArray(VertexArrayID);

	glUniform1f(alphaID, 1);
	glUniform1f(cullValueID, 0.4);

	// We will need the camera's position in order to sort the particles
	// w.r.t the camera's distance.
	glm::vec3 CameraPosition(glm::inverse(ViewMatrix)[3]);
	glm::mat4 ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;

	// Generate 10 new particule each millisecond,
	// but limit this to 16 ms (60 fps), or if you have 1 long frame (1sec),
	// newparticles will be huge and the next frame even longer.
	int newparticles = (int)(delta*10000.0);
	if (newparticles > (int)(0.016f*10000.0))
		newparticles = (int)(0.005f*10000.0);

	for (int i = 0; i<newparticles; i++){
		int particleIndex = FindUnusedParticle();
		ParticlesContainer[particleIndex].life = 1; // This particle will live for 1 second.
		ParticlesContainer[particleIndex].pos = glm::vec3(0, 0, -10.0f);

		float spread = 3.5;
		glm::vec3 maindir = glm::vec3(0.0f, -10.0f, 0.0f);
		// This part creates the wavy effect the fire has
		glm::vec3 randomdir = glm::vec3(
			(rand() % 2000 - 1000.0f) / 1000.0f,
			(rand() % 2000 - 1000.0f) / 1000.0f,
			(rand() % 2000 - 1000.0f) / 1000.0f
			);
		ParticlesContainer[particleIndex].pos += glm::vec3(position.x + (randomdir.x * spread), position.y, position.z + (randomdir.z * spread));
		//The line above causes the fire effect.  I modelled this on a cone because of the bonfire's shape.  Basically the particles at the centre of the cone move the fastest
		//and the particles closer to the edges move much slower which gives the cone-like shape.  I think this looks quite realistic for this scene
		ParticlesContainer[particleIndex].speed = glm::vec3(0, 12 - (abs(randomdir.x) + abs(randomdir.z)) * 9, 0);
	}

	// Simulate all particles
	int ParticlesCount = 0;
	for (int i = 0; i<MaxParticles; i++){

		Particle& p = ParticlesContainer[i]; // shortcut

		if (p.life > 0.0f){

			// Decrease life
			p.life -= delta;
			if (p.life > 0.0f){

				p.pos += p.speed * ((float)delta);
				p.cameradistance = glm::length2(p.pos - CameraPosition);

				// Fill the GPU buffer
				particle_positions[4 * ParticlesCount + 0] = p.pos.x;
				particle_positions[4 * ParticlesCount + 1] = p.pos.y;
				particle_positions[4 * ParticlesCount + 2] = p.pos.z;

			}
			else{
				// Particles that just died will be put at the end of the buffer in SortParticles();
				p.cameradistance = -1.0f;
			}

			ParticlesCount++;
		}
	}

	SortParticles();

	// Update the buffers that OpenGL uses for rendering.
	// There are much more sophisticated means to stream data from the CPU to the GPU, 
	// but this is outside the scope of this tutorial.
	// http://www.opengl.org/wiki/Buffer_Object_Streaming

	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLfloat)* 4, particle_positions);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Use our shader
	glUseProgram(programID);

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fire_texture);
	// Set our "myTextureSampler" sampler to user Texture Unit 0
	glUniform1i(TextureID, 0);

	glUniformMatrix4fv(ViewProjMatrixID, 1, GL_FALSE, &ViewProjectionMatrix[0][0]);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		4,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	glDrawArrays(GL_POINTS, 0, ParticlesCount);

	glDisable(GL_BLEND);
}


void FireParticleEffect::defineUniforms()
{
	glBindVertexArray(VertexArrayID);
	glUseProgram(programID);
	ViewProjMatrixID = glGetUniformLocation(programID, "VP");
	alphaID = glGetUniformLocation(programID, "alpha");
	cullValueID = glGetUniformLocation(programID, "cull_value");
	// fragment shader
	TextureID = glGetUniformLocation(programID, "myTextureSampler");
}


int FireParticleEffect::FindUnusedParticle()
{
	for (int i = LastUsedParticle; i<MaxParticles; i++){
		if (ParticlesContainer[i].life < 0){
			LastUsedParticle = i;
			return i;
		}
	}

	for (int i = 0; i<LastUsedParticle; i++){
		if (ParticlesContainer[i].life < 0){
			LastUsedParticle = i;
			return i;
		}
	}
	return 0; // All particles are taken, override the first one
}