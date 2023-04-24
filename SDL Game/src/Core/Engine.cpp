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


Engine* Engine::s_Instance = nullptr;
Warrior* player = nullptr; //tao object
//gan nullptr cho bien con tro static

bool Engine::Init(std::string LevelX)
{
    if(SDL_Init(SDL_INIT_VIDEO) != 0 && IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) != 0) { //khoi tao he thong con su kien vs tai ho tro dinh dang anh png,jpg
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return false; //check xem khoi tao sdl thanh cong khoong
    }

	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_ALLOW_HIGHDPI); //bang cac trang thai cua so,chuan bi cua so de sd voi OpenGL
    //window se dc tao o dpi cao neu ho tro va co the thay doi kich co neu them SDL_WINDOW_RESIZABLE

    m_Window = SDL_CreateWindow("NinjaParkua Daisekai", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, window_flags); //tao window vs xy,wh
    if(m_Window == nullptr) {
       SDL_Log("Failed to create Window: %s", SDL_GetError());
        return false; //check xem tao window thanh cong khong
    }

    m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); //tao khung 2d cho cua so window
    if(m_Renderer == nullptr) {
        SDL_Log("Failed to create Renderer: %s", SDL_GetError());
        return false; //check xem tao renderer thanh cong khong
    }

    if (!MapParser::GetInstance()->Load(LevelX)) {
		std::cout << "Failed to Load map!" << std::endl; //ktra load map thanh cong
	}
    m_LevelMap = MapParser::GetInstance()->GetMap(LevelX); //lay map

    TextureManager::GetInstance()->ParseTextures("assets/textures.tml");
    //ham phan tich parse texture va load cac image voi "id" va "path" trong file

    Warrior* player = new Warrior(new Properties("player_idle", 280, 200, 200, 200)); //dat player vs boss bat dau vs trang thai idle

    player->life = 3;
    m_GameObjects.push_back(player);//push cac object vao mang dong GameObjects -> Engine::Render()

    Camera::GetInstance()->SetTarget(player->GetOrigin()); //dat vi tri target cho cameara la trung tam cua player

    std::cout << "Engine Init sac set phu li!" << std::endl;

    int m_Point = 0;
    int m_HPoint = 3;
    int m_Num[5] = {};
    int m_NumCount = 1;
    int m_const = m_Point;

    bool m_FirstTime = true;

    SDL_Init(SDL_INIT_AUDIO);

    if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) != 0) {
        std::cout << "Failed to OpenAudio! Error: %s" << Mix_GetError() << std::endl;
        return false;
    }

    Mix_MasterVolume(12);
    PlaySound("Music");

    return m_IsRunning = true;
}

void Engine::GameOver()
{
        m_GameOver = true;
        if(Input::GetInstance()->GetKeyDown(SDL_SCANCODE_K)) {
            m_GameOver = false;
            m_HPoint  = 3;
            m_Point = 0;
        }
        m_Checker = false;
}

void Engine::Check()
{
    m_Checker = !m_Checker;
}

void Engine::StatusDown()
{
    if(m_HPoint >= 1 && m_GameOver == false)
        m_HPoint--;
/*
    if(m_Point >= 1 && m_GameOver == false)
        m_Point--;
*/
}

void Engine::StatusUp()
{
    m_Point++;
    PlaySound("Checker");
}

void Engine::GamePoint()
{
    if(m_const != m_Point || m_FirstTime == true){
        m_FirstTime = false;
        m_const = m_Point;

        if(m_const >= 10 && m_const <= 99) {m_NumCount = 2;}
        else if(m_const >= 100 && m_const <= 999) {m_NumCount = 3;}
        else {m_NumCount = 1;}

        m_const = m_Point;

        for(int i=0; i < m_NumCount; i++) {
            m_Num[i] = m_const % 10;
            m_const /= 10;
        }
        m_const = m_Point;
    }
}

void Engine::DrawGamePoint()
{
    if(!m_GameOver) {
        TextureManager::GetInstance()->Draw("Coin", 1106, 14, 5000, 5000, 0.016, 0.016, 0); // coin tren goc
        for(int i=0; i<m_NumCount; i++) {
            if(m_Num[i] == 0)
                TextureManager::GetInstance()->Draw("0", (1100 - (i+1)*30), 24, 2400, 2400, 0.025, 0.025, 0); //diem so o goc
            else if(m_Num[i] == 1)
                TextureManager::GetInstance()->Draw("1", (1100 - (i+1)*30), 24, 2400, 2400, 0.025, 0.025, 0);
            else if(m_Num[i] == 2)
                TextureManager::GetInstance()->Draw("2", (1100 - (i+1)*30), 24, 2400, 2400, 0.025, 0.025, 0);
            else if(m_Num[i] == 3)
                TextureManager::GetInstance()->Draw("3", (1100 - (i+1)*30), 24, 2400, 2400, 0.025, 0.025, 0);
            else if(m_Num[i] == 4)
                TextureManager::GetInstance()->Draw("4", (1100 - (i+1)*30), 24, 2400, 2400, 0.025, 0.025, 0);
            else if(m_Num[i] == 5)
                TextureManager::GetInstance()->Draw("5", (1100 - (i+1)*30), 24, 2400, 2400, 0.025, 0.025, 0);
            else if(m_Num[i] == 6)
                TextureManager::GetInstance()->Draw("6", (1100 - (i+1)*30), 24, 2400, 2400, 0.025, 0.025, 0);
            else if(m_Num[i] == 7)
                TextureManager::GetInstance()->Draw("7", (1100 - (i+1)*30), 24, 2400, 2400, 0.025, 0.025, 0);
            else if(m_Num[i] == 8)
                TextureManager::GetInstance()->Draw("8", (1100 - (i+1)*30), 24, 2400, 2400, 0.025, 0.025, 0);
            else if(m_Num[i] == 9)
                TextureManager::GetInstance()->Draw("9", (1100 - (i+1)*30), 24, 2400, 2400, 0.025, 0.025, 0);
        }
    }
    else {
        TextureManager::GetInstance()->Draw("GameOver", 240, 0, 1200, 1200, 0.6, 0.6);

        TextureManager::GetInstance()->Draw("Coin", 616, 530, 5000, 5000, 0.032, 0.032, 0); //coin gameover
        for(int i=0; i<m_NumCount; i++) {
            if(m_Num[i] == 0)
                TextureManager::GetInstance()->Draw("0", (570 - (i+1)*30), 550, 2400, 2400, 0.05, 0.05, 0);//...
            else if(m_Num[i] == 1)
                TextureManager::GetInstance()->Draw("1", (570 - (i+1)*30), 550, 2400, 2400, 0.05, 0.05, 0);
            else if(m_Num[i] == 2)
                TextureManager::GetInstance()->Draw("2", (570 - (i+1)*30), 550, 2400, 2400, 0.05, 0.05, 0);
            else if(m_Num[i] == 3)
                TextureManager::GetInstance()->Draw("3", (570 - (i+1)*30), 550, 2400, 2400, 0.05, 0.05, 0);
            else if(m_Num[i] == 4)
                TextureManager::GetInstance()->Draw("4", (570 - (i+1)*30), 550, 2400, 2400, 0.05, 0.05, 0);
            else if(m_Num[i] == 5)
                TextureManager::GetInstance()->Draw("5", (570 - (i+1)*30), 550, 2400, 2400, 0.05, 0.05, 0);
            else if(m_Num[i] == 6)
                TextureManager::GetInstance()->Draw("6", (570 - (i+1)*30), 550, 2400, 2400, 0.05, 0.05, 0);
            else if(m_Num[i] == 7)
                TextureManager::GetInstance()->Draw("7", (570 - (i+1)*30), 550, 2400, 2400, 0.05, 0.05, 0);
            else if(m_Num[i] == 8)
                TextureManager::GetInstance()->Draw("8", (570 - (i+1)*30), 550, 2400, 2400, 0.05, 0.05, 0);
            else if(m_Num[i] == 9)
                TextureManager::GetInstance()->Draw("9", (570 - (i+1)*30), 550, 2400, 2400, 0.05, 0.05, 0);
        }
    }
}

void Engine::Render()
{
    SDL_SetRenderDrawColor(m_Renderer, 124, 218, 254, 255); //dat mau cho hoat dong ve (124, 218, 254)-xanh da troi nhat
    SDL_RenderClear(m_Renderer); //xoa mua tieu render hien tai bang mau ve truoc khi present
    m_LevelMap->Render();//render map
    TextureManager::GetInstance()->Draw("back", 0, 0, 8000, 2462, 0.2373, 0.2373, 0.35);
    //draw anh backgound da load voi kich co x0.3 va toc do ve phia sau = 0,3 nhan vat
    m_LevelMap->Render();//render map

    GamePoint();

    if(m_Checker == true) {
        TextureManager::GetInstance()->Draw("Coin", 412, 306, 5000, 5000, 0.008, 0.008, 1);
        TextureManager::GetInstance()->Draw("Check", 2730, 348, 800, 800, 0.017, 0.017, 1);
        TextureManager::GetInstance()->Draw("Check", 1352, 464, 800, 800, 0.0135, 0.0135, 1);
    }
    else {
        TextureManager::GetInstance()->Draw("Coin", 2891, 230, 5000, 5000, 0.008, 0.008, 1);
        TextureManager::GetInstance()->Draw("Check", 490, 379, 800, 800, 0.017, 0.017, 1);
        TextureManager::GetInstance()->Draw("Check", 1357, 480, 800, 800, 0.0135, 0.0135, 1);
    }

    for(unsigned int i=0; i != m_GameObjects.size(); i++) {
        m_GameObjects[i]->Draw(); // vong lap ve moi objects len man hinh screen
    }


    switch(m_HPoint) {
    case 3:
        TextureManager::GetInstance()->Draw("3HPoint", 0, 0, 2400, 800, 0.1, 0.1, 0);
        break;
    case 2:
        TextureManager::GetInstance()->Draw("2HPoint", 0, 0, 2400, 800, 0.1, 0.1, 0);
        break;
    case 1:
        TextureManager::GetInstance()->Draw("1HPoint", 0, 0, 2400, 800, 0.1, 0.1, 0);
        break;
    default:
        TextureManager::GetInstance()->Draw("0HPoint", 0, 0, 2400, 800, 0.1, 0.1, 0);
        break;
    }

    DrawGamePoint();

    SDL_RenderPresent(m_Renderer); //cap nhat man hinh vs moi render ke tuw lan goi truoc
}

void Engine::Update()
{
    if(true) {
        float dt = Timer::GetInstance()->GetDeltaTime(); //gan gia tri m_DeltaTime cho dt

        for(unsigned int i=0; i != m_GameObjects.size(); i++) {
            m_GameObjects[i]->Update(dt); // vong lap update tat ca objects
        }

        Camera::GetInstance()->Update(dt); //update camera theo dt
        m_LevelMap->Update();
    }
}

void Engine::Events()
{
    Input::GetInstance()->Listen(); //ktra neu events xay ra
}


void Engine::PlaySound(std::string sound)
{
    if(sound == "Checker")
        Mix_PlayChannel(-1, Mix_LoadWAV("assets/Sound/CoinSound.wav"), 0);
    else if(sound == "Jump")
        Mix_PlayChannel(-1, Mix_LoadWAV("assets/Sound/JumpSound.wav"), 0);
    else if(sound == "Fall")
        Mix_PlayChannel(-1, Mix_LoadWAV("assets/Sound/Fall.wav"), 0);
    else if(sound == "Die")
        Mix_PlayChannel(-1, Mix_LoadWAV("assets/Sound/DieSound.wav"), 0);
    else if(sound == "GameStart")
        Mix_PlayChannel(-1, Mix_LoadWAV("assets/Sound/GameStartSound.wav"), 0);
    else if(sound == "GameOver")
        Mix_PlayChannel(-1, Mix_LoadWAV("assets/Sound/GameOverSound.wav"), 0);
    else if(sound == "Interface")
        Mix_PlayChannel(-1, Mix_LoadWAV("assets/Sound/InterfaceSound.wav"), 0);
    else if(sound == "Music")
        Mix_PlayChannel(-1, Mix_LoadWAV("assets/Sound/BackMusic.wav"), -1);
    else if(sound == "Slash")
        Mix_PlayChannel(-1, Mix_LoadWAV("assets/Sound/SlashSound.wav"), 0);
}


bool Engine::Clean(std::string LevelX)
{
    for(unsigned int i=0; i != m_GameObjects.size(); i++) {
        m_GameObjects[i]->Clean(); //clean moi objects
    }

    TextureManager::GetInstance()->Clean(); //don moi texture
    MapParser::GetInstance()->Clean();

    SDL_DestroyRenderer(m_Renderer); //xoa renderer vs cua so gp bo nho
    SDL_Delay(500);
    SDL_DestroyWindow(m_Window);
    IMG_Quit(); // khu khoi tao SDL_Image
    SDL_Quit(); //don moi he thong con da khoi tao
}

void Engine::Quit()
{
    TTF_Quit();
    m_IsRunning = false;
}
