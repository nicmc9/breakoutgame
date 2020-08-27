#pragma once

#include <string>

#include "game_object.h"

// Размер и скорость блока бонусов
const glm::vec2 POWERUP_SIZE(60.0f, 20.0f);
const glm::vec2 VELOCITY(0.0f, 150.0f); // только вертикальное перемещение

/*
Бонусы наследуют состояние  и функции GameObject,
но и содержат дополнительные состояния, продолжительность воздействия
и флаг активации
*/
class PowerUp : public GameObject 
{
public:
    // 
    std::string Type;
    float       Duration;	
    bool        Activated;
    // 
    PowerUp(std::string type, glm::vec3 color, float duration, 
            glm::vec2 position, Texture2D texture) 
        : GameObject(position, POWERUP_SIZE, texture, color, VELOCITY), 
          Type(type), Duration(duration), Activated() 
    { }
};  