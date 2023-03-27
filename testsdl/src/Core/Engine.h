#ifndef ENGINE_H
#define ENGINE_H

#include "GameMap.h"

#include <iostream>
#include<string>

#include<SDL.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include<SDL_ttf.h>

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800

class Engine
{

    public:
        static Engine* GetInstance() {
            return s_Instance = (s_Instance != nullptr)? s_Instance : new Engine();
            //check neu s_Instance la con tro null
        }

        bool Init(); //khoi tao engine
        bool Clean(); //don engine
        void Quit(); //kt qua trinh

        void Update();
        void Render();
        void Events();

        inline bool IsRunning() {return m_IsRunning;} //check xem engine dang chay ko
        inline SDL_Renderer* GetRenderer() {return m_Renderer;}

    private:
        Engine() {};
        bool m_IsRunning;

        GameMap* m_LevelMap;

        SDL_Window* m_Window;
        SDL_Renderer* m_Renderer;

        static Engine* s_Instance;
};

#endif // ENGINE_H
