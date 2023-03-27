#ifndef INPUT_H
#define INPUT_H

#include<SDL.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include<SDL_ttf.h>

class Input
{
    public:
        static Input* GetInstance() {
            return s_Instance = (s_Instance != nullptr)? s_Instance : new Input();
            //check neu s_Instance la con tro null
        }

        void Listen();
        bool GetKeyDown(SDL_Scancode key);

    private:
        Input();
        void KeyUp();
        void KeyDown();

        const Uint8* m_KeyStates;
        static Input* s_Instance;
};

#endif // INPUT_H
