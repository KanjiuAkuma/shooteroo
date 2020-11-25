#pragma once

/**
  * Created by Joscha Vack on 11/21/2020.
  *
  **/

#include "base/Actor.h"
#include "settings/GameSettings.h"

class Game;

class Hostile : public Actor {
public:
    Hostile(const HostileSettings* settings, bool small, glm::vec2 initialPosition, glm::vec2 playerPosition);

    void onHit();

private:
    friend Game;
};