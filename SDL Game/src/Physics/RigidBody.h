#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "Vector2D.h"

#include <iostream>
#include<string>

#include<SDL.h>
#include<SDL_image.h>

#define UNI_MASS 1.0f
#define GRAVITY 9.8f

#define FORWARD 1
#define BACKWARD -1
#define UPWARD -2.8
#define DOWNWARD 1

class RigidBody
{
    public:
        RigidBody() {
            m_Mass = UNI_MASS;
            m_Gravity = GRAVITY; //khoi tao bien thanh vien khoi lg va gia toc trong trg
        }

        inline void SetMass(float mass) {m_Mass = mass;}
        inline void SetGravity(float gravity) {m_Gravity = gravity;}
        //ham lay g vs m

        inline void ApplyForce(Vector2D F) {m_Force = F;}
        inline void ApplyForceX(float Fx) {m_Force.X = Fx;}
        inline void ApplyForceY(float Fy) {m_Force.Y = Fy;}
        inline void UnSetForce() {m_Force = Vector2D(0,0);}
        //Luc F tac dung len player ptich theo Fx + Fy va luc vector 0

        inline void ApplyFriction(Vector2D Fr) {m_Friction = Fr;}
        inline void UnSetFriction() {m_Friction = Vector2D(0,0);}
        //Luc ma sat va luc vector 0

        inline float GetMass() {return m_Mass;}
        inline Vector2D Position() {return m_Position;}
        inline Vector2D Velocity() {return m_Velocity;}
        inline Vector2D Accelaration() {return m_Accelaration;}
        //nhu tren

        void Update(float dt) { //cap nhat gia toc, van toc, quang duong theo tg
            m_Accelaration.X = (m_Force.X + m_Friction.X)/m_Mass; // a=F/m
            m_Accelaration.Y = m_Gravity + m_Force.Y/m_Mass; //a= g + F/m
            m_Velocity = m_Accelaration * dt; //v=a*t
            m_Position = m_Velocity * dt; //s=v*t
        }

    private:
        float m_Mass; //khoi lg
        float m_Gravity; //gia toc trong trg

        Vector2D m_Force; //luc
        Vector2D m_Friction; //luc ma sat

        Vector2D m_Position; //quang dg vi tri
        Vector2D m_Velocity; //van toc
        Vector2D m_Accelaration; //gia toc
};

#endif // RIGIDBODY_H
