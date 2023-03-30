#include "Warrior.h"
#include "TextureManager.h"
#include "Input.h"
#include "GameObject.h"
#include "Camera.h"
#include "CollisionHandler.h"

#include<SDL.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include<SDL_ttf.h>


Warrior::Warrior(Properties* props): Character(props)
{
    m_JumpTime = JUMP_TIME;
    m_JumpForce = JUMP_FORCE;

    m_Collider = new Collider();
    m_Collider->SetBuffer(-81, -61.5, -6, -6);

    m_RigidBody = new RigidBody();
    m_RigidBody->SetGravity(9.8f);

    m_Animation = new Animation();
    m_Animation->SetProps(m_TextureID, 1, 8, 200, SDL_FLIP_NONE);
}

void Warrior::Draw()
{
    m_Animation->Draw(m_Transform->X, m_Transform->Y, m_Width, m_Height);

    Vector2D cam = Camera::GetInstance()->GetPosition();
    SDL_Rect box = m_Collider->Get();
    box.x -= cam.X;
    box.y -= cam.Y;
    SDL_RenderDrawRect(Engine::GetInstance()->GetRenderer(), &box);
}

void Warrior::Update(float dt)
{
    m_Animation->SetProps("player", 1, 8, 200);
    m_RigidBody->UnSetForce();

    //di chuyen sang ngang
    if(Input::GetInstance()->GetKeyDown(SDL_SCANCODE_A) || Input::GetInstance()->GetKeyDown(SDL_SCANCODE_LEFT)) {
        m_RigidBody->ApplyForceX(5*BACKWARD);
        m_Animation->SetProps("player_run", 1, 8, 200, SDL_FLIP_HORIZONTAL); //dao nguoc frame theo chieu ngang
    }
    if(Input::GetInstance()->GetKeyDown(SDL_SCANCODE_D) || Input::GetInstance()->GetKeyDown(SDL_SCANCODE_RIGHT)) {
        m_RigidBody->ApplyForceX(5*FORWARD);
        m_Animation->SetProps("player_run", 1, 8, 200);
    }


    //nhay
    if ((Input::GetInstance()->GetKeyDown(SDL_SCANCODE_W) || Input::GetInstance()->GetKeyDown(SDL_SCANCODE_UP)
         || Input::GetInstance()->GetKeyDown(SDL_SCANCODE_SPACE)) && m_IsGrounded) {
		m_IsJumping = true;
		m_IsGrounded = false;
		m_RigidBody->ApplyForceY(UPWARD * m_JumpForce);
	}

	if ((Input::GetInstance()->GetKeyDown(SDL_SCANCODE_W) || Input::GetInstance()->GetKeyDown(SDL_SCANCODE_UP)
      || Input::GetInstance()->GetKeyDown(SDL_SCANCODE_SPACE)) && m_IsJumping && m_JumpTime > 0) {
		m_JumpTime -= dt;
		m_RigidBody->ApplyForceY(UPWARD * m_JumpForce);
	}
	else {
		m_IsJumping = false;
		m_JumpTime = JUMP_TIME;
	}


	//di chuyen tren truc x
	m_RigidBody->Update(dt);
	m_LastSafePosition.X = m_Transform->X;
	m_Transform->X += m_RigidBody->Position().X;
	m_Collider->Set(m_Transform->X, m_Transform->Y, 30, 50);

	if (CollisionHandler::GetInstance()->MapCollision(m_Collider->Get())) {
		m_Transform->X = m_LastSafePosition.X;
	}

	//di chuyen tren truc y
	m_RigidBody->Update(dt);
	m_LastSafePosition.Y = m_Transform->Y;
	m_Transform->Y += m_RigidBody->Position().Y;
	m_Collider->Set(m_Transform->X, m_Transform->Y, 30, 50);

	//goi vi tri safe neu xay ra va cham
	if (CollisionHandler::GetInstance()->MapCollision(m_Collider->Get())) {
		m_IsGrounded = true;
		m_Transform->Y = m_LastSafePosition.Y;
	}
	else {
		m_IsGrounded = false;
	}

	//ko nhay neu ko nhay tren mat dat
	if ((Input::GetInstance()->GetKeyDown(SDL_SCANCODE_A) || Input::GetInstance()->GetKeyDown(SDL_SCANCODE_LEFT)) && (m_IsJumping || !m_IsGrounded)) {
		m_Animation->SetProps("player_jump", 1, 2, 200, SDL_FLIP_HORIZONTAL);
	}
	else if (m_IsJumping || !m_IsGrounded) {
		m_Animation->SetProps("player_jump", 1, 2, 200);
	}

    m_Origin->X = m_Transform->X + m_Width/2;
    m_Origin->Y = m_Transform->Y + m_Height/2;

    m_Animation->Update();
}

void Warrior::Clean()
{
    TextureManager::GetInstance()->Drop(m_TextureID);
}

/*
#include "Warrior.h"
#include "TextureManager.h"
#include "Input.h"
#include "GameObject.h"
#include "Camera.h"
#include "CollisionHandler.h"

#include<SDL.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include<SDL_ttf.h>


Warrior::Warrior(Properties* props): Character(props)
{
    m_JumpTime = JUMP_TIME;
    m_JumpForce = JUMP_FORCE;

    m_Collider = new Collider();
    m_Collider->SetBuffer(-84, -70, -5, -5);

    m_RigidBody = new RigidBody();
    m_RigidBody->SetGravity(5.0f);

    m_Animation = new Animation();
    m_Animation->SetProps(m_TextureID, 1, 8, 200, SDL_FLIP_NONE);
}

void Warrior::Draw()
{
    m_Animation->Draw(m_Transform->X, m_Transform->Y, m_Width, m_Height);

    Vector2D cam = Camera::GetInstance()->GetPosition();
    SDL_Rect box = m_Collider->Get();
    box.x -= cam.X;
    box.y -= cam.Y;
    SDL_RenderDrawRect(Engine::GetInstance()->GetRenderer(), &box);
}

void Warrior::Update(float dt)
{
    m_Animation->SetProps("player", 1, 8, 200);
    m_RigidBody->UnSetForce();

    //di chuyen sang ngang
    if(Input::GetInstance()->GetKeyDown(SDL_SCANCODE_A) || Input::GetInstance()->GetKeyDown(SDL_SCANCODE_LEFT)) {
        m_RigidBody->ApplyForceX(5*BACKWARD);
        m_Animation->SetProps("player_run", 1, 8, 200, SDL_FLIP_HORIZONTAL); //dao nguoc frame theo chieu ngang
    }
    if(Input::GetInstance()->GetKeyDown(SDL_SCANCODE_D) || Input::GetInstance()->GetKeyDown(SDL_SCANCODE_RIGHT)) {
        m_RigidBody->ApplyForceX(5*FORWARD);
        m_Animation->SetProps("player_run", 1, 8, 200);
    }


    //nhay
    if ((Input::GetInstance()->GetKeyDown(SDL_SCANCODE_W) || Input::GetInstance()->GetKeyDown(SDL_SCANCODE_UP)
         || Input::GetInstance()->GetKeyDown(SDL_SCANCODE_SPACE)) && m_IsGrounded) {
		m_IsJumping = true;
		m_IsGrounded = false;
		m_RigidBody->ApplyForceY(UPWARD * m_JumpForce);
	}

	if ((Input::GetInstance()->GetKeyDown(SDL_SCANCODE_W) || Input::GetInstance()->GetKeyDown(SDL_SCANCODE_UP)
      || Input::GetInstance()->GetKeyDown(SDL_SCANCODE_SPACE)) && m_IsJumping && m_JumpTime > 0) {
		m_JumpTime -= dt;
		m_RigidBody->ApplyForceY(UPWARD * m_JumpForce);
	}
	else {
		m_IsJumping = false;
		m_JumpTime = JUMP_TIME;
	}


	//di chuyen tren truc x
	m_RigidBody->Update(dt);
	m_LastSafePosition.X = m_Transform->X;
	m_Transform->X += m_RigidBody->Position().X;
	m_Collider->Set(m_Transform->X, m_Transform->Y, 30, 50);

	if (CollisionHandler::GetInstance()->MapCollision(m_Collider->Get())) {
		m_Transform->X = m_LastSafePosition.X;
	}

	//di chuyen tren truc y
	m_RigidBody->Update(dt);
	m_LastSafePosition.Y = m_Transform->Y;
	m_Transform->Y += m_RigidBody->Position().Y;
	m_Collider->Set(m_Transform->X, m_Transform->Y, 30, 50);

	//goi vi tri safe neu xay ra va cham
	if (CollisionHandler::GetInstance()->MapCollision(m_Collider->Get())) {
		m_IsGrounded = true;
		m_Transform->Y = m_LastSafePosition.Y;
	}
	else {
		m_IsGrounded = false;
	}

	//ko nhay neu ko nhay tren mat dat
	if (m_IsJumping || !m_IsGrounded) {
		m_Animation->SetProps("player_jump", 1, 2, 200);
	}

    m_Origin->X = m_Transform->X + m_Width/2;
    m_Origin->Y = m_Transform->Y + m_Height/2;

    m_Animation->Update();
}

void Warrior::Clean()
{
    TextureManager::GetInstance()->Drop(m_TextureID);
}
*/
