/**
  * Created by Joscha Vack on 11/21/2020.
  *
  **/

#include "Projectile.h"

#define TAU 6.28318530718f

Projectile::Projectile(const ProjectileSettings* settings, glm::vec2 position, glm::vec2 direction, float angularVelocity) :
        Entity("Projectile"),
        size(&settings->size), speed(&settings->speed), omega(angularVelocity),
        position(position), direction(direction) {}

void Projectile::onUpdate(float dt) {
    position += direction * (*speed) * dt;

    rotation += omega * dt;
    if (TAU < rotation) {
        rotation -= TAU;
    }
}

glm::vec2 Projectile::getPosition() const {
    return position;
}

float Projectile::getOrientation() const {
    return rotation;
}

float Projectile::getSize() const {
    return *size;
}
