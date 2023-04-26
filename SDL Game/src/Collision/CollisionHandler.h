#ifndef COLLISIONHANDLER_H
#define COLLISIONHANDLER_H

#include "TileLayer.h"
#include "GameMap.h"
#include "Engine.h"

#include<vector>

#include<SDL.h>
#include<SDL_image.h>


class CollisionHandler
{
    public:
        bool MapCollision(SDL_Rect a);
        bool ObjectCollision(SDL_Rect a);
        bool CheckCollision(SDL_Rect a, SDL_Rect b);
        bool UpCollision();

        inline static CollisionHandler* GetInstance() {return s_Instance = (s_Instance != nullptr) ? s_Instance : new CollisionHandler(); } //static


    private:
        CollisionHandler(); //ctrinh con private
        TileMap m_CollisionTilemap; //neu tileid >0 thi nhan vat co the va cham
        TileLayer* m_CollisionLayer; //tilelayer dc chon de co the va cham
        static CollisionHandler* s_Instance; //dinh nghia thanh phan static

};


#endif // COLLISIONHANDLER_H
