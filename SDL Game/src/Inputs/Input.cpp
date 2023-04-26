#include "Input.h"
#include "Engine.h"

Input* Input::s_Instance = nullptr; //khoi tao cho gia tri static Instance trc moi thu

Input::Input()
{
    m_KeyStates = SDL_GetKeyboardState(nullptr);
}

void Input::Listen()
{
    SDL_Event event;

    //SDL_Event la tong hop tat ca cac cau truc events su kien dc su dung trong SDL
    //SDL_PollEvent tra ve 1 neu 1 event dang xay ra
    while(SDL_PollEvent(&event)) { //check gia tri cua cac events xay ra
        switch(event.type) {
            case SDL_QUIT: Engine::GetInstance()->Quit(); break;
            case SDL_KEYDOWN: KeyDown(); break;
            case SDL_KEYUP: KeyUp(); break;
        }
    }
}

bool Input::GetKeyDown(SDL_Scancode key)
{
    if(m_KeyStates[key] == 1) {
        return true; //neu nut dc nhan -> return true va ngc lai
    }
    return false;
}

//SDL_GetKeyBoardState: nhan anh chup nhanh(snapshot) trang thai htai cua ban phim -> (neu ko Null) return con tro vao chuoi KeyStates
void Input::KeyUp()
{
    m_KeyStates = SDL_GetKeyboardState(nullptr); //khoi tao keystate
}

void Input::KeyDown()
{
    m_KeyStates = SDL_GetKeyboardState(nullptr);
}

int Input::GetAxisKey(Axis axis)
{
    switch(axis){
        case HORIZONTAL: //di chuyen ngang
            if(GetKeyDown(SDL_SCANCODE_D))
                return 1;
            if(GetKeyDown(SDL_SCANCODE_A))
                return -1;
            break;
        case VERTICAL: //di chuyen doc
            if(GetKeyDown(SDL_SCANCODE_W))
                return 1;
            if(GetKeyDown(SDL_SCANCODE_S))
                return -1;
            break;

        default:
            return 0;
    }
}
