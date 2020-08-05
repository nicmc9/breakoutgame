#pragma once

#include <glad/glad.h>

/*
Класс для хранения и управления настройками 2D текстур
*/
class Texture2D
{
public:
    //Идентификатор объекта текстуры в памяти все функции работают через него
    unsigned int ID;
    // ширина и высота в пикселях загруженного изображения
    unsigned int Width, Height; 
    // формат текстурного объекта 
    unsigned int Internal_Format; 
    //формат загруженного изображения
    unsigned int Image_Format; 
   
    unsigned int Wrap_S; 
    unsigned int Wrap_T; 
    unsigned int Filter_Min; 
    unsigned int Filter_Max; 
   
    Texture2D();
    //создает текстуру из загруженного изображения
    void Generate(unsigned int width, unsigned int height, unsigned char* data);
    // привязывает текстуру как текущий GL_TEXTURE_2D объект
    void Bind() const;
};

