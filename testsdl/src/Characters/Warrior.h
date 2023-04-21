#ifndef WARRIOR_H
#define WARRIOR_H

#define JUMP_TIME 14.0f
#define JUMP_FORCE 6.2f
#define RUN_FORCE 6.0f
#define ATTACK_TIME 25.0f

#include "Character.h"
#include "Animation.h"
#include "RigidBody.h"
#include "Vector2D.h"
#include "Collider.h"
#include "SpriteAnimation.h"

#include<iostream>


class Warrior: public Character
{
    public:
        Warrior();
        Warrior(Properties* props); //dinh nghia ctr con(define constructor)

        virtual void Draw();//
        virtual void Update(float dt);//thua ke IObject
        virtual void Clean();//

        int life = 3;

    private:
        void AnimationState();

    private:
        //Is... = true -> set animation thanh ... animation
        bool m_IsJumping;
        bool m_IsRunning;
        bool m_IsFalling;
        bool m_IsGrounded;  //nv dang cham dat -> moi duoc nhay
        bool m_IsAttacking;
        bool m_IsCrouching;
        bool m_IsDying;
        bool m_CheckA = false;
        bool m_CheckB = true;

        int m_GamePoint = 0;

        float m_JumpTime;   //tg nhan vat co the nhay len
        float m_JumpForce;  //luc nhay len nv
        float m_AttackTime; //tg tan cong

        Collider* m_Collider;
        SpriteAnimation* m_SpriteAnimation;
        RigidBody* m_RigidBody;
        Vector2D m_LastSafePosition;
};

#endif // WARRIOR_H
