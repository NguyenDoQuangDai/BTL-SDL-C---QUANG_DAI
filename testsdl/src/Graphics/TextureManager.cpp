#include "TextureManager.h"
#include "Engine.h"
#include "Camera.h"
#include "tinyxml.h"

#include <iostream>
#include<string>

#include<SDL.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include<SDL_ttf.h>

TextureManager* TextureManager::s_Instance = nullptr;

bool TextureManager::Load(std::string id, std::string filename)
{
    SDL_Surface* surface = IMG_Load(filename.c_str()); //load anh thanh surface roi tra kq dang texture
    if(surface == nullptr) {
        SDL_Log("Failed to load texture: %s, %s", filename.c_str(), SDL_GetError());
        return false;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(Engine::GetInstance()->GetRenderer(), surface); //tao texture
    if(texture == nullptr) {
        SDL_Log("Failed to create texture from surface: %s", SDL_GetError());
        return false;
    }

    m_TextureMap[id] = texture;

    return true;
}

void TextureManager::Draw(std::string id, int x, int y, int width, int height, float scaleX, float scaleY, float scrollRatio, SDL_RendererFlip flip)
{   SDL_Rect srcRect = {0,  0, width, height};
    Vector2D cam = Camera::GetInstance()->GetPosition()*scrollRatio; //tuy thuoc scrollRatio ma background chuyen dong theo nv nhanh/cham
    SDL_Rect dstRect = {x - cam.X, y - cam.Y, width * scaleX, height * scaleY};
    SDL_RenderCopyEx(Engine::GetInstance()->GetRenderer(), m_TextureMap[id], &srcRect, &dstRect, 0, nullptr, flip);
}

void TextureManager::DrawFrame(std::string id, int x, int y, int width, int height, int row, int frame, SDL_RendererFlip flip)
{   SDL_Rect srcRect = {width*frame,  height*(row-1), width, height};
    Vector2D cam = Camera::GetInstance()->GetPosition();
    SDL_Rect dstRect = {x - cam.X, y - cam.Y, width, height};
    SDL_RenderCopyEx(Engine::GetInstance()->GetRenderer(), m_TextureMap[id], &srcRect, &dstRect, 0, nullptr, flip);
}//ve cac frame de taoj animation

void TextureManager::DrawTile(std::string tilesetID, int tileSize, int x, int y, int row, int frame, SDL_RendererFlip flip)
{   SDL_Rect srcRect = {tileSize * frame, tileSize * row, tileSize, tileSize };
    Vector2D cam = Camera::GetInstance()->GetPosition();
    SDL_Rect dstRect = {x - cam.X, y - cam.Y, tileSize, tileSize };
	SDL_RenderCopyEx(Engine::GetInstance()->GetRenderer(), m_TextureMap[tilesetID], &srcRect, &dstRect, 0, 0, flip);
}

bool TextureManager::ParseTextures(std::string source)
{
    TiXmlDocument xml;
    xml.LoadFile(source); //mo file xml
    if(xml.Error()) {
        std::cout << "Failed to load: " << source << std::endl; //check neu mo loi
        return false;
    }

    TiXmlElement* root = xml.RootElement(); //truyen tham so trong file textures.tml(<texture> =>root ) (id,path) va dung Load
    for(TiXmlElement* e=root->FirstChildElement(); e != nullptr; e = e->NextSiblingElement()) {//vong lap lay id vs path va Load
        if(e->Value() == std::string("texture")) {
            std::string id = e->Attribute("id");
            std::string src = e->Attribute("source");
            Load(id, src);
        }
    }

    std::cout << "Texture win" << std::endl;
    return true;
}

void TextureManager::Drop(std::string id)
{
    SDL_DestroyTexture(m_TextureMap[id]); //xoa texture ko can nua
    m_TextureMap.erase(id); //bo texture khoi map
}

void TextureManager::Clean()
{
    std::map<std::string, SDL_Texture*>::iterator it;
    for(it = m_TextureMap.begin(); it != m_TextureMap.end(); it++) {
        SDL_DestroyTexture(it->second);
    }
    m_TextureMap.clear(); //clear all

    SDL_Log("Texture map cleaned!");
}


