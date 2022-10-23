//
// Created by cpp on 08.04.22.
//

#include "Entity.h"
#include "Game.h"

Entity::Entity(EntityTag tag, const size_t id) :mTag(tag),mId(id){}

void  Entity::destroy() {mActive = false;}
bool  Entity::isActive(){return mActive;}
const EntityTag Entity::getTag()const{return mTag;}
const size_t Entity::getId(){return mId;}