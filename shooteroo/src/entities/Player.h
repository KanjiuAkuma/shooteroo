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

    void shoot(glm::vec2 targetLocation);

    void boost();

    void ult();

    void flash(glm::vec2 direction);

    float getSpeed() const override;
    bool isInvulnerable();

private:
    const PlayerSettings* settings;
    bool boosting = false, ulting = false;
    float weaponCooldown = 0, flashCooldown = 0,
            boostRemaining = 0, boostCooldown = 0,
            ultCooldown = 0, ultRemaining = 0;

    friend Game;
};
