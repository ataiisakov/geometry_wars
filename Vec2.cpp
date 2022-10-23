//
// Created by cpp on 08.04.22.
//

#include "Vec2.h"
#include <cmath>

bool Vec2::operator==(const Vec2 &other) { return mX == other.mX && mY == other.mY; }

bool Vec2::operator!=(const Vec2 &other) { return mX != other.mX && mY != other.mY; }

void Vec2::operator+=(const Vec2 &other) {
    mX += other.mX;
    mY += other.mY;
}

void Vec2::operator*=(const float num) {
    mX *= num;
    mY *= num;
}

Vec2 Vec2::operator+(const Vec2 &other) { return Vec2(mX + other.mX, mY + other.mY); }

Vec2 Vec2::operator-(const Vec2 &other) const { return Vec2(mX - other.mX, mY - other.mY); }

Vec2 Vec2::operator*(const float num) { return Vec2(mX * num, mY * num); }

Vec2 Vec2::operator/(const float num) { return Vec2(mX / num, mY / num); }

float Vec2::dist(const Vec2 &other) const {
    return sqrt(((mX - other.mX) * (mX - other.mX)) + ((mY - other.mY) * (mY - other.mY)));
}

Vec2 &Vec2::add(float num) {
    mX += mX, mY += num;
    return *this;
}

double Vec2::length() { return std::sqrt(mX * mX + mY * mY); }

void Vec2::normalize() {
    double len = length();
    if (len != 0) {
        mX = mX / len;
        mY = mY / len;
    }
}