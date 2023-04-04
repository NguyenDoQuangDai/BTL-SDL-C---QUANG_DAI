#ifndef WARRIOR_H
#define WARRIOR_H

#define JUMP_TIME 10.0f
#define JUMP_FORCE 7.0f
#define RUN_FORCE 4.0f
#define ATTACK_TIME 25.0f

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
        void AnimationState();

    private:
        bool m_IsJumping;
        bool m_IsRunning;
        bool m_IsFalling;
        bool m_IsGrounded;  //nv cham dat -> moi duoc nhay
        bool m_IsAttacking;
        bool m_IsCrouching;

        float m_JumpTime;   //tg nhay len
        float m_JumpForce;  //luc nhay
        float m_AttackTime;

        Collider* m_Collider;
        Animation* m_Animation;
        RigidBody* m_RigidBody;
        Vector2D m_LastSafePosition;
};

#endif // WARRIOR_H
