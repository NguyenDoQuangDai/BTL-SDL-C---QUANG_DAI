#ifndef INPUT_H
#define INPUT_H

#include<SDL.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include<SDL_ttf.h>

enum Axis {HORIZONTAL, VERTICAL};

class Input
{
    public:
        static Input* GetInstance() {
            return s_Instance = (s_Instance != nullptr)? s_Instance : new Input();
            //check neu s_Instance la con tro null
        }

        void Listen(); //xem co event xay ra ko
        bool GetKeyDown(SDL_Scancode key); //ktra co phai nut key dc nhan

        int GetAxisKey(Axis axis); //nhan event nhan nut di chuyen ngang doc

    private:
        Input();
        void KeyUp(); //nha phim
        void KeyDown(); //nhan nut phim

        const Uint8* m_KeyStates; //luu trang thai key
        static Input* s_Instance;
};

#endif // INPUT_H
