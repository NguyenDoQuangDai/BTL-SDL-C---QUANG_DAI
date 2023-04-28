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

bool Engine::Init()
{
    if(SDL_Init(SDL_INIT_VIDEO) != 0 && IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) != 0) {//khoi tao he thong con su kien vs tai ho tro dinh dang anh png,jpg
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
    //...ACCELERATED: renderer sd tang toc phan cung
    //...PRESENTVSYNC: hien tai(present) dc dong bo vs refresh rate
    if(m_Renderer == nullptr) {
        SDL_Log("Failed to create Renderer: %s", SDL_GetError());
        return false; //check xem tao renderer thanh cong khong
    }

    if (!MapParser::GetInstance()->Load("Level1")) {
		std::cout << "Failed to Load map!" << std::endl; //ktra load map thanh cong
	}
    m_LevelMap = MapParser::GetInstance()->GetMap("Level1"); //lay map

    TextureManager::GetInstance()->ParseTextures("assets/textures.tml");
    //ham phan tich parse texture lay cac thong so "id" va "path" trong file va goi ham load cac image tu 2 thong so do

    Warrior* player = new Warrior(new Properties("player_idle", 280, 200, 200, 200)); //dat player vs boss bat dau vs trang thai idle

    player->life = 3;
    m_GameObjects.push_back(player);//push cac object vao mang dong GameObjects -> Engine::Render()

    Camera::GetInstance()->SetTarget(player->GetOrigin()); //dat vi tri target cho cameara la trung tam cua player

    m_Point = 0;
    m_HPoint = 3;
    m_Num[5] = {};
    m_NumCount = 1;
    m_const = m_Point;
    m_Volume = 10;

    m_Menu = true;
    m_GamePause = false;
    m_FirstTime = true;
    m_Button0 = true, m_Button1 = false, m_Button2 = false;


    SDL_Init(SDL_INIT_AUDIO);

    if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) != 0) {
        std::cout << "Failed to OpenAudio! Error: %s" << Mix_GetError() << std::endl;
        return false;
    }

    Mix_MasterVolume(32);
    PlaySound("Music");

    return m_IsRunning = true;
    std::cout << "Engine Init sac set phu li!" << std::endl;
}

void Engine::GameOver()
{
    m_GameOver = true;
    m_Checker = false;
    if(Input::GetInstance()->GetKeyDown(SDL_SCANCODE_K)) {
        m_GameOver = false;
        m_HPoint  = 3;
        m_Point = 0;
    }
}

void Engine::Menu()
{
    //draw menu
    if(m_Menu == true) {
        TextureManager::GetInstance()->Draw("backmenu", 0, 0, 6000, 2700, 0.29, 0.29, 0);
        TextureManager::GetInstance()->Draw("name", -15, 20, 360, 240, 1.1, 1.1, 0);
//        TextureManager::GetInstance()->Draw("shade", 400, 300, 348, 241, 1, 1, 0);

        if(m_Button0) TextureManager::GetInstance()->Draw("button2", 25, 300, 3000, 700, 0.1, 0.11, 0);
        else TextureManager::GetInstance()->Draw("button0", 25, 300, 3000, 700, 0.1, 0.11, 0);

        if(m_Button1) TextureManager::GetInstance()->Draw("button2", 25, 450, 3000, 700, 0.1, 0.11, 0);
        else TextureManager::GetInstance()->Draw("button0", 25, 450, 3000, 700, 0.1, 0.11, 0);

        if(m_Button2) TextureManager::GetInstance()->Draw("button2", 25, 600, 3000, 700, 0.1, 0.11, 0);
        else TextureManager::GetInstance()->Draw("button0", 25, 600, 3000, 700, 0.1, 0.11, 0);

        TextureManager::GetInstance()->Draw("playword", 88, 275, 1200, 700, 0.15, 0.16, 0);
        TextureManager::GetInstance()->Draw("guideword", 70, 428, 1200, 700, 0.15, 0.16, 0);
        TextureManager::GetInstance()->Draw("quitword", 80, 580, 1200, 700, 0.15, 0.16, 0);

    //MenuFuction
        //hien thi nut dang duoc chon
        if(m_Button0) {
            if(Input::GetInstance()->GetKeyDown(SDL_SCANCODE_DOWN)) {
                PlaySound("Interface");
                m_Button1 = true;
                m_Button0 = false;
                m_Button2 = false;
                SDL_Delay(150);
            }
            else if(Input::GetInstance()->GetKeyDown(SDL_SCANCODE_UP)) {
                PlaySound("Interface");
                SDL_Delay(150);
            }
        }
        else if(m_Button1) {
            if(Input::GetInstance()->GetKeyDown(SDL_SCANCODE_DOWN)) {
                PlaySound("Interface");
                m_Button2 = true;
                m_Button0 = false;
                m_Button1 = false;
                SDL_Delay(150);
            }
            else if(Input::GetInstance()->GetKeyDown(SDL_SCANCODE_UP)) {
                PlaySound("Interface");
                m_Button0 = true;
                m_Button1 = false;
                m_Button2 = false;
                SDL_Delay(150);
            }
        }
        else if(m_Button2) {
            if(Input::GetInstance()->GetKeyDown(SDL_SCANCODE_UP)) {
                PlaySound("Interface");
                m_Button1 = true;
                m_Button0 = false;
                m_Button2 = false;
                SDL_Delay(150);
            }
            else if(Input::GetInstance()->GetKeyDown(SDL_SCANCODE_DOWN)) {
                PlaySound("Interface");
                SDL_Delay(150);
            }
        }


        //enter button
        if(Input::GetInstance()->GetKeyDown(SDL_SCANCODE_RETURN)) {
            if(m_Button0){
                m_Menu = false;
                PlaySound("Choose");
                m_Button0 = true;
                m_Button1 = false;
                m_Button2 = false;
            }
            else if(m_Button1) {
                m_Board  = !m_Board;
                PlaySound("Interface");
                SDL_Delay(150);
            }
            else {Quit();}
        }
        if(m_Board) {
            TextureManager::GetInstance()->Draw("board", 230, 130, 1200, 632, 0.85, 0.85, 0);
            TextureManager::GetInstance()->Draw("guide", 480, 90, 606, 660, 0.86, 0.86, 0);
        }
    }
    else {m_Board  = false;}
}

void Engine::GamePause()
{
    if(m_Menu == false && m_GameOver  == false) {
        m_GamePause = !m_GamePause;
        SDL_Delay(150);
    }
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
        else if(m_const >= 1000 && m_const <= 9999) {m_NumCount = 4;}
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

        TextureManager::GetInstance()->Draw("Coin", 605, 530, 5000, 5000, 0.032, 0.032, 0); //coin gameover
        for(int i=0; i<m_NumCount; i++) {
            if(m_Num[i] == 0)
                TextureManager::GetInstance()->Draw("0", (600 - (i+1)*60), 550, 2400, 2400, 0.05, 0.05, 0);//...
            else if(m_Num[i] == 1)
                TextureManager::GetInstance()->Draw("1", (600 - (i+1)*60), 550, 2400, 2400, 0.05, 0.05, 0);
            else if(m_Num[i] == 2)
                TextureManager::GetInstance()->Draw("2", (600 - (i+1)*60), 550, 2400, 2400, 0.05, 0.05, 0);
            else if(m_Num[i] == 3)
                TextureManager::GetInstance()->Draw("3", (600 - (i+1)*60), 550, 2400, 2400, 0.05, 0.05, 0);
            else if(m_Num[i] == 4)
                TextureManager::GetInstance()->Draw("4", (600 - (i+1)*60), 550, 2400, 2400, 0.05, 0.05, 0);
            else if(m_Num[i] == 5)
                TextureManager::GetInstance()->Draw("5", (600 - (i+1)*60), 550, 2400, 2400, 0.05, 0.05, 0);
            else if(m_Num[i] == 6)
                TextureManager::GetInstance()->Draw("6", (600 - (i+1)*60), 550, 2400, 2400, 0.05, 0.05, 0);
            else if(m_Num[i] == 7)
                TextureManager::GetInstance()->Draw("7", (600 - (i+1)*60), 550, 2400, 2400, 0.05, 0.05, 0);
            else if(m_Num[i] == 8)
                TextureManager::GetInstance()->Draw("8", (600 - (i+1)*60), 550, 2400, 2400, 0.05, 0.05, 0);
            else if(m_Num[i] == 9)
                TextureManager::GetInstance()->Draw("9", (600 - (i+1)*60), 550, 2400, 2400, 0.05, 0.05, 0);
        }
    }
}

void Engine::Render()
{
    SDL_SetRenderDrawColor(m_Renderer, 0, 0, 100, 255); //dat mau cho hoat dong ve (124, 218, 254)-xanh da troi
    SDL_RenderClear(m_Renderer); //xoa mua tieu render hien tai bang mau ve truoc khi present


    if(m_Menu == false) {
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
            GameOver();
            break;
        }

        if(m_Point == 9999) {GameOver();}

        DrawGamePoint();
        if(!m_GameOver)
            TextureManager::GetInstance()->Draw("esc", -25, 685, 600, 600, 0.32, 0.32, 0);
    }

    Menu();
    if(m_GamePause) {
        //ve gamepause
        TextureManager::GetInstance()->Draw("pause", 100, 100, 1200, 710, 0.85, 0.85, 0);
        TextureManager::GetInstance()->Draw("button1", 465, 30, 3000, 700, 0.1, 0.11, 0);
        TextureManager::GetInstance()->Draw("pauseword", 515, 5, 1200, 700, 0.18, 0.2, 0);
        TextureManager::GetInstance()->Draw("guide", 350, 100, 606, 660, 0.86, 0.86, 0);

        if(m_Button0) TextureManager::GetInstance()->Draw("button2", 250, 700, 3000, 700, 0.1, 0.11, 0);
        else TextureManager::GetInstance()->Draw("button0", 250, 700, 3000, 700, 0.1, 0.11, 0);

        if(m_Button1) TextureManager::GetInstance()->Draw("button2", 673, 700, 3000, 700, 0.1, 0.11, 0);
        else TextureManager::GetInstance()->Draw("button0", 673, 700, 3000, 700, 0.1, 0.11, 0);

        TextureManager::GetInstance()->Draw("continueword", 298, 683, 1200, 700, 0.17, 0.18, 0);
        TextureManager::GetInstance()->Draw("quitword", 730, 685, 1200, 700, 0.15, 0.16, 0);

        //hien thi mau nut dang chon
        if(m_Button0) {
            if(Input::GetInstance()->GetKeyDown(SDL_SCANCODE_RIGHT)) {
                PlaySound("Interface");
                m_Button1 = true;
                m_Button0 = false;
                SDL_Delay(150);
            }
            else if(Input::GetInstance()->GetKeyDown(SDL_SCANCODE_LEFT)) {
                PlaySound("Interface");
                SDL_Delay(150);
            }
        }
        else if(m_Button1) {
            if(Input::GetInstance()->GetKeyDown(SDL_SCANCODE_LEFT)) {
                PlaySound("Interface");
                m_Button0 = true;
                m_Button1 = false;
                SDL_Delay(150);
            }
            else if(Input::GetInstance()->GetKeyDown(SDL_SCANCODE_RIGHT)) {
                PlaySound("Interface");
                SDL_Delay(150);
            }
        }
        //thuc hien lenh button
        if(Input::GetInstance()->GetKeyDown(SDL_SCANCODE_RETURN)) {
            if(m_Button0){
                m_GamePause = false;
                PlaySound("Interface");
                m_Button0 = true;
                m_Button1 = false;
            }
            else {
                PlaySound("Interface");
                SDL_Delay(150);
                Quit();
            }
        }
    }

    SDL_RenderPresent(m_Renderer); //cap nhat man hinh vs moi render ke tuw lan goi truoc

}

void Engine::Update()
{
    float dt = Timer::GetInstance()->GetDeltaTime(); //gan gia tri m_DeltaTime cho dt

    if(m_Menu == false) {
        for(unsigned int i=0; i != m_GameObjects.size(); i++) {
            m_GameObjects[i]->Update(dt); // vong lap update tat ca objects
        }

        Camera::GetInstance()->Update(dt); //update camera theo dt
        m_LevelMap->Update();
    }

    ChangeVolume();
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
    else if(sound == "Choose")
        Mix_PlayChannel(-1, Mix_LoadWAV("assets/Sound/GameStartSound.wav"), 0);
    else if(sound == "GameOver")
        Mix_PlayChannel(-1, Mix_LoadWAV("assets/Sound/GameOverSound.wav"), 0);
    else if(sound == "Interface")
        Mix_PlayChannel(-1, Mix_LoadWAV("assets/Sound/InterfaceSound.wav"), 0);
    else if(sound == "Music")
        Mix_PlayChannel(-1, Mix_LoadWAV("assets/Sound/Music.wav"), -1);
    else if(sound == "Slash")
        Mix_PlayChannel(-1, Mix_LoadWAV("assets/Sound/SlashSound.wav"), 0);
}

void Engine::ChangeVolume()
{
    if(Input::GetInstance()->GetKeyDown(SDL_SCANCODE_0)) {
        if(m_Volume != 0) {m_Volume = 0;}
        else{m_Volume = 10;}
        SDL_Delay(150);
    }
    else if(Input::GetInstance()->GetKeyDown(SDL_SCANCODE_1)) {
        m_Volume = 1;
        SDL_Delay(100);
    }
    else if(Input::GetInstance()->GetKeyDown(SDL_SCANCODE_2)) {
        m_Volume = 2;
        SDL_Delay(100);
    }
    else if(Input::GetInstance()->GetKeyDown(SDL_SCANCODE_3)) {
        m_Volume = 3;
        SDL_Delay(100);
    }
    else if(Input::GetInstance()->GetKeyDown(SDL_SCANCODE_4)) {
        m_Volume = 4;
        SDL_Delay(100);
    }
    else if(Input::GetInstance()->GetKeyDown(SDL_SCANCODE_5)) {
        m_Volume = 5;
        SDL_Delay(100);
    }
    else if(Input::GetInstance()->GetKeyDown(SDL_SCANCODE_6)) {
        m_Volume = 6;
        SDL_Delay(100);
    }
    else if(Input::GetInstance()->GetKeyDown(SDL_SCANCODE_7)) {
        m_Volume = 7;
        SDL_Delay(100);
    }
    else if(Input::GetInstance()->GetKeyDown(SDL_SCANCODE_8)) {
        m_Volume = 8;
        SDL_Delay(100);
    }
    else if(Input::GetInstance()->GetKeyDown(SDL_SCANCODE_9)) {
        m_Volume = 9;
        SDL_Delay(100);
    }

    Mix_MasterVolume(3.2*m_Volume);
}


bool Engine::Clean()
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
