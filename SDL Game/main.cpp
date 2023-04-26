#include "Timer.h"
#include "Engine.h"
#include "Warrior.h"

#include<iostream>

int main(int argc, char** argv) {

    Engine::GetInstance()->Init();

    while(Engine::GetInstance()->IsRunning()) {
        Engine:: GetInstance()->Events();
        //ktra event xay ra
        Engine::GetInstance()->Update();
        // tinh lai vi tri object tren mh
        Engine::GetInstance()->Render();
        //tra ket qua
        Timer::GetInstance()->Tick();
        //tra DeltaTime
        }
    Engine::GetInstance()->Clean();
    // don truoc khi thoat game
    return 0;
}
