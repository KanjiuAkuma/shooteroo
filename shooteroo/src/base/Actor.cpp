/**
  * Created by Joscha Vack on 11/20/2020.
  *
  **/

#include "Actor.h"

#include <cmath>

#include <glm/gtx/vector_angle.hpp>

#include "Game.h"

#define PI  3.14159265359f
#define TAU 6.28318530718f

Actor::Actor(const std::string& debugName, const float* size, const float* speed, const float* turningSpeed,
             glm::vec2 initialPosition, float orientation) :
        Entity(debugName),
        size(size),
        speed(speed),
        turningSpeed(turningSpeed),
        orientation(orientation),
        targetOrientation(orientation),
        position(initialPosition),
        targetLocation(initialPosition),
        moving(false) {}

Actor::Actor(const std::string& debugName, const float* size, const float* speed, const float* turningSpeed,
             glm::vec2 initialPosition, glm::vec2 targetPosition) :
        Entity(debugName),
        size(size),
        speed(speed),
        turningSpeed(turningSpeed),
        orientation(-glm::angle(glm::normalize(targetPosition - initialPosition), Game::mainAxis)),
        targetOrientation(orientation),
        position(initialPosition),
        targetLocation(targetPosition),
        moving(true) {}

void Actor::onUpdate(float dt) {
    glm::vec2 dx = targetLocation - position;

    if (moving) {
        // update position
        targetOrientation = -glm::orientedAngle(glm::normalize(dx), Game::mainAxis);

        if (1.e-9 < abs(targetOrientation - orientation)) turning = true;
        float distance = glm::length(dx);
        float vt = getSpeed() * dt;

        if (vt < distance) {
            position += glm::normalize(dx) * vt;
        } else {
            position = targetLocation;
            moving = false;
        }
    }

    if (turning) {
        float diff = atan2(sin(targetOrientation - orientation), cos(targetOrientation - orientation));

        if (std::abs(diff) < (*turningSpeed)) {
            orientation = targetOrientation;
            turning = false;
        }
        else {
            float sign = diff < 0.f ? -1.f : 1.f;
            orientation += sign * (*turningSpeed);
        }
    }
}

void Actor::setTargetLocation(glm::vec2 targetLocation) {
    if (1e-9 < glm::distance(this->targetLocation, targetLocation)) {
        this->targetLocation = targetLocation;
        moving = true;
    }
}

void Actor::stopMoving() {
    moving = false;
}

bool Actor::isMoving() {
    return moving;
}

glm::vec2 Actor::getPosition() const {
    return position;
}

float Actor::getOrientation() const {
    return orientation;
}

float Actor::getSize() const {
    return *size;
}

float Actor::getSpeed() const {
    return *speed;
}
