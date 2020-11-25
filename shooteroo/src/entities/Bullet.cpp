/**
  * Created by Joscha Vack on 11/21/2020.
  *
  **/

#include "Bullet.h"

#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "base/Actor.h"
#include "Game.h"

Bullet::Bullet(const WeaponSettings* settings, glm::vec2 initialPosition, glm::vec2 direction) :
        Entity("Bullet"),
        size(&settings->pSize), speed(&settings->pSpeed), range(settings->pRange),
        position(initialPosition), direction(direction) {}

void Bullet::onUpdate(float dt) {
    float dx = (*speed) * dt;
    if (range < dx) {
        dx = range;
    }
    range -= dx;
    position += direction * dx;

    if (range == 0) {
        Game::get().removeBullet(this);
        delete this;
    }
}

void Bullet::onHit() {
    Game::get().removeBullet(this);
    delete this;
}

glm::vec2 Bullet::getPosition() const {
    return position;
}

float Bullet::getOrientation() const {
    return -glm::angle(direction, Game::mainAxis);
}

float Bullet::getSize() const {
    return *size;
}