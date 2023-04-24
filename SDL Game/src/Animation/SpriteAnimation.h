#ifndef SPRITEANIMATION_H
#define SPRITEANIMATION_H

#include "Animation.h"

#include <iostream>
#include<string>

#include<SDL.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include<SDL_ttf.h>

class SpriteAnimation : public Animation { //thua ke

    public:
        SpriteAnimation(bool repeat = true); //chung tham so nhu Animation

        virtual void Update(float dt);
        void Draw(float x, float y, int spriteWidth, int spriteHeight, float xScale = 1, float yScale = 1, SDL_RendererFlip flip = SDL_FLIP_NONE);
        void SetProps(std::string textureID, int spriteRow, int frameCount, int speed); //dat props thanh...(vd: IsRunning)

    private:
        int  m_Speed, m_SpriteRow, m_FrameCount, m_CurrentFrame;
        std::string m_TextureID;
};

#endif // SPRITEANIMATION_H
