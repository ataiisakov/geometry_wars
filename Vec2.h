//
// Created by cpp on 08.04.22.
//

#ifndef GEOMETRY_WARS_VEC2_H
#define GEOMETRY_WARS_VEC2_H
#include <cmath>

class Vec2 {
public:
    float mX{0}, mY{0};
    Vec2(){}
    Vec2(float x, float y) :mX(x),mY(y){}

    Vec2 operator + (const Vec2& other);
    Vec2 operator - (const Vec2& other) const;
    bool operator == (const Vec2& other);
    bool operator != (const Vec2& other);
    void operator += (const Vec2& other);
    void operator *= (const float num);
    Vec2 operator * (const float num);
    Vec2 operator / (const float num);
    float dist(const Vec2& other) const;
    Vec2& add(float num);
    double length();
    void normalize();
};


#endif //GEOMETRY_WARS_VEC2_H
