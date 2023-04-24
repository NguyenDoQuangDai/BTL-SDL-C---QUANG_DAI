#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <iostream>
#include<string>

#include<SDL.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include<SDL_ttf.h>

class Vector2D
{
    public:
        float X, Y; //X,Y theo 2 truc hoanh tung

    public:
        Vector2D(float x = 0, float y = 0): X(x), Y(y) {} //dinh nghia vector voi 2 thanh phan X, Y



    public:
        //cong vector
        inline Vector2D operator+(const Vector2D& v2) const {
            return Vector2D(X + v2.X, Y + v2.Y);
        }
        //tru vector
        inline Vector2D operator-(const Vector2D& v2) const {
            return Vector2D(X - v2.X, Y - v2.Y);
        }
        //nhan vector voi 1 so
        inline Vector2D operator*(const float scalar) const {
            return Vector2D(X*scalar, Y*scalar);
        }
        //hien thi kq vector cho debug
        void Log(std::string msg = "") {
            std::cout << msg << "(X Y) = (" << X << " " << Y << ")" << std::endl;
        }

};

#endif // VECTOR2D_H
