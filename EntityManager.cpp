//
// Created by cpp on 08.04.22.
//

#include "EntityManager.h"

EntityManager::EntityManager() {}

ptr<Entity> EntityManager::addEntity(const EntityTag tag) {
    auto e = ptr<Entity>(new Entity(tag,mTotalEntities++));
    mEntitiesToAdd.push_back(e);
    return e;
}


void EntityManager::update() {
    for (const auto e : mEntitiesToAdd){
        mEntities.push_back(e);
        mEntityMap[e->getTag()].push_back(e);
    }

    removeDeadEntities(mEntities);
    for(auto& [tag,entityVec] : mEntityMap){
        removeDeadEntities(entityVec);
    }
    mEntitiesToAdd.clear();

}

EntityVector &EntityManager::getEntities() {return mEntities;}

EntityVector &EntityManager::getEntities(const EntityTag tag) {return mEntityMap[tag];}

void EntityManager::removeDeadEntities(EntityVector &vector) {
    std::erase_if(vector,[](auto entity){return !entity->isActive();});
}
