
#pragma once
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "shader.h"
#include "texture.h"
#include "game_object.h"
struct Particle
{
    glm::vec2 Position, Velocity;
    glm::vec4 Color;
    float Life;

    Particle(): Position(0.0f), Velocity(0.0f), Color(1.0f),Life(0.0f){}
};

// ParticleGenerator acts as a container for rendering a large number of 
// particles by repeatedly spawning and updating particles and killing 
// them after a given amount of time.

/*
Генератор частиц действует как контейнер  для большого количества частиц,
инициализирует данные OpenGL, а также за уничтожение и оживление частиц
*/
class ParticleGenerator
{
public:
    
    ParticleGenerator(Shader shader, Texture2D texture, unsigned int amount);
    //Обновляет состояние частиц
    void Update(float dt, GameObject &object, unsigned int newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
    // отрисовка
    void Draw();
private:
   
    std::vector<Particle> particles;
    unsigned int amount;
    // Данные для отрисовки
    Shader shader;
    Texture2D texture;
    unsigned int VAO;
    void init();
    //
   
    // возвращает индекс первой частицв Live <= 0.0f 
    unsigned int firstUnusedParticle();
    // возрождение частицы
    void respawnParticle(Particle &particle, GameObject &object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};


