
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
    m_RigidBody->SetGravity(9.0f); //tuy chinh gia toc trong trg

    m_SpriteAnimation = new SpriteAnimation();
    m_SpriteAnimation->SetProps(m_TextureID, 1, 8, 200);
}

void Warrior::Draw()
{
    m_SpriteAnimation->Draw(m_Transform->X, m_Transform->Y, m_Width, m_Height, 1, 1, m_Flip);

    Vector2D cam = Camera::GetInstance()->GetPosition();
    SDL_Rect box = m_Collider->Get();
    box.x -= cam.X;
    box.y -= cam.Y;
    //dieu chinh vi tri cua box theo camera do nhan vat ko vuot qua nua man hinh tru khi het map
    SDL_RenderDrawRect(Engine::GetInstance()->GetRenderer(), &box); //draw box xong chinh lai sao cho vua nhan vat
}

void Warrior::Update(float dt)
{
    m_IsRunning = false;
    m_IsCrouching = false; //gan false tu dau cho dang ngoi va dang chay
    m_RigidBody->UnSetForce(); //dam bao ko con luc tac dung khi nv ket thuc hanh dong,  neu bo di nv se chuyen dong mai mai


    //di chuyen sang ngang
    if(Input::GetInstance()->GetAxisKey(HORIZONTAL) == FORWARD) { //neu horizontal trong input return 1 = FORWARD(RigidBody.h) ->Forward
        m_RigidBody->ApplyForceX(FORWARD * RUN_FORCE);
        m_Flip = SDL_FLIP_NONE;
        m_IsRunning = true;
    }
    if(Input::GetInstance()->GetAxisKey(HORIZONTAL) == BACKWARD) { //neu horizontal trong input return -1  = BACKWARD ->Backward
        m_RigidBody->ApplyForceX(BACKWARD * RUN_FORCE);
        m_Flip = SDL_FLIP_HORIZONTAL;
        m_IsRunning = true;
    }


    //ngoi
    if(Input::GetInstance()->GetKeyDown(SDL_SCANCODE_S) || Input::GetInstance()->GetKeyDown(SDL_SCANCODE_DOWN)) {
        m_RigidBody->UnSetForce();
        m_IsCrouching = true;
    }


    // Event nhay
    if ((Input::GetInstance()->GetKeyDown(SDL_SCANCODE_W) || Input::GetInstance()->GetKeyDown(SDL_SCANCODE_UP)
         || Input::GetInstance()->GetKeyDown(SDL_SCANCODE_SPACE)) && m_IsGrounded) { //chi duoc nhay len neu nv tren mat dat
		m_IsJumping = true;
		m_IsGrounded = false;
		m_RigidBody->ApplyForceY(UPWARD * m_JumpForce);
	}
    // thuc hien event nhay
	if ((Input::GetInstance()->GetKeyDown(SDL_SCANCODE_W) || Input::GetInstance()->GetKeyDown(SDL_SCANCODE_UP)
      || Input::GetInstance()->GetKeyDown(SDL_SCANCODE_SPACE)) && m_IsJumping && m_JumpTime > 0){//tac dung luc huong len theo tg nhay>0
		m_JumpTime -= dt;
		m_RigidBody->ApplyForceY(UPWARD * m_JumpForce);
		//neu nhan nhay -> nhan vat nhay den khi m_JumpTime == 0
		//neu nha nut nhay ra / khi m_JumpTime == 0 -> thuc hien else
	}
	else {
		m_IsJumping = false;
		m_JumpTime = JUMP_TIME; //dat lai ve m_JumpTime 14.0f
	}


    //Roi
    if(m_RigidBody->Velocity().Y > 0 && !m_IsGrounded) { //neu v>0 va nv ko o tren mat dat -> dang roi true
        m_IsFalling = true;
    }
    else {m_IsFalling = false;}


    // Event tancong
    if(Input::GetInstance()->GetKeyDown(SDL_SCANCODE_K)) {
        //m_RigidBody->UnSetForce(); //->Neu bo di thi vua chay vua tan cong duoc
        m_IsAttacking = true;
    }
    // Tg event tancong (tuong tu thuc hien event nhay)
    if(m_IsAttacking && m_AttackTime > 0) {
        m_AttackTime -= dt;
    }
    else{
        m_IsAttacking = false;
        m_AttackTime = ATTACK_TIME;
    }

	//di chuyen tren truc x
	m_RigidBody->Update(dt); //update RigidBody theo dt
	m_LastSafePosition.X = m_Transform->X; //luu vi tri hien tai cua transform nv tren truc X truoc khi co va cham
	m_Transform->X += m_RigidBody->Position().X; //tinh vi tri tren truc X su dung RigidBody va update m_Transform
	m_Collider->Set(m_Transform->X, m_Transform->Y, 20, 42); //tinh toan update box collider voi vi tri cua player

	//goi vi tri safe neu xay ra va cham
	if (CollisionHandler::GetInstance()->MapCollision(m_Collider->Get())) {
		m_Transform->X = m_LastSafePosition.X;
		//neu player va cham voi mat dat theo chieu ngang  -> goi lai vi tri safe trc do
	}

	//di chuyen tren truc y
	m_RigidBody->Update(dt); //update RigidBody theo dt
	m_LastSafePosition.Y = m_Transform->Y; //luu vi tri hien tai cua transform nv tren truc Y trc khi co va cham
	m_Transform->Y += m_RigidBody->Position().Y; //tinh vi tri tren truc Y su dung RigidBody va update m_Transform
	m_Collider->Set(m_Transform->X, m_Transform->Y, 20, 42); //tinh toan update box collider voi vi tri player

	//goi vi tri safe neu xay ra va cham
	if (CollisionHandler::GetInstance()->MapCollision(m_Collider->Get())) {
		m_IsGrounded = true;
		m_Transform->Y = m_LastSafePosition.Y;
		//neu player va cham voi mat dat theo chieu doc  -> goi lai vi tri safe va dat nhan vat ve trang thai dang cham dat
	}
	else {
		m_IsGrounded = false;
	}

    m_Origin->X = m_Transform->X + m_Width/2;
    m_Origin->Y = m_Transform->Y + m_Height/2;
    //update gia tri vi tri cua origin vao chinh giua player cho camera di chuyen theo

    AnimationState();
    m_SpriteAnimation->Update(dt); //update animation theo dt
}

void Warrior::AnimationState()
{
    m_SpriteAnimation->SetProps("player_idle", 1, 8, 200); //khoi tao set properties thanh idle

    //cai Is... nao true thi set properties thanh cai day
    if(m_IsRunning) {m_SpriteAnimation->SetProps("player_run", 1, 8, 200);}
    if(m_IsCrouching) {m_SpriteAnimation->SetProps("player_crouch", 1, 2, 100);}
    if(m_IsJumping) {m_SpriteAnimation->SetProps("player_jump", 1, 2, 300);}
    if(m_IsFalling) {m_SpriteAnimation->SetProps("player_fall", 1, 2, 300);}
    if(m_IsAttacking) {
            m_SpriteAnimation->SetProps("player_attack", 1, 6, 70);
            if(Input::GetInstance()->GetAxisKey(HORIZONTAL) == FORWARD)
                m_Collider->SetBuffer(-105, -68, 0, -6);

            else if(Input::GetInstance()->GetAxisKey(HORIZONTAL) == BACKWARD)
                m_Collider->SetBuffer(-85, -68, 0, -6);

    }
    else {
        m_Collider->SetBuffer(-90, -68, 0, -6);
    }
}


void Warrior::Clean()
{
    TextureManager::GetInstance()->Drop(m_TextureID);
}
