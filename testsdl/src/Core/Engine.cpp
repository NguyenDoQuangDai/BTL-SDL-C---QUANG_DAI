#include "Engine.h"
#include "TextureManager.h"
#include "Vector2D.h"
#include "Transform.h"
#include "Warrior.h"

#include <iostream>
#include<string>

#include<SDL.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include<SDL_ttf.h>

using namespace std;

Engine* Engine::s_Instance = nullptr;
Warrior* player = nullptr;

bool Engine::Init()
{
    if(SDL_Init(SDL_INIT_VIDEO) != 0 && IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) != 0) {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return false; //check xem khoi tao sdl thanh cong khoong
    }

    m_Window = SDL_CreateWindow("Soft Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if(m_Window == nullptr) {
       SDL_Log("Failed to create Window: %s", SDL_GetError());
        return false; //check xem tao window thanh cong khong
    }

    m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(m_Renderer == nullptr) {
        SDL_Log("Failed to create Renderer: %s", SDL_GetError());
        return false; //check xem tao renderer thanh cong khong
    }

    TextureManager::GetInstance()->Load("player", "assets/Attack1.png"); //load image
    player = new Warrior(new Properties("player", 0, 0, 200, 200));

    Transform tf;
    tf.Log();

    return m_IsRunning = true;
}

void Engine::Update()
{
    player->Update(0);
}

void Engine::Render()
{
    SDL_SetRenderDrawColor(m_Renderer, 124, 218, 254, 255);
    SDL_RenderClear(m_Renderer);

    player->Draw();
    SDL_RenderPresent(m_Renderer);
}

void Engine::Events()
{
    SDL_Event event;
    SDL_PollEvent(&event);
    switch(event.type) {
    case SDL_QUIT:
        Quit();
        break;
    }
}

bool Engine::Clean()
{
    TextureManager::GetInstance()->Clean(); //don moi texture
    SDL_DestroyRenderer(m_Renderer);
    SDL_DestroyWindow(m_Window); //xoa renderer vs cua so gp bo nho
    IMG_Quit();
    SDL_Quit();
}

void Engine::Quit()
{
    m_IsRunning = false;
}
