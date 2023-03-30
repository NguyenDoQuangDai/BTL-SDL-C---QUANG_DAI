#include "Engine.h"
#include "TextureManager.h"
#include "Vector2D.h"
#include "Transform.h"
#include "Warrior.h"
#include "Input.h"
#include "Timer.h"
#include "MapParser.h"
#include "Camera.h"

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

	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

    m_Window = SDL_CreateWindow("Soft Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, window_flags);
    if(m_Window == nullptr) {
       SDL_Log("Failed to create Window: %s", SDL_GetError());
        return false; //check xem tao window thanh cong khong
    }

    m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(m_Renderer == nullptr) {
        SDL_Log("Failed to create Renderer: %s", SDL_GetError());
        return false; //check xem tao renderer thanh cong khong
    }

    if (!MapParser::GetInstance()->Load()) {
		std::cout << "Failed to Load map!" << std::endl;
	}
    m_LevelMap = MapParser::GetInstance()->GetMap("Level1");

    TextureManager::GetInstance()->Load("player", "assets/Idle.png"); //load frame img
    TextureManager::GetInstance()->Load("player_run", "assets/Run.png");
    TextureManager::GetInstance()->Load("player_jump", "assets/Jump.png");
    TextureManager::GetInstance()->Load("background", "assets/Image/Doge.png");
    player = new Warrior(new Properties("player", 320, 285, 200, 200));

    Transform tf;
    tf.Log();

    Camera::GetInstance()->SetTarget(player->GetOrigin());

    return m_IsRunning = true;
}

void Engine::Update()
{
    float dt = Timer::GetInstance()->GetDeltaTime();
    m_LevelMap->Update();
    player->Update(dt);
    Camera::GetInstance()->Update(dt);
}

void Engine::Render()
{
    SDL_SetRenderDrawColor(m_Renderer, 124, 218, 254, 255);
    SDL_RenderClear(m_Renderer);

    TextureManager::GetInstance()->Draw("background", 0, 0, 1920, 1080);

    m_LevelMap->Render();
    player->Draw();
    SDL_RenderPresent(m_Renderer);
}

void Engine::Events()
{
    Input::GetInstance()->Listen();
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
