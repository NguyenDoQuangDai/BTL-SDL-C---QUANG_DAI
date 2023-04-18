#ifndef TIMER_H
#define TIMER_H

const int TARGET_FPS = 60; // frame/s
const float TARGET_DELTATIME = 1.5f; //1.5ms

class Timer
{
    public:
        void Tick(); // -> SDL_GetTicks
        inline float GetDeltaTime() {return m_DeltaTime;} //ham tra ve member_DeltaTime
        inline static Timer* GetInstance() {return s_Instance = (s_Instance != nullptr)? s_Instance : new Timer();}
        //class static

    private:
        Timer() {};
        static Timer* s_Instance; //dinh nghia static_Instance
        float m_DeltaTime; //khoang thoi gian
        float m_LastTime; //...
};

#endif // TIMER_H
