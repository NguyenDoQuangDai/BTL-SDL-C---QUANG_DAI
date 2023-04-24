#ifndef CAMERA_H
#define CAMERA_H

#include "SDL.h"
#include "Point.h"
#include "Vector2D.h"
#include "Engine.h"

class Camera
{
    public:
        void Update(float dt);

        inline SDL_Rect GetViewBox() {return m_ViewBox;} //lay vi tri camera hinh chu nhat
        inline Vector2D GetPosition() {return m_Position;} //lay vi tri chinh xac cua camera
        inline void SetTarget(Point* target) {m_Target = target;} //lay muc tieu ma camera theo duoi (trung tam)
        inline static Camera* GetInstance(){return s_Instance = (s_Instance != nullptr)? s_Instance : new Camera();} //static class


    private:
        Camera() {m_ViewBox = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}; } //set viewbox ban dau tu (0,0) den het cua so

        Point* m_Target;
        Vector2D m_Position;

        SDL_Rect m_ViewBox;
        static Camera* s_Instance;
};

#endif // CAMERA_H
