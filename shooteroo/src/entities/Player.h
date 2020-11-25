#pragma once

/**
  * Created by Joscha Vack on 11/20/2020.
  *
  **/

#include "base/Actor.h"
#include "settings/GameSettings.h"

class Game;

class Player : public Actor {
public:
    Player(const PlayerSettings* settings, glm::vec2 initialPosition);

    void setTargetLocation(glm::vec2 targetLocation) override;

    void onUpdate(float dt) override;

    void shoot(glm::vec2 direction);

private:
    const PlayerSettings* settings;
    float weaponCooldown = 0;

    friend Game;
};
