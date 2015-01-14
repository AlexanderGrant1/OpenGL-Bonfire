#pragma once

#include "wrapper_glfw.h"
#include <glm/glm.hpp>
#include "Particle.h"

class FireParticleEffect
{
public:
	FireParticleEffect(GLuint fireTexture, glm::vec3 pos);
	~FireParticleEffect();

	void create(GLuint program);
	int FindUnusedParticle();
	void SortParticles();
	void drawParticles(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix);
	void defineUniforms();
	void Setup();
	GLuint particles_position_buffer;
	const int MaxParticles = 2000;
	Particle ParticlesContainer[2000];
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

