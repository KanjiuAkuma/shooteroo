/**
  * Created by Joscha Vack on 11/21/2020.
  *
  **/

#include "Entity.h"

int Entity::baseId = 0;

Entity::Entity(const std::string& debugName) :
        debugName(debugName), id(baseId++) {
    LOG_DEBUG("Created entity {}", toString());
}

Entity::~Entity() {
    LOG_DEBUG("Deleted entity {}", toString());
}

std::string Entity::toString() const {
    return debugName + " id=" + std::to_string(id);
}