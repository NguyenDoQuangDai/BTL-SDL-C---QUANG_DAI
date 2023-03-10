#include <iostream>
#include<SDL.h>
#include<SDL_image.h>

const unsigned int _SCREEN_WIDTH_ = 800;
const unsigned int _SCREEN_HEIGHT_ = 600;

using namespace std;

SDL_Texture* loadTexture(const char* path, SDL_Renderer* renderer);

int main(int argc,char *argv[])
{
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    window = SDL_CreateWindow("quang dai", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _SCREEN_WIDTH_, _SCREEN_HEIGHT_, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);


    SDL_Texture* background = nullptr;
    background = loadTexture("Doge.png", renderer);

    SDL_Rect bgRect;
    bgRect.x = 0;
    bgRect.y = 0;
    bgRect.w = _SCREEN_WIDTH_;
    bgRect.h = _SCREEN_HEIGHT_;

    SDL_Event event;
    bool isRunning = true;
    while (isRunning) {
        SDL_RenderCopy(renderer, background, NULL, &bgRect);
        SDL_RenderPresent(renderer);
        while (SDL_PollEvent(&event)){
            switch (event.type){
            case SDL_QUIT:
               isRunning = false;
               break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    isRunning = false;
                    break;
                }
            }
        }
    }

    return 0;
}

SDL_Texture* loadTexture(const char* path, SDL_Renderer* renderer) {
    SDL_Texture* newTexture = nullptr;
    newTexture = IMG_LoadTexture (renderer, path);
    return newTexture;
}
