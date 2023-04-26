
#include "Warrior.h"
#include "TextureManager.h"
#include "Input.h"
#include "GameObject.h"
#include "Camera.h"
#include "CollisionHandler.h"

#include<SDL.h>
#include<SDL_image.h>
#include<SDL_mixer.h>

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
    //SDL_RenderDrawRect(Engine::GetInstance()->GetRenderer(), &box); //draw box xong chinh lai sao cho vua nhan vat
}

void Warrior::Update(float dt)
{
    if(!m_Pause) {
    m_IsRunning = false;
    m_IsCrouching = false; //gan false tu dau cho dang ngoi va dang chay
    m_RigidBody->UnSetForce(); //dam bao ko con luc tac dung khi nv ket thuc hanh dong,  neu bo di nv se chuyen dong mai mai


    //di chuyen sang ngang
    if(Input::GetInstance()->GetAxisKey(HORIZONTAL) == FORWARD  && m_Origin->Y < 700) {
            //neu horizontal trong input return 1 = FORWARD(RigidBody.h) ->Forward
        m_RigidBody->ApplyForceX(FORWARD * RUN_FORCE);
        m_Flip = SDL_FLIP_NONE;
        m_IsRunning = true;
    }
    if(Input::GetInstance()->GetAxisKey(HORIZONTAL) == BACKWARD && m_Origin->Y < 700) {
            //neu horizontal trong input return -1  = BACKWARD ->Backward
        m_RigidBody->ApplyForceX(BACKWARD * RUN_FORCE);
        m_Flip = SDL_FLIP_HORIZONTAL;
        m_IsRunning = true;
    }



    //ngoi
    if(Input::GetInstance()->GetKeyDown(SDL_SCANCODE_S)) {
        m_RigidBody->UnSetForce();
        m_IsCrouching = true;
    }



    // Event nhay
    if ((Input::GetInstance()->GetKeyDown(SDL_SCANCODE_W)
         || Input::GetInstance()->GetKeyDown(SDL_SCANCODE_SPACE)) && m_IsGrounded) { //chi duoc nhay len neu nv tren mat dat
		m_IsJumping = true;
		m_IsGrounded = false;
		m_RigidBody->ApplyForceY(UPWARD * m_JumpForce);
		Engine::GetInstance()->PlaySound("Jump");
	}
    // thuc hien event nhay
	if ((Input::GetInstance()->GetKeyDown(SDL_SCANCODE_W) || Input::GetInstance()->GetKeyDown(SDL_SCANCODE_SPACE))
      && m_IsJumping && m_JumpTime > 0){//tac dung luc huong len theo tg nhay>0
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
        if(!m_GameOver) {
            Engine::GetInstance()->PlaySound("Slash");
        }
        if(m_CheckB == true && m_Origin->X >= 2820 && m_Origin->X <= 2940 && m_Origin->Y >= 300) {
            m_CheckB = false;
            m_CheckA = true;
            m_GamePoint++;
            Engine::GetInstance()->Check();
            Engine::GetInstance()->StatusUp();
            std::cout << "B checked! Gamepoint la: " << m_GamePoint << std::endl;
        }
        else if(m_CheckA == true && m_Origin->Y >= 300 && ((m_Origin->X >= 330 && m_Origin->X <= 443 && m_Flip == SDL_FLIP_NONE)
                                                            || (m_Origin->X >= 455 && m_Origin->X <= 535 && m_Flip == SDL_FLIP_HORIZONTAL))) {
            m_CheckA = false;
            m_CheckB = true;
            m_GamePoint++;
            Engine::GetInstance()->Check();
            Engine::GetInstance()->StatusUp();
            std::cout << "A checked! Gamepoint la: " << m_GamePoint << std::endl;
        }
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
		m_Transform->Y = m_LastSafePosition.Y;
		if(!m_IsJumping) {
		    if(!m_IsGrounded)
                Engine::GetInstance()->PlaySound("Fall");
            m_IsGrounded = true;
        }
		//neu player va cham voi mat dat theo chieu doc  -> goi lai vi tri safe va dat nhan vat ve trang thai dang cham dat
	}
	else {
		m_IsGrounded = false;
	}

    m_Origin->X = m_Transform->X + m_Width/2;
    m_Origin->Y = m_Transform->Y + m_Height/2;
    //update gia tri vi tri cua origin vao chinh giua player cho camera di chuyen theo

    if(m_Origin->Y >= 750 && m_Origin->Y <= 780) {m_IsDying = true;}
    else{m_IsDying = false;}

    if(m_IsDying) {
            Engine::GetInstance()->PlaySound("Die");
    }

    if(m_Origin->Y >= 1100) {
            if(life >= -1) {life--;}
            m_IsDying = true;

            Engine::GetInstance()->StatusDown();

            if(life == 0) {Engine::GetInstance()->PlaySound("GameOver");}
            if(life <= 0) {
                m_GameOver = true;
//                Engine::GetInstance()->GameOver();
                m_CheckA = false;
                m_CheckB = true;
                m_RigidBody->UnSetForce();

                if(Input::GetInstance()->GetKeyDown(SDL_SCANCODE_K)) {
                    m_GameOver = false;
                    m_Flip = SDL_FLIP_NONE;
                    life = 3;
                    m_Transform->X = 280;
                    m_Transform->Y = 150;
                    Engine::GetInstance()->PlaySound("Choose");

                    if(m_CheckA) {
                        Engine::GetInstance()->Check();
                    }
                }
            }
            else {
                SDL_Delay(1500);
                if(m_CheckA == false) {
                    m_Transform->X = 280;
                    m_Transform->Y = 150;
                    m_Flip = SDL_FLIP_NONE;
                }
                else {
                    m_Transform->X = 2750;
                    m_Transform->Y = 150;
                    m_Flip = SDL_FLIP_HORIZONTAL;
            }
        }
    }

    AnimationState();
    m_SpriteAnimation->Update(dt); //update animation theo dt
//    std::cout << "X.Origin: " << m_Origin->X << "  Y.Origin: " << m_Origin->Y << std::endl;
    }
    if(Input::GetInstance()->GetKeyDown(SDL_SCANCODE_ESCAPE)) {
        m_Pause = !m_Pause;
        Engine::GetInstance()->GamePause();
    }
    if(Input::GetInstance()->GetKeyDown(SDL_SCANCODE_RETURN)) {
        m_Pause = false;
    }
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
            if(m_Flip == SDL_FLIP_NONE)
                m_Collider->SetBuffer(-105, -68, 0, -6); //dich chuyen vi tri cua box theo nhan vat khi attack

            else {
                m_Collider->SetBuffer(-75, -68, 0, -6);
            }
    }
    else {
        m_Collider->SetBuffer(-90, -68, 0, -6);
    }
}


void Warrior::Clean()
{
    TextureManager::GetInstance()->Drop(m_TextureID);

    std::cout << "Warrior cleaned!" << std::endl;
}
