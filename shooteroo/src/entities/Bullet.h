#pragma once

#include <settings/GameSettings.h>
#include "base/Actor.h"

/**
  * Created by Joscha Vack on 11/21/2020.
  *
  **/

class Game;

class Bullet : public Entity {
public:
    Bullet(const WeaponSettings* settings, glm::vec2 position, glm::vec2 direction);

    void onUpdate(float dt) override;
    void onHit();

    glm::vec2 getPosition() const override;
    float getOrientation() const override;
    float getSize() const override;

private:
    const float *size, *speed;
    float range;

    glm::vec2 position, direction;

    friend Game;
};
