#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <iostream>
#include<string>
#include<map>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

class TextureManager
{
    public:
        inline static TextureManager* GetInstance() {return s_Instance = (s_Instance != nullptr)? s_Instance : new TextureManager();}

        bool Load(std::string id, std::string filename); //thuc hien cn load texture co id
        bool ParseTextures(std::string source); //phan tich file texture
        void Drop(std::string id); //bo texture khoi map khi ko can nua
        void Clean(); //don texture khoi bo nho de tranh tran bo nho

        void Draw(std::string id, int x, int y, int width, int height, float scaleX = 1, float scaleY = 1, float scrollRatio = 0,
                    SDL_RendererFlip flip = SDL_FLIP_NONE); //ve object tren screen
        void DrawFrame(std:: string id, int x, int y, int width, int height, int row, int frame, SDL_RendererFlip flip = SDL_FLIP_NONE);
            //ve frames, drawframe tuong tu nhu draw function nhung co them 2 thong so: dong thu(row) va so frame
        void DrawTile(std::string tilesetID, int tileSize, int x, int y, int row, int frame, SDL_RendererFlip flip = SDL_FLIP_NONE);
            //tuong tu DrawFrame nhung width vs height -> tilesize(16x16, 32x32...)

    private:
        TextureManager(){};
        std::map<std::string, SDL_Texture*> m_TextureMap; //quan ly texture bang id(string)
        static TextureManager* s_Instance;
};

#endif // TEXTUREMANAGER_H
