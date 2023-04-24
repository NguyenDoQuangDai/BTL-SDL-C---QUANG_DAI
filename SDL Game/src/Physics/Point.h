#ifndef POINT_H
#define POINT_H

#include<iostream>

class Point
{
public:

	float X, Y; //X,Y theo 2 truc hoanh tung

public:

	Point(float X = 0, float Y = 0) : X(X), Y(Y) {}

    // tinh vi tri va di chuyen (+ - *) giong nhu trong header file vector2D.h

    //cong vector
	inline Point operator + (const Point& p2) const {
		return Point(X + p2.X, Y + p2.Y);
	}

	inline friend Point operator += (Point& p1, const Point& p2) {
		p1.X += p2.X;
		p1.Y += p2.Y;

		return p1; //tra ve vector p1 sau khi tang them vector p2
	}

    //tru vector
	inline Point operator - (const Point& p2) const {
		return Point(X - p2.X, Y - p2.Y);
	}

	inline friend Point operator -= (Point& p1, const Point& p2) {
		p1.X -= p2.X;
		p1.Y -= p2.Y;

		return p1;  //tra ve vector p1 sau khi bot di vector p2
	}

    //nhan vector voi 1 so
	inline Point operator * (const float scalar) const {
		return Point(X * scalar, Y * scalar);
	}

    //hien thi kq cho debug
	void Log(std::string msg = "") {
		std::cout << msg << "(X Y) = (" << X << " " << Y << ")" << std::endl;
	}
};


#endif // POINT_H
