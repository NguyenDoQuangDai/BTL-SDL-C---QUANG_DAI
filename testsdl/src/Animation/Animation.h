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
        void Draw(float x, float y, int spritWidth, int spritHeight);
        void SetProps(std::string textureID, int spritRow, int frameCount, int animSpeed, SDL_RendererFlip flip = SDL_FLIP_NONE);

    private:
        int m_SpriteRow, m_SpriteFrame, m_AnimSpeed, m_FrameCount;
        std::string m_TextureID;
        SDL_RendererFlip m_Flip;
};

#endif // ANIMATION_H
