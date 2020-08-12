#include "ball_object.h"


BallObject::BallObject() 
    : GameObject(), Radius(12.5f), Stuck(true), Sticky(false), PassThrough(false) { }

BallObject::BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite)
    : GameObject(pos, glm::vec2(radius * 2.0f, radius * 2.0f), sprite, glm::vec3(1.0f), velocity), Radius(radius), Stuck(true)
    , Sticky(false), PassThrough(false) { }


glm::vec2 BallObject::Move(float dt, unsigned int window_width)
{
    // Если шар не прилип к лопатке меняет его позиции  в пространстве с проверкой границ
    if(!this->Stuck)
    {
        //Двигаем шар
        this->Position += this->Velocity*dt;  //Скорость содержит и направление ее обновляем в коллизиях
        //ПРоверка достижения границ экрана
        if(this->Position.x <= 0.0f)
        {
            this->Velocity.x = - this->Velocity.x; // меняем направление скорости
            this->Position.x = 0.0f;
        }
        else if (this->Position.x + this->Size.x >= window_width)
        {
            this->Velocity.x = -this->Velocity.x;
            this->Position.x = window_width - this->Size.x;
        }
        if(this->Position.y <= 0.0f)
        {
            this->Velocity.y = - this->Velocity.y;
            this->Position.y = 0.0f;
        }
        //от нижней границы не отталкиваемся это потеря жизни
    }
    return this->Position;  // TODO проверить этот возврат 
}

// Сбрасываем мяч к начальной позиции на лапатке если он улетел вниз
void BallObject::Reset(glm::vec2 position, glm::vec2 velocity)
{
    this->Position = position;
    this->Velocity = velocity;
    this->Stuck = true;
    this->Sticky = false;
    this->PassThrough = false;
}