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
    m_IsRunning = false;
    m_IsJumping = false;
    m_IsFalling = false;
    m_IsCrouching = false;
    m_IsGrounded = false;
    m_IsAttacking = false;

    m_Flip = SDL_FLIP_NONE;
    m_JumpTime = JUMP_TIME;
    m_JumpForce = JUMP_FORCE;
    m_AttackTime = ATTACK_TIME;

    m_Collider = new Collider();
    m_Collider->SetBuffer(-90, -68, 0, -6);

    m_RigidBody = new RigidBody();
    m_RigidBody->SetGravity(9.8f);

    m_Animation = new Animation();
    m_Animation->SetProps(m_TextureID, 1, 8, 200);
}

void Warrior::Draw()
{
    m_Animation->Draw(m_Transform->X, m_Transform->Y, m_Width, m_Height, m_Flip);

    Vector2D cam = Camera::GetInstance()->GetPosition();
    SDL_Rect box = m_Collider->Get();
    box.x -= cam.X;
    box.y -= cam.Y;
    SDL_RenderDrawRect(Engine::GetInstance()->GetRenderer(), &box);
}

void Warrior::Update(float dt)
{
    m_IsRunning = false;
    m_IsCrouching = false;
    m_RigidBody->UnSetForce();

    //di chuyen sang ngang
    if(Input::GetInstance()->GetAxisKey(HORIZONTAL) == FORWARD /* && !m_IsAttacking */) { //neu horizontal trong input return 1 ->Forward
        m_RigidBody->ApplyForceX(FORWARD * RUN_FORCE);
        m_Flip = SDL_FLIP_NONE;
        m_IsRunning = true;
    }
    if(Input::GetInstance()->GetAxisKey(HORIZONTAL) == BACKWARD /* && !m_IsAttacking */) {
        m_RigidBody->ApplyForceX(BACKWARD * RUN_FORCE);
        m_Flip = SDL_FLIP_HORIZONTAL;
        m_IsRunning = true;
    }
    //ngoi
    if(Input::GetInstance()->GetKeyDown(SDL_SCANCODE_S) || Input::GetInstance()->GetKeyDown(SDL_SCANCODE_DOWN)) {
        m_RigidBody->UnSetForce();
        m_IsCrouching = true;
    }
    // tancong
    if(Input::GetInstance()->GetKeyDown(SDL_SCANCODE_K)) {
        //m_RigidBody->UnSetForce();
        m_IsAttacking = true;
    }

    //nhay
    if ((Input::GetInstance()->GetKeyDown(SDL_SCANCODE_W) || Input::GetInstance()->GetKeyDown(SDL_SCANCODE_UP)
         || Input::GetInstance()->GetKeyDown(SDL_SCANCODE_SPACE)) && m_IsGrounded) { //chi tac dung luc nhay len neu nv tren mat dat
		m_IsJumping = true;
		m_IsGrounded = false;
		m_RigidBody->ApplyForceY(UPWARD * m_JumpForce);
	}

	if ((Input::GetInstance()->GetKeyDown(SDL_SCANCODE_W) || Input::GetInstance()->GetKeyDown(SDL_SCANCODE_UP)
      || Input::GetInstance()->GetKeyDown(SDL_SCANCODE_SPACE)) && m_IsJumping && m_JumpTime > 0) { //tac dung luc huong len neu tg nhay>0
		m_JumpTime -= dt;
		m_RigidBody->ApplyForceY(UPWARD * m_JumpForce);
	}
	else {
		m_IsJumping = false;
		m_JumpTime = JUMP_TIME;
	}
    //Roi
    if(m_RigidBody->Velocity().Y > 0 && !m_IsGrounded) {
        m_IsFalling = true;
    }
    else {m_IsFalling = false;}

    // Tg tancong
    if(m_IsAttacking && m_AttackTime > 0) {
        m_AttackTime -= dt;
    }
    else{
        m_IsAttacking = false;
        m_AttackTime = ATTACK_TIME;
    }

	//di chuyen tren truc x
	m_RigidBody->Update(dt);
	m_LastSafePosition.X = m_Transform->X;
	m_Transform->X += m_RigidBody->Position().X;
	m_Collider->Set(m_Transform->X, m_Transform->Y, 20, 40);

	if (CollisionHandler::GetInstance()->MapCollision(m_Collider->Get())) {
		m_Transform->X = m_LastSafePosition.X;
	}

	//di chuyen tren truc y
	m_RigidBody->Update(dt);
	m_LastSafePosition.Y = m_Transform->Y;
	m_Transform->Y += m_RigidBody->Position().Y;
	m_Collider->Set(m_Transform->X, m_Transform->Y, 20, 40);

	//goi vi tri safe neu xay ra va cham
	if (CollisionHandler::GetInstance()->MapCollision(m_Collider->Get())) {
		m_IsGrounded = true;
		m_Transform->Y = m_LastSafePosition.Y;
	}
	else {
		m_IsGrounded = false;
	}

    m_Origin->X = m_Transform->X + m_Width/2;
    m_Origin->Y = m_Transform->Y + m_Height/2;

    AnimationState();
    m_Animation->Update();
}

void Warrior::AnimationState()
{
    m_Animation->SetProps("player_idle", 1, 8, 200);

    if(m_IsRunning) {m_Animation->SetProps("player_run", 1, 8, 200);}
    if(m_IsCrouching) {m_Animation->SetProps("player_crouch", 1, 2, 200);}
    if(m_IsJumping) {m_Animation->SetProps("player_jump", 1, 2, 200);}
    if(m_IsFalling) {m_Animation->SetProps("player_fall", 1, 2, 200);}
    if(m_IsAttacking) {m_Animation->SetProps("player_attack", 1, 10, 200);}
}


void Warrior::Clean()
{
    TextureManager::GetInstance()->Drop(m_TextureID);
}
