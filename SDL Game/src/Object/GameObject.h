#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "IObject.h"
#include "Transform.h"
#include "Point.h"

#include <iostream>
#include<string>

#include<SDL.h>
#include<SDL_image.h>


struct Properties {
    public:
        Properties(std::string textureID, int x, int y, int width, int height, SDL_RendererFlip flip = SDL_FLIP_NONE) {
            X = x;
            Y = y;
            Flip = flip;
            Width = width;
            Height = height;
            TextureID = textureID;
        }//chg trinh con dinh nghia properties(-> dinh nghia cach objects dc khoi tao)

    public:
        std::string TextureID;
        int Width, Height;
        float X, Y; //khai bao gia tri ma Transform se co
        SDL_RendererFlip Flip;
};

class GameObject : public IObject //thua ke tu IObject
{
    public:
        GameObject(Properties* props): m_TextureID(props->TextureID), m_Width(props->Width), m_Height(props->Height), m_Flip(props->Flip) {
            m_Transform = new Transform(props->X, props->Y);//dinh nghia tao new instance cho transform vs X&Y
                                                            //(vi no la pointer nen ko lam nhu bon kia dc)
            float px = props->X + props->Width/2;
            float py = props->Y + props->Height/2;
            //lay trung tam cua object (px, py)
            m_Origin = new Point(px, py); //khoi tao member Origin voi vi tri trung tam object
        }

        inline Point* GetOrigin() {return m_Origin;} //lay goc Origin cho camera position
        //thuc thi cac ham cua IObject khi thua ke tu no
        virtual void Draw()=0;//
        virtual void Update(float dt)=0;// ve/don object
        virtual void Clean()=0;//

    protected:
        Point* m_Origin;
        Transform* m_Transform; //khoi tao member transform khi include
        int m_Width, m_Height; //khoi tao chieu dai, rong cho Object
        std::string m_TextureID; //textureID nay tham chieu den cai texture cua object thuc su co
        SDL_RendererFlip m_Flip; //dinh huong xoay ngang hay doc object
};

#endif // GAMEOBJECT_H
