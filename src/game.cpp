#include <iostream>

#include "game.hpp"
#include "resource_manager.h"



#include <irrKlang.h>
using namespace irrklang;

ISoundEngine *SoundEngine = createIrrKlangDevice();

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
}

void Game::Init()
{
    Shader sprite_shader = ResourceManager::LoadShader("resources/shaders/sprite.vs", "resources/shaders/sprite.fs", nullptr, "sprite");
    Shader particle_shader = ResourceManager::LoadShader("resources/shaders/particle.vs", "resources/shaders/particle.fs", nullptr, "particle");
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

    Renderer = new SpriteRenderer(sprite_shader);

    glm::vec2 playerPos = glm::vec2( this->Width / 2.0f - PLAYER_SIZE.x / 2.0f,  this->Height - PLAYER_SIZE.y  );
    Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));

    glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x/2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);

    Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::GetTexture("face"));

    Text = new TextRenderer(this->Width, this->Height);
    Text->Load("resources/fonts/arial.ttf", 24);

    Particles = new ParticleGenerator( particle_shader, ResourceManager::GetTexture("particle"), 500);
}

void Game::Update(float dt)
{
    Ball->Move(dt, this->Width);

    Particles->Update(dt, *Ball, 2, glm::vec2(Ball->Radius / 2.0f));

    // проверка всех коллизий
    this->DoCollisions();

     if (Ball->Position.y >= this->Height) // did ball reach bottom edge?
    {
        --this->Lives;
        // did the player lose all his lives? : Game over
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
        //TODO включить позже
        //Effects->Chaos = true; 
        this->State = GAME_WIN;
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
        //std::cout<<"velocity paddle == "<<velocity<<'\n';
        // move playerboard
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
            this->KeysProcessed[GLFW_KEY_ENTER] = true; //Иноче сразу запустит уровень 
            //Effects->Chaos = false;
            this->State = GAME_MENU;
        }
    }    
}

void Game::Render()
{
       if(this->State == GAME_ACTIVE|| this->State == GAME_MENU)
    {
        // Используем SpriteRenderer напрямую для отрисовки фона
        Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f );
        //Логигу отрисовки уровня отдаем самому уровню - вместе с настренным рисовальшиком
        this->Levels[this->CurrentLevel].Draw(*Renderer);

        Player->Draw(*Renderer);
        Ball->Draw(*Renderer);
          // Отрисовка частиц
        Particles->Draw();

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
    // reset player/ball stats
    Player->Size = PLAYER_SIZE;
    Player->Position = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height - PLAYER_SIZE.y);
    Ball->Reset(Player->Position + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -(BALL_RADIUS * 2.0f)), INITIAL_BALL_VELOCITY);
}

// collision detection
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
                      SoundEngine->play2D("resources/audio/bleep.mp3", false);
                   }
                   else{
                      SoundEngine->play2D("resources/audio/solid.wav", false);
                   }
                   Direction dir = std::get<1>(collision);
                   glm::vec2 diff_vector  = std::get<2>(collision);
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

    Collision result = CheckCollision(*Ball, *Player);
    if (!Ball->Stuck && std::get<0>(result))
    {
        // check where it hit the board, and change velocity based on where it hit the board
        float centerBoard = Player->Position.x + Player->Size.x / 2.0f;
        float distance = (Ball->Position.x + Ball->Radius) - centerBoard;
        float percentage = distance / (Player->Size.x / 2.0f);
        // then move accordingly
        float strength = 2.0f;
        glm::vec2 oldVelocity = Ball->Velocity;
        Ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength; 
        Ball->Velocity.y = -1.0f * abs(Ball->Velocity.y);  
        Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(oldVelocity);
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
    // get center point circle first 
    glm::vec2 center(one.Position + one.Radius);
    // calculate AABB info (center, half-extents)
    glm::vec2 aabb_half_extents(two.Size.x / 2.0f, two.Size.y / 2.0f);
    glm::vec2 aabb_center(
        two.Position.x + aabb_half_extents.x, 
        two.Position.y + aabb_half_extents.y
    );
    // get difference vector between both centers
    glm::vec2 difference = center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    // add clamped value to AABB_center and we get the value of box closest to circle
    glm::vec2 closest = aabb_center + clamped;
    // retrieve vector between center circle and closest point AABB and check if length <= radius
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