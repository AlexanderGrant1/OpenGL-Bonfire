#pragma once

#include "wrapper_glfw.h"
#include <glm/glm.hpp>
#include "Particle.h"

class SmokeParticleEffect
{
public:
	SmokeParticleEffect(GLuint smokeTexture, glm::vec3 pos);
	~SmokeParticleEffect();

	void create(GLuint program);
	int FindUnusedParticle();
	void SortParticles();
	void drawParticles(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix);
	void defineUniforms();
	void Setup();
	GLuint particles_position_buffer;
	const int MaxParticles = 500;
	Particle ParticlesContainer[500];
	int LastUsedParticle;
	GLfloat* particle_positions;
	int ParticlesCount;
	double lastTime;

	glm::vec3 position;

	GLuint VertexArrayID;
	GLuint programID;
	GLuint fire_texture;
	GLuint TextureID;
	GLuint ViewProjMatrixID;
	GLuint alphaID;
	GLuint cullValueID;
};

