#ifndef COLLIDER_H
#define COLLIDER_H

#include<SDL.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include<SDL_ttf.h>

class Collider
{
    public:
        inline SDL_Rect Get() {return m_Box;}
        inline void SetBuffer(int x, int y, int w, int h) {m_Buffer = {x, y, w, h};} //xoa/thay doi phan space thua cua player

        void Set(int x, int y, int w, int h) {
            m_Box = {
                x - m_Buffer.x,
                y - m_Buffer.y,
                w - m_Buffer.w,
                h - m_Buffer.h
            };
        }

	private:
        SDL_Rect m_Box;
        SDL_Rect m_Buffer;

};


#endif // COLLIDER_H
