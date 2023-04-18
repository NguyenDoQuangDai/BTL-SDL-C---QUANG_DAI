#include "Engine.h"
#include "TextureManager.h"
#include "Vector2D.h"
#include "Transform.h"
#include "Warrior.h"
#include "Input.h"
#include "Timer.h"
#include "MapParser.h"
#include "Camera.h"
#include "Enemy.h"

#include <iostream>
#include<string>

using namespace std;

Engine* Engine::s_Instance = nullptr;
Warrior* player = nullptr; //tao object
//gan nullptr cho bien con tro static

bool Engine::Init()
{
    if(SDL_Init(SDL_INIT_VIDEO) != 0 && IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) != 0) { //khoi tao he thong con su kien vs tai ho tro dinh dang anh png,jpg
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return false; //check xem khoi tao sdl thanh cong khoong
    }

	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI); //bang cac trang thai cua so,chuan bi cua so de sd voi OpenGL
    //window se dc tao o dpi cao neu ho tro va co the thay doi kich co
    m_Window = SDL_CreateWindow("Soft Engine Daisekai", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, window_flags); //tao window vs xy,wh
    if(m_Window == nullptr) {
       SDL_Log("Failed to create Window: %s", SDL_GetError());
        return false; //check xem tao window thanh cong khong
    }

    m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); //tao khung 2d cho cua so window
    if(m_Renderer == nullptr) {
        SDL_Log("Failed to create Renderer: %s", SDL_GetError());
        return false; //check xem tao renderer thanh cong khong
    }

    if (!MapParser::GetInstance()->Load()) {
		std::cout << "Failed to Load map!" << std::endl; //ktra load map thanh cong
	}
    m_LevelMap = MapParser::GetInstance()->GetMap("Level1"); //lay map

    TextureManager::GetInstance()->ParseTextures("assets/textures.tml");
    //ham phan tich parse texture va load image voi "id" va "path" trong file


    Warrior* player = new Warrior(new Properties("player_idle", 400, 200, 200, 200)); //dat player bat dau vs trang thai idle
    Enemy* boss1 = new Enemy(new Properties("boss_idle", 400, 200, 128, 96));

    m_GameObjects.push_back(player);
    m_GameObjects.push_back(boss1);//push cac object vao mang dong GameObjects -> Engine::Render()

    Camera::GetInstance()->SetTarget(player->GetOrigin()); //dat vi tri target cho cameara la trung tam cua player
    return m_IsRunning = true;
}

void Engine::Render()
{
    SDL_SetRenderDrawColor(m_Renderer, 124, 218, 254, 255); //dat mau cho hoat dong ve (124, 218, 254)-xanh da troi nhat
    SDL_RenderClear(m_Renderer); //xoa mua tieu render hien tai bang mau ve truoc khi present

    TextureManager::GetInstance()->Draw("background", 0, 0, 1920, 1080, 1.1, 1.1, 0.3);
    //draw anh backgound da load voi kich co x1.1 va toc do = 0,3 nhan vat
    m_LevelMap->Render();//render map

    for(unsigned int i=0; i != m_GameObjects.size(); i++) {
        m_GameObjects[i]->Draw(); // vong lap ve moi objects len man hinh screen
    }

    SDL_RenderPresent(m_Renderer); //cap nhat man hinh vs moi render ke tuw lan goi truoc
}

void Engine::Update()
{
    float dt = Timer::GetInstance()->GetDeltaTime(); //gan gia tri m_DeltaTime cho dt

    for(unsigned int i=0; i != m_GameObjects.size(); i++) {
        m_GameObjects[i]->Update(dt); // vong lap update tat ca objects
    }

    Camera::GetInstance()->Update(dt); //update camera theo dt
    m_LevelMap->Update();
}

void Engine::Events()
{
    Input::GetInstance()->Listen(); //ktra neu events xay ra
}

bool Engine::Clean()
{
    for(unsigned int i=0; i != m_GameObjects.size(); i++) {
        m_GameObjects[i]->Clean(); //clean moi objects
    }

    TextureManager::GetInstance()->Clean(); //don moi texture
    MapParser::GetInstance()->Clean();

    SDL_DestroyRenderer(m_Renderer); //xoa renderer vs cua so gp bo nho
    SDL_DestroyWindow(m_Window);
    IMG_Quit(); // khu khoi tao SDL_Image
    SDL_Quit(); //don moi he thong con da khoi tao
}

void Engine::Quit()
{
    m_IsRunning = false;
}
