#ifndef ANIMATION_H
#define ANIMATION_H

#include <iostream>
#include<string>

#include<SDL.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include<SDL_ttf.h>

class Animation
{
    public:
        Animation(){};

        void Update();
        void Draw(float x, float y, int spritWidth, int spritHeight, SDL_RendererFlip flip = SDL_FLIP_NONE);
        void SetProps(std::string textureID, int spritRow, int frameCount, int speed);

    private:
        int m_SpriteRow, m_CurrentFrame, m_Speed, m_FrameCount;
        std::string m_TextureID;
};

#endif // ANIMATION_H
