#ifndef WARRIOR_H
#define WARRIOR_H

#define JUMP_TIME 10.0f
#define JUMP_FORCE 8.0f

#include "Character.h"
#include "Animation.h"
#include "RigidBody.h"
#include "Vector2D.h"
#include "Collider.h"


class Warrior: public Character
{
    public:
        Warrior(Properties* props);

        virtual void Draw();//
        virtual void Update(float dt);//
        virtual void Clean();//

    private:
        bool m_IsJumping;   //nv nhay
        bool m_IsGrounded;  //nv cham dat -> moi duoc nhay

        float m_JumpTime;   //tg nhay len
        float m_JumpForce;  //luc nhay

        Collider* m_Collider;
        Animation* m_Animation;
        RigidBody* m_RigidBody;
        Vector2D m_LastSafePosition;
};

#endif // WARRIOR_H
