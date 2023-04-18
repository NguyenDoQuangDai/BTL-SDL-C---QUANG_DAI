#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Vector2D.h"


class Transform //tinh tien
{
    public:
        float X, Y;


    public:
        Transform(float x = 0, float y = 0): X(x), Y(y) {} //tuong tu chg trinh con trong class vector2D

        void Log(std::string msg = "") {
            std::cout << msg << "(X Y) = (" << X << " " << Y << ")" << std::endl; //hien thi kq vector
        }


    public:
        inline void TranslateX(float x) {X += x;} //tinh tien theo truc x
        inline void TranslateY(float y) {Y += y;} //tinh tien theo truc y
        inline void Translate(Vector2D v) {X += v.X; Y += v.Y;} //tinh tien theo huong vector
};

#endif // TRANSFORM_H
