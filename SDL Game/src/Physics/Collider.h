#ifndef COLLIDER_H
#define COLLIDER_H

#include "CollisionHandler.h"
#include "Camera.h"
#include "Engine.h"


#include<SDL.h>
#include<SDL_image.h>

class Collider
{
    public:
        inline SDL_Rect Get() {return m_Box;} //tra ve box hcn bao quanh player -> dung de check va cham voi map/object...
        inline void SetBuffer(int x, int y, int w, int h) {m_Buffer = {x, y, w, h};}
        //xoa/thay doi phan space thua(khoang trong) cua object nhan vat tuy vao animation

        void Set(int x, int y, int w, int h) {
            m_Box = {
                x - m_Buffer.x,
                y - m_Buffer.y,
                w - m_Buffer.w,
                h - m_Buffer.h
            }; //box bao quanh nv khi da bo di phan space thua
        }

        bool CollideWithMap() {
            return CollisionHandler::GetInstance()->MapCollision(m_Box); //box player va cham voi map
        }

	private:
        SDL_Rect m_Box;
        SDL_Rect m_Buffer;

};


#endif // COLLIDER_H
