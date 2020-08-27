#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.h"
#include "shader.h"

/*
Класс отвечающий за отрисовку спрайтов
Создает VAO по умолчанию . а всю информацию для отрисовки
мы передаем в DrawSprite тем самы используя тот же VAO и шейдер
но получая разные по виду и положению спрайты
*/
class SpriteRenderer
{
public:
     
    SpriteRenderer(Shader &shader);
    ~SpriteRenderer();
    /* передаем текстуру которая накладывается на данных спрайт, 
       позиция размер и угол поворота нужны для матрицы модели
       цвет передается в шейдер т.к текстуры мы загружаем в градациях серого 
    */
    void DrawSprite(Texture2D &texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f),
                    float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
private:

    Shader       shader;  
    unsigned int quadVAO;
    //Настраиваем буферы данных для спрайта
    void initRenderData();
};

