#include "Enemy.h"
#include "CollisionHandler.h"

#include<iostream>

//tuong tu nhu Warrior.cpp


Enemy::Enemy(Properties* props): Character(props) {
    m_RigidBody = new RigidBody();
    m_RigidBody->SetGravity(9.8); //lay trong luc nhu player - warrior.cpp
    m_Collider = new Collider();

    m_Animation = new SeqAnimation(false); //khoi tao animation voi khong repeat vi cbi SetCurentSeq boss1 xuat hien
    m_Animation->Parse("assets/animation.aml"); //phan tich animation file(SeqAnimation)
    m_Animation->SetCurrentSeq("boss1_appear");
    //dat sequnece hien tai thanh boss1 xuat hien -> ctr chay thi boss1 appear va ko repeat
}

void Enemy::Draw()
{
    m_Animation->DrawFrame(m_Transform->X, m_Transform->Y, 1.0f, 1.0f, m_Flip); //draw sequence frame hien tai cua boss1
    m_Collider->Draw();
}

void Enemy::Update(float dt) //tuong tu (neu ko noi la Y NHU) ham Update trong Warrior.cpp
{
    //chuyen dong theo chieu ngang truc X
    m_RigidBody->Update(dt);
	m_LastSafePosition.X = m_Transform->X;
	m_Transform->X += m_RigidBody->Position().X;
	m_Collider->Set(m_Transform->X, m_Transform->Y, 20, 55);

	//giu vi tri safe neu xay ra va cham
	if (CollisionHandler::GetInstance()->MapCollision(m_Collider->Get())) {
		m_Transform->X = m_LastSafePosition.X;
	}

	//chuyen dong theo chieu doc truc y
	m_RigidBody->Update(dt);
	m_LastSafePosition.Y = m_Transform->Y;
	m_Transform->Y += m_RigidBody->Position().Y;
	m_Collider->Set(m_Transform->X, m_Transform->Y, 20, 55);

	//giu vi tri safe neu xay ra va cham
	if (CollisionHandler::GetInstance()->MapCollision(m_Collider->Get())) {
		m_Transform->Y = m_LastSafePosition.Y;
	}

	m_Animation->Update(dt);

	if(m_Animation->IsEnded()) {
        //neu animation ket thuc thi SetCurrentSeq boss1_idle va repeat
        m_Animation->SetCurrentSeq("boss1_attack");
        m_Animation->SetRepeat(true);
	}
}

void Enemy::Clean()
{

}
