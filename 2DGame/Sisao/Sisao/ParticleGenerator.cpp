/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "particlegenerator.h"

ParticleGenerator::ParticleGenerator(ShaderProgram shader, Texture texture, unsigned int amount)
    : shader(shader), texture(texture), amount(amount)
{
    this->init();
}

void ParticleGenerator::Update(float dt, Player& object, unsigned int newParticles, glm::vec2 offset)
{
    // add new particles 
    for (unsigned int i = 0; i < newParticles; ++i)
    {
        int unusedParticle = this->firstUnusedParticle();
        this->respawnParticle(this->particles[unusedParticle], object, offset);
    }
    // update all particles
    for (unsigned int i = 0; i < this->amount; ++i)
    {
        Particle& p = this->particles[i];
        p.Life -= dt/100.f; // reduce life
        if (p.Life > 0.0f)
        {	// particle is alive, thus update
            p.Position -= p.Velocity * dt;
            p.Color.a -= dt * 2.5f;
        }
    }
}

void ParticleGenerator::easyUpdate(float dt)
{
  
    // update all particles
    for (unsigned int i = 0; i < this->amount; ++i)
    {
        Particle& p = this->particles[i];
        p.Life -= dt / 100.f; // reduce life
        if (p.Life > 0.0f)
        {	// particle is alive, thus update
            p.Position -= p.Velocity * dt;
            p.Color.a -= dt * 2.5f;
        }
    }
}

// render all particles
void ParticleGenerator::Draw(glm::mat4 proj)
{
    // use additive blending to give it a 'glow' effect
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    this->shader.use();
    for (Particle particle : this->particles)
    {
        if (particle.Life > 0.0f)
        {
            glm::mat4 projection = proj;
            this->shader.setUniformMatrix4f("projection", projection);
            this->shader.setUniform2f("offset", particle.Position.x,particle.Position.y);
            this->shader.setUniform4f("color", 0.f, 0.f, 0.f, 1.f);
            this->texture.use();
            glBindVertexArray(this->VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }
    }
    // don't forget to reset to default blending mode
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::init()
{
    // set up mesh and attribute properties
    unsigned int VBO;
    float particle_quad[] = {
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(this->VAO);
    // fill mesh buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
    // set mesh attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindVertexArray(0);

    // create this->amount default particle instances
    for (unsigned int i = 0; i < this->amount; ++i)
        this->particles.push_back(Particle());
}

// stores the index of the last particle used (for quick access to next dead particle)
unsigned int lastUsedParticle = 0;
unsigned int ParticleGenerator::firstUnusedParticle()
{
    // first search from last used particle, this will usually return almost instantly
    for (unsigned int i = lastUsedParticle; i < this->amount; ++i) {
        if (this->particles[i].Life <= 0.0f) {
            lastUsedParticle = i;
            return i;
        }
    }
    // otherwise, do a linear search
    for (unsigned int i = 0; i < lastUsedParticle; ++i) {
        if (this->particles[i].Life <= 0.0f) {
            lastUsedParticle = i;
            return i;
        }
    }
    // all particles are taken, override the first one (note that if it repeatedly hits this case, more particles should be reserved)
    lastUsedParticle = 0;
    return 0;
}

void ParticleGenerator::respawnParticle(Particle& particle, Player& object, glm::vec2 offset)
{
    float randomy = (rand() % 50) / 10.0f;
    float randx = ((rand() % 200)-100) / 10.0f;
    int sign = rand() %100;
    if (sign % 2 == 0) {
        randx = randx * -1;
    }
    float rColor = 0.5f + ((rand() % 100) / 100.0f);
    if (!object.isMirror()) {       
        particle.Position = (glm::vec2(object.getPosition()) + glm::vec2(16.f, 30.f)) + glm::vec2(randx, -randomy);
    }
    else {
        particle.Position = (glm::vec2(object.getPosition()) + glm::vec2(16.f, 2.f)) + glm::vec2(randx, -randomy);

    }
    particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
    particle.Life = 1.0f;
    particle.Velocity = object.getVelocity() * 0.1f;
}

void ParticleGenerator::respawnAllParticles(glm::vec2 pos, glm::vec2 offset) {
    for (unsigned int i = 0; i < this->amount; ++i)
    {
        Particle& particle = this->particles[i];
        float randomy = (rand() % 10) / 10.0f;
        float randx = ((rand() % 20) - 10) / 10.0f;
        int sign = rand() % 100;
        if (sign % 2 == 0) {
            randx = randx * -1;
        }
        int signy = rand() % 100;
        if (signy % 2 == 0) {
            randomy = randomy * -1;
        }
        float rColor = 0.5f + ((rand() % 100) / 100.0f);
       
        particle.Position = (pos) + glm::vec2(randx, randomy);
       
        particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
        particle.Life = 5.0f;
        float vel = (rand() % 10) / 100.f;
        particle.Velocity = glm::vec2(randx,randomy) * vel;
    }
}
