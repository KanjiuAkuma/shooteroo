#pragma once

/**
  * Created by Joscha Vack on 11/20/2020.
  *
  **/

#include "Entity.h"
#include <glm/glm.hpp>

class Game;

class Actor : public Entity {
public:
    Actor(const std::string& debugName, const float* size, const float* speed, const float* turningSpeed, glm::vec2 initialPosition, float orientation);
    Actor(const std::string& debugName, const float* size, const float* speed, const float* turningSpeed, glm::vec2 initialPosition, glm::vec2 targetPosition);

    // public functions
    virtual void setTargetLocation(glm::vec2 targetLocation);

    void stopMoving();

    bool isMoving();

    // physics
    virtual void onUpdate(float dt) override;

    // rendering
    virtual glm::vec2 getPosition() const override;

    virtual float getOrientation() const override;

    virtual float getSize() const override;

    virtual float getSpeed() const;

protected:
    const float *size, *speed, *turningSpeed;
    float orientation, targetOrientation;

    glm::vec2 position;
    glm::vec2 targetLocation;
    bool moving, turning;

    friend Game;
};
