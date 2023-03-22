#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <iostream>
#include<string>
#include<map>

#include<SDL.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include<SDL_ttf.h>

class TextureManager
{
    public:
        static TextureManager* GetInstance() {return s_Instance = (s_Instance != nullptr)? s_Instance : new TextureManager();}

        bool Load(std::string id, std::string filename); //thuc hien cn load
        void Drop(std::string id); //bo texture khoi map
        void Clean(); //don texture khoi bo nho

        void Draw(std::string id, int x, int y, int width, int height, SDL_RendererFlip flip = SDL_FLIP_NONE); //ve tren screen

    private:
        TextureManager(){}
        std::map<std::string, SDL_Texture*> m_TextureMap; //quan ly texture
        static TextureManager* s_Instance;
};

#endif // TEXTUREMANAGER_H
