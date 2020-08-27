#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <vector>

#include "sprite_renderer.h"
#include "game_level.h"
#include "game_object.h"
#include "ball_object.h"
#include "text_renderer.h"
#include "particle_generator.h"
#include "post_processor.h"
#include "power_up.h"

//основные игровые состояния
enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

enum Direction {
	UP,
	RIGHT,
	DOWN,
	LEFT
}; 
/*
Игровой класс содержит всю логику игры и рендер,
 отделяет оконную часть от игровой
 */

// Начальный размер и скорость лопатки игрока
const glm::vec2 PLAYER_SIZE(150.0f, 20.0f);
const float PLAYER_VELOCITY = 500.0f;

// начальный размер и скорость шара
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
const float BALL_RADIUS = 12.5f;

typedef std::tuple<bool, Direction, glm::vec2> Collision; 

class Game
{
public:
    GameState               State;
    bool                    Keys[1024];
    bool                    KeysProcessed[1024]; // Обработка нажатив для меню
    unsigned int            Width, Height;
    //контейнер бонусов
    std::vector<PowerUp>  PowerUps;

    //контейнер уровней
    std::vector<GameLevel> Levels;
    unsigned int           CurrentLevel;
    unsigned int Lives;

    //

    SpriteRenderer* Renderer = nullptr;
    GameObject      *Player = nullptr;
    BallObject      *Ball = nullptr; 
    TextRenderer    *Text = nullptr;
    ParticleGenerator   *Particles = nullptr; 
    PostProcessor   *Effects = nullptr;

    /// Контейнеры для строк на русском языке
    std::vector<std::string> P_lives = {"Ж","и","з","н","е","й",":","0"};
    std::vector<std::string> P_start = {"Н","а","ж","м","и","т","е"," ", "В","В","О","Д"," ","д","л","я"," ","с","т","а","р","т","а"};
    std::vector<std::string> P_press = {"W"," ","и","л","и"," ","S"," ","п","о","м","е","н","я","т","ь"," ","у","р","о","в","е","н","ь"}; 
    std::vector<std::string> P_win = {"П","О","Б","Е","Д","А","!","!","!"}; 
    std::vector<std::string> P_winpress = {"В","В","О","Д"," ","п","р","о","д","о","л","ж","и","т","ь",","," ","E","S","C"," ","в","ы","х","о","д"};
    Game(unsigned int width, unsigned int height);
    ~Game();
     //Загружаем все шейдеры текстуры и создаем игровые объекты
    void Init();
    
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();

    void DoCollisions();
    //
    void SpawnPowerUps(GameObject &block);
    void UpdatePowerUps(float dt);
    void ActivatePowerUp(PowerUp &powerUp);
    // сброс параметров 
    void ResetLevel();
    void ResetPlayer();
};