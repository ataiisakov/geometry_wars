//
// Created by cpp on 08.04.22.
//

#ifndef GEOMETRY_WARS_COMPONENTS_H
#define GEOMETRY_WARS_COMPONENTS_H

#include "Common.h"

class CTransform {
public:
    Vec2 pos{0, 0}, velocity{0, 0};
    float angle{0};

    CTransform(const Vec2 &pos, const Vec2 &velocity, float angel)
            : pos(pos), velocity(velocity), angle(angel) {}
};

class CShape {
public:
    sf::CircleShape circle;

    CShape(float radius, int points, const sf::Color &fill, const sf::Color &outline, float thickness)
            : circle(radius, points) {
        circle.setFillColor(fill);
        circle.setOutlineColor(outline);
        circle.setOutlineThickness(thickness);
        circle.setOrigin(radius, radius);
    }
};

class CCollision {
public:
    float radius = 0;

    CCollision(float r) : radius(r) {}
};

class CScore {
public:
    int score = 0;

    CScore(int score) : score(score) {}
};

class CLifespan {
public:
    int remaining, total;

    CLifespan(int total) : remaining(total), total(total) {}
};

class CInput {
public:
    bool up{false};
    bool right{false};
    bool left{false};
    bool down{false};
    bool shoot{false};

    CInput() {}
};

#endif //GEOMETRY_WARS_COMPONENTS_H
