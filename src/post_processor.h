#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "sprite_renderer.h"
#include "shader.h"

/*
    Класс PostProcessor содержит состояния всех эффектов постобработки в игре.
    Класс использует внекабровый буфер для сохранения кадра в текстуру, 
    и с помощью булевых переменных управляем включением или выключением 
    эффектов Shake, Confuse, Chaos. 
    BeginRender() должнен вызываться перед любым рендером
    EndRender() соответсвенно  в конце
*/

class PostProcessor
{
public:
    // состояни
    Shader PostProcessingShader;
    Texture2D Texture;
    unsigned int Width, Height;
    // опции
    bool Confuse, Chaos, Shake;
    // 
    PostProcessor(Shader shader, unsigned int width, unsigned int height);
    // подготавливает внекадровый буфер перед отрисовкой
    void BeginRender();
    // должен быть вызван в конце отприсовки
    void EndRender();
    // отрисовываем оттекстурированный квадрат в зависимости от опций
    void Render(float time);
private:
    unsigned int MSFBO, FBO; // MSFBO = многоотборочный буфер. обычный внекадровый буфер
    unsigned int RBO; // RBO  используется для многоотборочного цветного буфера
    unsigned int VAO;
    // 
    void initRenderData();
};

