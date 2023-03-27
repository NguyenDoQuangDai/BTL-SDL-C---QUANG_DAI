#include "Timer.h"
#include "Engine.h"

int main(int argc, char** argv) {

    Engine::GetInstance()->Init();

    while(Engine::GetInstance()->IsRunning()) {
        Engine:: GetInstance()->Events();
        //check co event nao xay ra
        Engine::GetInstance()->Update();
        // tinh lai vi tri nv tren mh
        Engine::GetInstance()->Render();
        //tra ket qua
        Timer::GetInstance()->Tick();
    }

    Engine::GetInstance()->Clean();
    // don truoc khi thoat game
    return 0;
}
