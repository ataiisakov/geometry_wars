//
// Created by cpp on 08.04.22.
//

#ifndef GEOMETRY_WARS_COMMON_H
#define GEOMETRY_WARS_COMMON_H


#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>
#include "Vec2.h"


const float PI = std::acos(-1);
enum class EntityTag {
    Player, Enemy, SmallEnemy, Bullet, Default
};

static float toRadians(float degree) {
    return degree * (PI / 180);
}

template<class T> using ptr = std::shared_ptr<T>;
#endif //GEOMETRY_WARS_COMMON_H
