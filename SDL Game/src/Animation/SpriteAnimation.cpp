#include "SpriteAnimation.h"
#include "TextureManager.h"

SpriteAnimation::SpriteAnimation(bool repeat): Animation(repeat) {}

void SpriteAnimation::DrawFrame(float x, float y, int width, int height, float xScale, float yScale, SDL_RendererFlip flip)
{
    TextureManager::GetInstance()->DrawFrame(m_TextureID, x, y, width, height, m_SpriteRow, m_CurrentFrame, flip); //ve frame
}

void SpriteAnimation::Update(float dt)
{
    m_CurrentFrame = (SDL_GetTicks()/m_Speed) % m_FrameCount;
    //frame hien tai = (thoi gian tu dau / SPF) % so frame cua sheet
    //speed la thoi gian frame duoc xuat hien(Second Per Frame)
}

void SpriteAnimation::SetProps(std::string textureID, int spriteRow, int frameCount, int speed) //dat props thanh trang thai nv(vd: IsRunning)
{
    m_Speed = speed;
    m_TextureID = textureID;
    m_SpriteRow = spriteRow;
    m_FrameCount = frameCount;
}
