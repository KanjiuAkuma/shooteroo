#pragma once

/**
  * Created by Joscha Vack on 11/20/2020.
  *
  **/

#include <string>
#include <glm/glm.hpp>

#include <core/logger/Logger.h>

struct EntityVertex {
    glm::vec2 offset;
    glm::vec2 position;
    glm::vec2 texCoords;
};

class Game;

/// Interface definition for game entities
class Entity {
public:
    explicit Entity(const std::string& debugName);
    virtual ~Entity();

    // physics
    virtual void onUpdate(float dt) = 0;

    // debugging
    std::string debugName;

    static int baseId;
    int id;



    // rendering
    virtual glm::vec2 getPosition() const = 0;
    virtual float getOrientation() const = 0;
    virtual float getSize() const = 0;

    std::string toString() const;
private:
    friend Game;
};
