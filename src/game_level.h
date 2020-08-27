#pragma once

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "game_object.h"
#include "sprite_renderer.h"
#include "resource_manager.h"

/*
  Контейнер для игровых объектов а именно кирпичей,
  сам загружает информацию с диска и инициализирем игровые объекты
  и сам их хранит  
*/
class GameLevel
{
public:
    
    std::vector<GameObject> Bricks;

    GameLevel() { }
    //Загрузка уровня из файла
    //размеры области отрисовки кирпичей  levelWidth == screen_widht levelHeight== screenHeight/2
    void Load(const char *file, unsigned int levelWidth, unsigned int levelHeight);
    // отрисовка всего уровня
    void Draw(SpriteRenderer &renderer);
    //проверка на завершенное, т.е. все кирпичи уничтожены кроме неубиваемых    
    bool IsCompleted();
private:
    //инициализация уровня на основе данных их файла
    void init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight);
};
