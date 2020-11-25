#pragma once

/**
  * Created by Joscha Vack on 11/21/2020.
  *
  **/

#include <glm/glm.hpp>

#include "base/Entity.h"
#include "settings/GameSettings.h"

class Game;

class Projectile : public Entity {
public:
    Projectile(const ProjectileSettings* settings, glm::vec2 position, glm::vec2 direction, float angularVelocity);

    void onUpdate(float dt) override;

    glm::vec2 getPosition() const override;
    float getOrientation() const override;
    float getSize() const override;

private:
    const float *size, *speed;
    float omega, rotation = 0;

    glm::vec2 position, direction;

    friend Game;
};
