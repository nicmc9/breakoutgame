#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>


#include "texture.h"
#include "sprite_renderer.h"


// Container object for holding all state relevant for a single
// game object entity. Each object in the game likely needs the
// minimal of state as described within GameObject.
/*
 Контейнер игрового объекта (Актор), содержит все неоходимое для
 отрисовки и управления каждой игровой сущности в игре,
  может быть базовым классом и храниться в объекте уровня
*/

class GameObject
{
public:
    // Состояния рендер
    Texture2D   Sprite;	
    glm::vec2   Position, Size; 
    glm::vec3   Color;
    float       Rotation;

    //состояния игровой логики
    glm::vec2 Velocity;
    bool        IsSolid;
    bool        Destroyed;
   
    // constructor(s)
    GameObject();
    GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));
    
    //содержит функцию отрисовки т.е. управляет своим рисование
    virtual void Draw(SpriteRenderer &renderer);
};