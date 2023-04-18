#ifndef SEQANIMATION_H
#define SEQANIMATION_H

#include "Animation.h"

#include<map>
#include<string>
#include<vector>

#include<SDL.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include<SDL_ttf.h>

struct Sequence {
    int Speed;
    int FrameCount;
    int Width, Height;
    std::vector<std::string> TextureIDs; //mang luu tru cac id texture cua cac seq image
};

class SeqAnimation: public Animation { //thua ke

    public:
        SeqAnimation(bool repeat = true);

        virtual void Update(float dt);
        void Parse(std::string source);
        void SetCurrentSeq(std::string seqID); //thay doi sequence bang cach dung seqID
        void SetRepeat(bool repeat) {m_Repeat = repeat;}
        void DrawFrame(float x, float y, float xScale = 1, float yScale = 1, SDL_RendererFlip flip = SDL_FLIP_NONE);

    private:
        Sequence m_CurrentSeq; //sequence hien tai cua nv
        std::map<std::string, Sequence> m_SeqMap; //map chua moi sequences cua thanh phan
};

#endif // SEQANIMATION_H
