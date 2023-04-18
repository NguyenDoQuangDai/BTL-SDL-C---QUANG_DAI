#ifndef ENGINE_H
#define ENGINE_H

#include "GameMap.h"
#include "GameObject.h"

#include<iostream>
#include<string>
#include<vector>

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
            //check neu s_Instance la con tro null -> return => luon chi co 1 Instance cua class input nay
        }// singleton class

        bool Init(); //khoi tao engine
        bool Clean(); //don engine
        void Quit(); //kt qua trinh

        void Update();
        void Render();
        void Events();

        inline GameMap* GetMap() {return m_LevelMap;} //return levelmap
        inline bool IsRunning() {return m_IsRunning;} //check xem engine dang chay ko
        inline SDL_Renderer* GetRenderer() {return m_Renderer;} //return doi tuong render

    private:
        Engine() {}; //-> ko the tao instance cua Engine
        bool m_IsRunning;

        GameMap* m_LevelMap;
        SDL_Window* m_Window;
        SDL_Renderer* m_Renderer;

        static Engine* s_Instance;

        std::vector<GameObject*> m_GameObjects; //moi object deu dc them vao mang GameObject list
};

#endif // ENGINE_H
