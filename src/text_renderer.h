#pragma once


#include <map>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "texture.h"
#include "shader.h"
#include "ruslit.h"

/// Содержит информацию загруженную из FreeType которую мы сохраним в контейнер
struct Character {
    unsigned int TextureID; // ИД текстуры содержащий 1 глиф (символ)
    glm::ivec2   Size;      // ширина и высота 
    glm::ivec2   Bearing;   // смещения от базовой линии влево и вверх глифа
    size_t Advance;   // горизонтальное смещение до следующего глифа
};


/*
 Класс загружает шрифт и хранить текстуры и информацию по глифам,
 а также функцию отрисовки
*/
class TextRenderer
{
public:
   
    std::map<std::string, Character> Characters;

   
  
    Shader TextShader;
  
       
    TextRenderer(unsigned int width, unsigned int height);
    
    void Load(std::string font, unsigned int fontSize);
    // renders a string of text using the precompiled list of characters
    void RenderText(std::vector<std::string>& phrase, float x, float y, float scale, glm::vec3 color = glm::vec3(1.0f));
private:
    // render state
    unsigned int VAO, VBO;
};