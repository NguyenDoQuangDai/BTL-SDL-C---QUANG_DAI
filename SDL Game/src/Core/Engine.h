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

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800

class Engine
{
    public:
        static Engine* GetInstance() {
            return s_Instance = (s_Instance != nullptr)? s_Instance : new Engine();
            //check neu s_Instance la con tro null -> tao new instance cho class. neu ko thi return => luon chi co 1 Instance cua class input nay
        }// singleton class

        bool Init(); //khoi tao engine
        bool Clean(); //don engine
        void Quit(); //kt qua trinh

        void Update();
        void Render();
        void Events();

        void GameOver();
        void Menu();
        void GamePause();

        void Check();
        void StatusDown();
        void StatusUp();
        void GamePoint();
        void DrawGamePoint();

        void PlaySound(std::string sound);

        inline GameMap* GetMap() {return m_LevelMap;} //return levelmap
        inline bool IsRunning() {return m_IsRunning;} //check xem engine dang chay ko
        inline SDL_Renderer* GetRenderer() {return m_Renderer;} //return doi tuong render


    private:
        Engine() {}; //-> ko the tao instance cua Engine
        bool m_IsRunning;

        bool m_Menu;
        bool m_Board = false;
        bool m_GamePause;
        bool m_Button0, m_Button1, m_Button2;

        bool m_GameOver = false;
        bool m_Checker = false;

        bool m_FirstTime;

        int m_Point;
        int m_HPoint;
        int m_Num[5];
        int m_NumCount;
        int m_const;


        GameMap* m_LevelMap;
        SDL_Window* m_Window;
        SDL_Renderer* m_Renderer;

        static Engine* s_Instance;
/*
        static Mix_Chunk* m_SoundChecker;
        static Mix_Chunk* m_SoundJump;
        static Mix_Chunk* m_SoundFall;
        static Mix_Chunk* m_SoundDie;
        static Mix_Chunk* m_SoundInterface;
        static Mix_Chunk* m_SoundGameStart;
        static Mix_Chunk* m_SoundGameOver;
*/

        std::vector<GameObject*> m_GameObjects; //moi object deu dc them vao mang GameObject list
};

#endif // ENGINE_H
