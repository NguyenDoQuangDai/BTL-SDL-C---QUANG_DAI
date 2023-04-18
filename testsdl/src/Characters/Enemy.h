#ifndef ENEMY_H
#define ENEMY_H

#include "Character.h"
#include "RigidBody.h"
#include "Collider.h"
#include "SeqAnimation.h"
#include "SpriteAnimation.h"
#include "Vector2D.h"

//tuong tu nhu class Warrior
class Enemy: public Character {

    public:
        Enemy(Properties* props); //dinh nghia ctr con

        virtual void Draw();
        virtual void Clean();//thua ke tu object
        virtual void Update(float dt);

    private:
        Collider* m_Collider;
        RigidBody* m_RigidBody;
        SeqAnimation* m_Animation;
        Vector2D m_LastSafePosition;
};

#endif // ENEMY_H
