#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include "text_renderer.h"
#include "resource_manager.h"


TextRenderer::TextRenderer(unsigned int width, unsigned int height)
{
    // Загрузка и конфигурация шейдера для рендера текста
    this->TextShader = ResourceManager::LoadShader("resources/shaders/text_2d.vs", "resources/shaders/text_2d.fs", nullptr, "text");
    this->TextShader.SetMatrix4("projection", glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f), true);
    this->TextShader.SetInteger("text", 0);
    
    // конфигурация вершинных буферов
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void TextRenderer::Load(std::string font, unsigned int fontSize)
{
    std::map<std::string, int> ruslit; 

    fill_symbols(ruslit); //TODO добавить обрабутку ошибок
    // сначала почистим контейнер
    this->Characters.clear();
    // инициализация самой библиотеки FreeType
    FT_Library ft;    
    if (FT_Init_FreeType(&ft)) // все что отличаеться от 0 ошибка
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    // загрузка шрифта
    FT_Face face;
    if (FT_New_Face(ft, font.c_str(), 0, &face))
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    // Установка размера глифа
    FT_Set_Pixel_Sizes(face, 0, fontSize);
    // Установка смещения байт для текстур в 1 байт
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
    // используем первые 128 ascii символов т.е.128 текстур символов
     
    for (auto c = ruslit.begin(); c != ruslit.end(); c++)
    {
            auto unicode = (*c).second;
            auto symbol = (*c).first;
        // Закрузка конкретного глифа для символа
        if (FT_Load_Char(face, unicode, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
       //сохраняем глиф в текстуре
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
            );
     
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
       
        // текстуру и метрики глифа размещаем в контейнере
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        Characters.insert({ symbol,character });
    }

   
    glBindTexture(GL_TEXTURE_2D, 0);
    // можно освободить память под шрифт
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    //Пока не нужен можно перевести в локальную переменную
    ruslit.clear(); 
}

void TextRenderer::RenderText(std::vector<std::string>& phrase, float x, float y, float scale, glm::vec3 color)
{
     //активируем состояния рендера	
    this->TextShader.Use();
    this->TextShader.SetVector3f("textColor", color);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(this->VAO);

    //Проходим по контейнеру строк (для русских символов сложнее)
       for (auto str = phrase.begin(); str != phrase.end(); str++) 
    {
        Character ch = Characters[*str];
        float xpos = x + ch.Bearing.x * scale;
        // горизонталь проавильно выравниваеться по метрикам большой буквы
        float ypos = y + (this->Characters["Ж"].Bearing.y - ch.Bearing.y) * scale;
        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // обновляем VBO для каждого символа GL_DYNAMIC_DRAW
        float vertices[6][4] =   {
            { xpos,     ypos + h,   0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 0.0f },
            { xpos,     ypos,       0.0f, 0.0f },

            { xpos,     ypos + h,   0.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 0.0f }
        };
        // отрисовываем текстуру глифа на квадрате
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // обновляем содержимое в памяти
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        // обязательно используем glBufferSubData, а не glBufferData
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // рисуем квадрат
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // теперь переместите курсоры для следующего глифа
        // битовый сдвиг на 6, чтобы получить значение в пикселях (1/64 раз 2 ^ 6 = 64)
        x += (ch.Advance >> 6) * scale; 
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}