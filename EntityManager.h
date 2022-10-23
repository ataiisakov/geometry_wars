//
// Created by cpp on 08.04.22.
//

#ifndef GEOMETRY_WARS_ENTITYMANAGER_H
#define GEOMETRY_WARS_ENTITYMANAGER_H

#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include "Entity.h"
#include "Common.h"


typedef std::vector<ptr<Entity>> EntityVector;
typedef std::map<EntityTag,EntityVector> EntityMap;

class EntityManager {
    EntityVector mEntities;
    EntityMap    mEntityMap;
    EntityVector mEntitiesToAdd;
    size_t       mTotalEntities{0};

    void removeDeadEntities(EntityVector& vector);
public:
    EntityManager();
    void update();
    ptr<Entity> addEntity(const EntityTag tag);
    EntityVector& getEntities();
    EntityVector& getEntities(const EntityTag tag);
};


#endif //GEOMETRY_WARS_ENTITYMANAGER_H
