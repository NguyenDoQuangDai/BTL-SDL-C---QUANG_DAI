#include "Warrior.h"
#include "TextureManager.h"
#include "Input.h"
#include "GameObject.h"

#include<SDL.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include<SDL_ttf.h>


Warrior::Warrior(Properties* props): Character(props)
{
    m_RigidBody = new RigidBody();
    m_Animation = new Animation();
    m_Animation->SetProps(m_TextureID, 1, 8, 200);
}

void Warrior::Draw()
{
    m_Animation->Draw(m_Transform->X, m_Transform->Y, m_Width, m_Height);
}

void Warrior::Update(float dt)
{
    m_Animation->SetProps("player", 1, 8, 200);
    m_RigidBody->UnSetForce();

    if(Input::GetInstance()->GetKeyDown(SDL_SCANCODE_A) || Input::GetInstance()->GetKeyDown(SDL_SCANCODE_LEFT)) {
        m_RigidBody->ApplyForceX(5*BACKWARD);
        m_Animation->SetProps("player_run", 1, 8, 200, SDL_FLIP_HORIZONTAL);
    }

    if(Input::GetInstance()->GetKeyDown(SDL_SCANCODE_D) || Input::GetInstance()->GetKeyDown(SDL_SCANCODE_RIGHT)) {
        m_RigidBody->ApplyForceX(5*FORWARD);
        m_Animation->SetProps("player_run", 1, 8, 200);
    }

    m_RigidBody->Update(dt);

    m_Transform->TranslateX(m_RigidBody->Position().X);
    //m_Transform->TranslateY(m_RigidBody->Position().Y);

    m_Animation->Update();

    m_Origin->X = m_Transform->X + m_Width/2;
    m_Origin->Y = m_Transform->Y + m_Height/2;
}

void Warrior::Clean()
{
    TextureManager::GetInstance()->Clean();
}
