#ifndef COLLISIONHANDLER_H
#define COLLISIONHANDLER_H

#include "TileLayer.h"
#include "GameMap.h"
#include "Engine.h"

#include<vector>

#include<SDL.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include<SDL_ttf.h>


class CollisionHandler
{
    public:
        bool MapCollision(SDL_Rect a);
        bool ObjectCollision(SDL_Rect a);
        bool CheckCollision(SDL_Rect a, SDL_Rect b);

        inline static CollisionHandler* GetInstance() {return s_Instance = (s_Instance != nullptr) ? s_Instance : new CollisionHandler(); }


    private:
        CollisionHandler();
        TileMap m_CollisionTilemap;
        TileLayer* m_CollisionLayer;
        static CollisionHandler* s_Instance;

};


#endif // COLLISIONHANDLER_H
