#pragma once

#include <map>
#include <string>

#include <glad/glad.h>

#include "texture.h"
#include "shader.h"

/*
Статический синглетон для управления ресурсами
может загружать текстуры и шейдеры и обеспечивает их хранение
*/
class ResourceManager
{
public:
    // хранилище ресурсов
    static std::map<std::string, Shader>    Shaders;
    static std::map<std::string, Texture2D> Textures;

    //Загружает шейдер из файла генерирует программу и сохраняет в хранилище
    static Shader    LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name);
    static Shader&    GetShader(std::string name);


    //Загружает и генерирует текстуру из файла и сохраняет в хранилище
    static Texture2D LoadTexture(const char* file, bool alpha, std::string name);
    
    static Texture2D& GetTexture(std::string name);

    // очиска шейдеров и текстур
    static void      Clear();
private:
    ResourceManager() = delete;

   // загружает и генерирует шейдерную программу
    static Shader    loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);
    // загружает и генерирует текстуру
    static Texture2D loadTextureFromFile(const char* file, bool alpha);
};

