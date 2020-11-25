/**
  * Created by Joscha Vack on 11/21/2020.
  *
  **/

#include "Hostile.h"
#include "Game.h"

Hostile::Hostile(const HostileSettings* settings, bool small, glm::vec2 initialPosition, glm::vec2 playerPosition) :
        Actor("Hostile",
              small ? &settings->sizeSmall : &settings->sizeBig,
              &settings->speed, &settings->turningSpeed,
              initialPosition, playerPosition) {}

void Hostile::onHit() {
    Game::get().removeHostile(this);
    delete this;
}
