#ifndef ANIMATION_H
#define ANIMATION_H

#include <iostream>
#include<string>

#include<SDL.h>
#include<SDL_image.h>

class Animation
{
    public:
        Animation(bool repeat = true): m_Repeat(repeat) {m_IsEnded = false;}
        //khien animation repeat mai mai neu IsEnded != true

        virtual void Update(float dt) = 0;
        inline bool IsEnded() {return m_IsEnded;} //tra ve value IsEnded

    protected:
        bool m_Repeat;
        bool m_IsEnded;
        int m_CurrentFrame;

};

#endif // ANIMATION_H
