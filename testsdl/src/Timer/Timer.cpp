#include "Timer.h"

#include<SDL.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include<SDL_ttf.h>

Timer* Timer::s_Instance = nullptr; //dinh nghia static member timer = nullptr

void Timer::Tick()
{
    m_DeltaTime = (SDL_GetTicks() - m_LastTime) * (TARGET_FPS / 1000.0f); // FPS/1000 -> FPmS


    if(m_DeltaTime > TARGET_DELTATIME) {
        m_DeltaTime = TARGET_DELTATIME; //giu deltatime <= TARGet_DELTATIME de dam bao animation on dinh
    }

    m_LastTime = SDL_GetTicks();
}
