#include "TextureManager.h"
#include "Engine.h"
#include "Camera.h"
#include "tinyxml.h"

#include <iostream>
#include<string>

#include<SDL.h>
#include<SDL_image.h>

TextureManager* TextureManager::s_Instance = nullptr; //khai bao object tinh(static)

bool TextureManager::Load(std::string id, std::string filename)
{
    SDL_Surface* surface = IMG_Load(filename.c_str()); //load anh thanh surface,
    //SDL_Surface: cau truc chua du lieu pixel tho, kthc, dinh dang pixel
    //IMG_Load: ham sdl image load image va bien no thanh surface (char)

    if(surface == nullptr) {
        SDL_Log("Failed to load texture: %s, %s", filename.c_str(), SDL_GetError());//ktra load surface debug
        return false;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(Engine::GetInstance()->GetRenderer(), surface); //tao texture tu surface,
    //SDL_Texture: cau truc bieu dien du lieu pixel
    //SDL_CreateTextureFromSurface: y nhu ten ham
    if(texture == nullptr) {
        SDL_Log("Failed to create texture from surface: %s", SDL_GetError());//check xem texture co dc tao
        return false;
    }

    m_TextureMap[id] = texture; //them texture vao texturemap

    return true;
}
// SDL_Rect: cau truc dinh nghia 1 HCN tren screen.
// SDL_RenderCopyEx: copy 1 phan texture vao render hien tai(co the xoay, lat).

//srcRect: khoang HCN cua anh ma minh muon ve
//dstRect: khoang HCN tren screen ma minh muon anh hien len

void TextureManager::Draw(std::string id, int x, int y, int width, int height, float scaleX, float scaleY, float scrollRatio, SDL_RendererFlip flip)
{   SDL_Rect srcRect = {0,  0, width, height};
    Vector2D cam = Camera::GetInstance()->GetPosition()*scrollRatio; //tuy thuoc scrollRatio ma background chuyen dong theo nv nhanh/cham
    SDL_Rect dstRect = {x - cam.X, y - cam.Y, width * scaleX, height * scaleY};
    SDL_RenderCopyEx(Engine::GetInstance()->GetRenderer(), m_TextureMap[id], &srcRect, &dstRect, 0, nullptr, flip);
    // -> ve background,... nullptr co the thay bang goc xoay(double) nhung chua dungf den =)
}

void TextureManager::DrawFrame(std::string id, int x, int y, int width, int height, int row, int frame, SDL_RendererFlip flip)
{   SDL_Rect srcRect = {width*frame,  height*(row-1), width, height};//lay cac frame trong sprite sheet lan luot //lay row-1 vi dat row dau = 1
    Vector2D cam = Camera::GetInstance()->GetPosition(); //lay chinh xac gia tri vi tri camera
    SDL_Rect dstRect = {x - cam.X, y - cam.Y, width, height};
    SDL_RenderCopyEx(Engine::GetInstance()->GetRenderer(), m_TextureMap[id], &srcRect, &dstRect, 0, nullptr, flip);
    // -> ve cac frame de taoj animation
}

void TextureManager::DrawTile(std::string tilesetID, int tileSize, int x, int y, int row, int frame, SDL_RendererFlip flip)
{   SDL_Rect srcRect = {tileSize * frame, tileSize * row, tileSize, tileSize };
    Vector2D cam = Camera::GetInstance()->GetPosition(); //lay chinh xac gia tri vi tri camera
    SDL_Rect dstRect = {x - cam.X, y - cam.Y, tileSize, tileSize};
	SDL_RenderCopyEx(Engine::GetInstance()->GetRenderer(), m_TextureMap[tilesetID], &srcRect, &dstRect, 0, 0, flip);
}

bool TextureManager::ParseTextures(std::string source)
{
    TiXmlDocument xml;
    xml.LoadFile(source); //load mo file xml
    if(xml.Error()) {
        std::cout << "Failed to load: " << source << std::endl; //check neu mo loi
        return false;
    }

    //VD: <textures>
    //      <texture id="player_run" source="assets/Run.png"/>

    TiXmlElement* root = xml.RootElement();//lay root Element hay "textures"

    for(TiXmlElement* e=root->FirstChildElement(); e != nullptr; e = e->NextSiblingElement()) {
                                                    //vong lap lay cac element: "id" vs "path" va Load
        if(e->Value() == std::string("texture")) {   //cua moi child Element "texture"

            std::string id = e->Attribute("id"); //lay thuoc tinh "id"
            std::string src = e->Attribute("source"); //...

            Load(id, src); //load image voi ten id va duong dan path
        }
    }
    return true;
}

void TextureManager::Drop(std::string id)
{
    SDL_DestroyTexture(m_TextureMap[id]); //xoa texture ko can nua khoi bo nho
    m_TextureMap.erase(id); //bo con tro texture khoi map
}

void TextureManager::Clean() //clean moi thu tren map khoi bo nho
{
    std::map<std::string, SDL_Texture*>::iterator it;
    //iterator la doi tuong co the tro den dia chi bo nho cua mang map ko quan tam trat tu
    for(it = m_TextureMap.begin(); it != m_TextureMap.end(); it++) {
        SDL_DestroyTexture(it->second); //loop de xoa moi texture
    }
    m_TextureMap.clear(); //clear:xoa moi gia tri value va bien size ve 0

    SDL_Log("Texture map cleaned!");
}


