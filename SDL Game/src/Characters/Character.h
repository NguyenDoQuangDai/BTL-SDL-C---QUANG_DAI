#ifndef CHARACTER_H
#define CHARACTER_H

#include "GameObject.h"

#include <iostream>
#include<string>


class Character: public GameObject //dat thua ke tu GameObject(co chg trinh con voi Propertíes)
{
    public:
        Character(Properties* props): GameObject(props) {}  //dinh nghia properties -> lay dc moi thong tin do tu GameObject
                                                            //goi chg trinh con GameObject vs truyen props
        virtual void Draw()=0;//
        virtual void Update(float dt)=0;//xem IObject
        virtual void Clean()=0;//

    protected:
        std::string m_Name; //ten cua nv(character)
};

#endif // CHARACTER_H
