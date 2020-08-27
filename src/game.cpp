#include <iostream>
#include <algorithm>

#include "game.hpp"
#include "resource_manager.h"



#include <irrKlang.h>
using namespace irrklang;

ISoundEngine *SoundEngine = createIrrKlangDevice();


float ShakeTime = 0.0f;

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height), Lives(3)
{
}

Game::~Game()
{
    delete Renderer;
    delete Player;
    delete Ball;
    delete Text;
    delete Particles;
    delete Effects;
    SoundEngine->drop();
}

void Game::Init()
{
    Shader sprite_shader = ResourceManager::LoadShader("resources/shaders/sprite.vs", "resources/shaders/sprite.fs", nullptr, "sprite");
    Shader particle_shader = ResourceManager::LoadShader("resources/shaders/particle.vs", "resources/shaders/particle.fs", nullptr, "particle");
    Shader postproc_shader = ResourceManager::LoadShader("resources/shaders/post_processing.vs", "resources/shaders/post_processing.fs", nullptr, "postprocessing");

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width), static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    //uniform sampler2D image; индекс в наборе текстур для данного семплера, т.к всегда одна текстура ставим это здесь
    sprite_shader.Use().SetInteger("image", 0);
    //матрица проекции одинаковая для всех спрайтов в игре поэтому ставим здесь
    sprite_shader.SetMatrix4("projection", projection);
    particle_shader.Use().SetInteger("sprite", 0);
    particle_shader.SetMatrix4("projection", projection);   


    //фоновый звук
    SoundEngine->play2D("resources/audio/breakout.mp3", true);

    //Загружаем все текстуры
    ResourceManager::LoadTexture("resources/textures/awesomeface.png", true, "face");
    ResourceManager::LoadTexture("resources/textures/background.jpg", false, "background");
    ResourceManager::LoadTexture("resources/textures/block.png", false, "block");
    ResourceManager::LoadTexture("resources/textures/block_solid.png", false, "block_solid");
    ResourceManager::LoadTexture("resources/textures/paddle.png", true, "paddle");
    ResourceManager::LoadTexture("resources/textures/particle.png", true, "particle"); 
    ResourceManager::LoadTexture("resources/textures/powerup_speed.png", true, "powerup_speed");
    ResourceManager::LoadTexture("resources/textures/powerup_sticky.png", true, "powerup_sticky");
    ResourceManager::LoadTexture("resources/textures/powerup_increase.png", true, "powerup_increase");
    ResourceManager::LoadTexture("resources/textures/powerup_confuse.png", true, "powerup_confuse");
    ResourceManager::LoadTexture("resources/textures/powerup_chaos.png", true, "powerup_chaos");
    ResourceManager::LoadTexture("resources/textures/powerup_passthrough.png", true, "powerup_passthrough");

    //Загружаем все уровни
    GameLevel one; one.Load("resources/levels/one.lvl", this->Width, this->Height / 2);
    GameLevel two; two.Load("resources/levels/two.lvl", this->Width, this->Height / 2);
    GameLevel three; three.Load("resources/levels/three.lvl", this->Width, this->Height / 2);
    GameLevel four; four.Load("resources/levels/four.lvl", this->Width, this->Height / 2);
    this->Levels.push_back(one);
    this->Levels.push_back(two);
    this->Levels.push_back(three);
    this->Levels.push_back(four);
    this->CurrentLevel = 0;

    Effects = new PostProcessor(postproc_shader, this->Width, this->Height);
    Renderer = new SpriteRenderer(sprite_shader);
    Particles = new ParticleGenerator( particle_shader, ResourceManager::GetTexture("particle"), 500);
    

    glm::vec2 playerPos = glm::vec2( this->Width / 2.0f - PLAYER_SIZE.x / 2.0f,  this->Height - PLAYER_SIZE.y  );
    Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));

    glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x/2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);
    Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::GetTexture("face"));

    Text = new TextRenderer(this->Width, this->Height);
    Text->Load("resources/fonts/arial.ttf", 24);

    
}

void Game::Update(float dt)
{
    Ball->Move(dt, this->Width);
 // проверка всех коллизий
    this->DoCollisions();

    Particles->Update(dt, *Ball, 2, glm::vec2(Ball->Radius / 2.0f));
  // обновить состояния бонусов
    this->UpdatePowerUps(dt);
   

     if (Ball->Position.y >= this->Height) // достил ли мяч нижней границы
    {
        --this->Lives;
    
        //кончились жизни ? игра закончена
        if (this->Lives == 0)
        {
            this->ResetLevel();
            this->State = GAME_MENU;
        }
        this->ResetPlayer();
    }

    if (this->State == GAME_ACTIVE && this->Levels[this->CurrentLevel].IsCompleted())
    {
        this->ResetLevel();
        this->ResetPlayer();
        Effects->Chaos = true; 
        this->State = GAME_WIN;
    }

    if (ShakeTime > 0.0f)
    {
        ShakeTime -= dt;
        if (ShakeTime <= 0.0f)
            Effects->Shake = false;
    }

}

void Game::ProcessInput(float dt)
{

if (this->State == GAME_MENU)
{
    if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER])
        {
            this->State = GAME_ACTIVE;
            this->KeysProcessed[GLFW_KEY_ENTER] = true;
        }
    if (this->Keys[GLFW_KEY_W] && !this->KeysProcessed[GLFW_KEY_W])
        {
            this->CurrentLevel = (this->CurrentLevel + 1) % 4; //Этого достаточно чтобы поменять отрисовку уровня    
            this->KeysProcessed[GLFW_KEY_W] = true;
        }
    if (this->Keys[GLFW_KEY_S] && !this->KeysProcessed[GLFW_KEY_S])
        {
           if (this->CurrentLevel > 0)
            --this->CurrentLevel;
        else
            this->CurrentLevel = 3;
            this->KeysProcessed[GLFW_KEY_S] = true;
        }
   
   
}  

if (this->State == GAME_ACTIVE)
    {
        float velocity = PLAYER_VELOCITY * dt;
      
        if (this->Keys[GLFW_KEY_A])
        {
            if (Player->Position.x >= 0.0f){
                Player->Position.x -= velocity;
                if (Ball->Stuck)
                    Ball->Position.x -= velocity;
            }
        }
        if (this->Keys[GLFW_KEY_D])
        {
            if (Player->Position.x <= this->Width - Player->Size.x){
                Player->Position.x += velocity;
                if (Ball->Stuck)
                    Ball->Position.x += velocity;
            }
        }
        if (this->Keys[GLFW_KEY_SPACE])
            Ball->Stuck = false;
    }

if (this->State == GAME_WIN)
    {
        if (this->Keys[GLFW_KEY_ENTER])
        {
            this->KeysProcessed[GLFW_KEY_ENTER] = true; //Иначе сразу запустит уровень 
            Effects->Chaos = false;
            this->State = GAME_MENU;
        }
    }    
}

void Game::Render()
{
       if(this->State == GAME_ACTIVE|| this->State == GAME_MENU || this->State == GAME_WIN)
    {
       Effects->BeginRender();
        // Используем SpriteRenderer напрямую для отрисовки фона
       Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f );
        //Логигу отрисовки уровня отдаем самому уровню - вместе с настренным рисовальшиком
       this->Levels[this->CurrentLevel].Draw(*Renderer);

       Player->Draw(*Renderer);
       Ball->Draw(*Renderer);
      //отрисовка бонусов
            for (PowerUp &powerUp : this->PowerUps)
                if (!powerUp.Destroyed)
                    powerUp.Draw(*Renderer);

       // Отрисовка частиц
       Particles->Draw();
              
       Effects->EndRender();
       Effects->Render(glfwGetTime()); 

       P_lives.pop_back();
       P_lives.emplace_back(std::to_string(this->Lives));
       Text->RenderText(P_lives , 5.0f, 5.0f, 1.0f);

    }

     if (this->State == GAME_MENU)
    {
        Text->RenderText(P_start, 250.0f, Height / 2, 1.0f);
        Text->RenderText(P_press, 275.0f, Height / 2 + 30.0f, 0.75f);
    }

     if (this->State == GAME_WIN)
    {
        Text->RenderText(P_win, 320.0, Height / 2 - 20.0, 1.0, glm::vec3(0.0, 1.0, 0.0) );
        Text->RenderText(P_winpress, 130.0, Height / 2, 1.0, glm::vec3(1.0, 1.0, 0.0));
    }
}

// Перезагрузка обновляет состояние т.к. параметры объектов isDestroyed в векторе меняються 
void Game::ResetLevel()
{
    if (this->CurrentLevel == 0)
        this->Levels[0].Load("resources/levels/one.lvl", this->Width, this->Height / 2);
    else if (this->CurrentLevel == 1)
        this->Levels[1].Load("resources/levels/two.lvl", this->Width, this->Height / 2);
    else if (this->CurrentLevel == 2)
        this->Levels[2].Load("resources/levels/three.lvl", this->Width, this->Height / 2);
    else if (this->CurrentLevel == 3)
        this->Levels[3].Load("resources/levels/four.lvl", this->Width, this->Height / 2);

    this->Lives = 3;
}

void Game::ResetPlayer()
{
    
    //Сброс состояний игрока и мяча
    Player->Size = PLAYER_SIZE;
    Player->Position = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height - PLAYER_SIZE.y);
    Ball->Reset(Player->Position + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -(BALL_RADIUS * 2.0f)), INITIAL_BALL_VELOCITY);
}

// Определение столкновений
bool CheckCollision(GameObject &one, GameObject &two);
Collision CheckCollision(BallObject &one, GameObject &two);
Direction VectorDirection(glm::vec2 closest);


void Game::DoCollisions()
{
    for(GameObject& box : this->Levels[this->CurrentLevel].Bricks)
    {
        if(!box.Destroyed)
        {
            Collision collision = CheckCollision(*Ball, box);

             if(std::get<0>(collision)){
                   if(!box.IsSolid){
                      box.Destroyed = true;
                      this->SpawnPowerUps(box);
                      SoundEngine->play2D("resources/audio/bleep.mp3", false);
                   }
                   else{
                        // Если блок твердый активируем эффект ненадолго
                      ShakeTime = 0.05f;
                      Effects->Shake= true;
                      SoundEngine->play2D("resources/audio/solid.wav", false);
                   }
                   Direction dir = std::get<1>(collision);
                   glm::vec2 diff_vector  = std::get<2>(collision);
                   //не производим разрешение столкновений если активирован бонус pass-through и блок не твердый
                  if (!(Ball->PassThrough && !box.IsSolid)) 
                {
                   if(dir == LEFT || dir == RIGHT)
                   {
                        Ball->Velocity.x = -Ball->Velocity.x;
                        float penetration = Ball->Radius - std::abs(diff_vector.x);
                        if(dir == LEFT)
                            Ball->Position.x += penetration;
                        else
                            Ball->Position.x -= penetration;
                   }
                   else
                   {
                       Ball->Velocity.y = - Ball->Velocity.y;
                       float penetration = Ball->Radius  - std::abs(diff_vector.y);
                       if(dir == UP)
                            Ball->Position.y -= penetration;
                       else
                            Ball->Position.y += penetration;
                   }
                    
                }
            }
        }
    }
  
  //Добавляем проверку столкновений между игроком и Бонусами
    for (PowerUp &powerUp : this->PowerUps)
    {
        if (!powerUp.Destroyed)
        {
            
            //Если бонус достиг нижней границы уничтожаем его
            if (powerUp.Position.y >= this->Height)
                powerUp.Destroyed = true;

            if (CheckCollision(*Player, powerUp))
            {	
                //если игрок столкнулся с бонусом то его удаляем, но при этом активируем эффект
                ActivatePowerUp(powerUp);
                powerUp.Destroyed = true;
                powerUp.Activated = true;
                SoundEngine->play2D("resources/audio/powerup.wav", false);
            }
        }
    }
    Collision result = CheckCollision(*Ball, *Player);
    if (!Ball->Stuck && std::get<0>(result))
    {
        //проверяем в каком месте лопатки ударился мяч и в зависимости от этого меняем скорость
        float centerBoard = Player->Position.x + Player->Size.x / 2.0f;
        float distance = (Ball->Position.x + Ball->Radius) - centerBoard;
        float percentage = distance / (Player->Size.x / 2.0f);
     
        float strength = 2.0f;
        glm::vec2 oldVelocity = Ball->Velocity;
        Ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength; 
        Ball->Velocity.y = -1.0f * abs(Ball->Velocity.y);  
        Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(oldVelocity);
      
        //Когда бонус липкости активировант , мы меняем состояние меча на стационарное
        Ball->Stuck = Ball->Sticky;

        SoundEngine->play2D("resources/audio/bleep.wav", false);
    } 

}


//AABB collision
bool CheckCollision(GameObject& one, GameObject& two){

    // collision x -axis?
    bool collisionX = one.Position.x + one.Size.x >= two.Position.x  && two.Position.x + two.Size.x >=one.Position.x;
    //collison  y - axis? 
    bool collisionY = one.Position.y + one.Size.y >= two.Position.y && two.Position.y + two.Size.y >= one.Position.y;
    //
    return collisionX&&collisionY;
}

Collision CheckCollision(BallObject &one, GameObject &two) // AABB - Circle collision
{
    //получить точку центра мяча
    glm::vec2 center(one.Position + one.Radius);
    // Вычислим информацию о квадрате (центр, и половинные размеры )
    glm::vec2 aabb_half_extents(two.Size.x / 2.0f, two.Size.y / 2.0f);
    glm::vec2 aabb_center(
        two.Position.x + aabb_half_extents.x, 
        two.Position.y + aabb_half_extents.y
    );
    // получить вектор разницы между центрами
    glm::vec2 difference = center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    //добавляем фиксированное значение к AABB_center, и мы получаем значение прямоугольника, ближайшего к кругу
     glm::vec2 closest = aabb_center + clamped;
    //получить вектор между центральным кругом и ближайшей точкой AABB и проверить, если длина <= радиус то имеем столкновение
    difference = closest - center;
    if(glm::length(difference) <= one.Radius)
        return std::make_tuple(true, VectorDirection(difference), difference);
    else
        return std::make_tuple(false, UP, glm::vec2(0.0f, 0.0f));
}   


Direction VectorDirection(glm::vec2 target)
{
    glm::vec2 compass[] = {
        glm::vec2(0.0f, 1.0f),	// up
        glm::vec2(1.0f, 0.0f),	// right
        glm::vec2(0.0f, -1.0f),	// down
        glm::vec2(-1.0f, 0.0f)	// left
    };
    float max = 0.0f;
    unsigned int best_match = -1;
    for (unsigned int i = 0; i < 4; i++)
    {
        float dot_product = glm::dot(glm::normalize(target), compass[i]);
        if (dot_product > max)
        {
            max = dot_product;
            best_match = i;
        }
    }
    return (Direction)best_match;
}    

// Бонусы
bool IsOtherPowerUpActive(std::vector<PowerUp> &powerUps, std::string type);

void Game::UpdatePowerUps(float dt)
{
    for (PowerUp &powerUp : this->PowerUps)
    {
        powerUp.Position += powerUp.Velocity * dt;
        if (powerUp.Activated)
        {
            powerUp.Duration -= dt;

            if (powerUp.Duration <= 0.0f)
            {
                //убрать бонус из списка(позже будет удален)
                powerUp.Activated = false;
                // убрать эффекты
                if (powerUp.Type == "sticky")
                {
                    if (!IsOtherPowerUpActive(this->PowerUps, "sticky"))
                    {	// убираем эффект только после проверки, на активность других аналогичных эффектов
                        // как бы складываем их длительность
                        Ball->Sticky = false;
                        Player->Color = glm::vec3(1.0f);
                    }
                }
                else if (powerUp.Type == "pass-through")
                {
                    if (!IsOtherPowerUpActive(this->PowerUps, "pass-through"))
                    {	
                        Ball->PassThrough = false;
                        Ball->Color = glm::vec3(1.0f);
                    }
                }
                else if (powerUp.Type == "confuse")
                {
                    if (!IsOtherPowerUpActive(this->PowerUps, "confuse"))
                    {	
                        Effects->Confuse = false;
                    }
                }
                else if (powerUp.Type == "chaos")
                {
                    if (!IsOtherPowerUpActive(this->PowerUps, "chaos"))
                    {	
                        Effects->Chaos = false;
                    }
                }
            }
        }
    }

    // Удаляем все Бонусы из контейнера, которые были уничтожены И неАктивированы 
    this->PowerUps.erase(std::remove_if(this->PowerUps.begin(), this->PowerUps.end(),
        [](const PowerUp &powerUp) { return powerUp.Destroyed && !powerUp.Activated; }
    ), this->PowerUps.end());
}

bool ShouldSpawn(unsigned int chance)
{
    unsigned int random = rand() % chance;
    return random == 0;
}
void Game::SpawnPowerUps(GameObject &block)
{
    if (ShouldSpawn(75)) // Шанс 1 к 75 
        this->PowerUps.push_back(PowerUp("speed", glm::vec3(0.5f, 0.5f, 1.0f), 0.0f, block.Position, ResourceManager::GetTexture("powerup_speed")));
    if (ShouldSpawn(75))
        this->PowerUps.push_back(PowerUp("sticky", glm::vec3(1.0f, 0.5f, 1.0f), 20.0f, block.Position, ResourceManager::GetTexture("powerup_sticky")));
    if (ShouldSpawn(75))
        this->PowerUps.push_back(PowerUp("pass-through", glm::vec3(0.5f, 1.0f, 0.5f), 10.0f, block.Position, ResourceManager::GetTexture("powerup_passthrough")));
    if (ShouldSpawn(75))
        this->PowerUps.push_back(PowerUp("pad-size-increase", glm::vec3(1.0f, 0.6f, 0.4), 0.0f, block.Position, ResourceManager::GetTexture("powerup_increase")));
    if (ShouldSpawn(15)) // Негативные эффекты должны появлятся чаще
        this->PowerUps.push_back(PowerUp("confuse", glm::vec3(1.0f, 0.3f, 0.3f), 15.0f, block.Position, ResourceManager::GetTexture("powerup_confuse")));
    if (ShouldSpawn(15))
        this->PowerUps.push_back(PowerUp("chaos", glm::vec3(0.9f, 0.25f, 0.25f), 15.0f, block.Position, ResourceManager::GetTexture("powerup_chaos")));
}

void Game::ActivatePowerUp(PowerUp &powerUp)
{
    if (powerUp.Type == "speed")
    {
        Ball->Velocity *= 1.2;
    }
    else if (powerUp.Type == "sticky")
    {
        Ball->Sticky = true;
        Player->Color = glm::vec3(1.0f, 0.5f, 1.0f);
    }
    else if (powerUp.Type == "pass-through")
    {
        Ball->PassThrough = true;
        Ball->Color = glm::vec3(1.0f, 0.5f, 0.5f);
    }
    else if (powerUp.Type == "pad-size-increase")
    {
        Player->Size.x += 50;
    }
    else if (powerUp.Type == "confuse")
    {
        if (!Effects->Chaos)
            Effects->Confuse = true; 
    }
    else if (powerUp.Type == "chaos")
    {
        if (!Effects->Confuse)
            Effects->Chaos = true;
    }
}

bool IsOtherPowerUpActive(std::vector<PowerUp> &powerUps, std::string type)
{
  // Проверяем, активен ли еще один Бонус того же типа
  // в этом случае мы не отключаем его действие (пока)
    
    for (const PowerUp &powerUp : powerUps)
    {
        if (powerUp.Activated)
            if (powerUp.Type == type)
                return true;
    }
    return false;
}