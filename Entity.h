//
// Created by cpp on 08.04.22.
//

#ifndef GEOMETRY_WARS_ENTITY_H
#define GEOMETRY_WARS_ENTITY_H
#include <iostream>
#include "Common.h"
#include "Components.h"

class Entity {
    EntityTag    mTag{EntityTag::Default};
    bool         mActive{true};
    size_t       mId{0};
    friend class EntityManager;

    Entity(const EntityTag tag, const size_t id);

public:
    ptr<CTransform> cTransform;
    ptr<CShape>     cShape;
    ptr<CCollision> cCollision;
    ptr<CInput>     cInput;
    ptr<CScore>     cScore;
    ptr<CLifespan>  cLifespan;

    void destroy();
    bool isActive();
    const EntityTag getTag() const;
    const size_t getId();
};


#endif //GEOMETRY_WARS_ENTITY_H
