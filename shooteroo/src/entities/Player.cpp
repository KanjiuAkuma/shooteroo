/**
  * Created by Joscha Vack on 11/20/2020.
  *
  **/

#include "Player.h"
#include "Bullet.h"
#include "Game.h"

#include <glm/gtx/vector_angle.hpp>

#include <Macros.h>
#include <core/window/Input.h>

Player::Player(const PlayerSettings* settings, glm::vec2 initialPosition) :
        settings(settings),
        Actor("Player", &settings->size,
              &settings->speed,
              &settings->turningSpeed,
              initialPosition, 0) {
}

void Player::onUpdate(float dt) {
    Actor::onUpdate(dt);
    float dc = dt * (ulting ? settings->ultCooldownMod : 1.f);
    weaponCooldown -= dc;
    boostCooldown -= dc;
    flashCooldown -= dc;

    boostCooldown -= dc;
    boostRemaining -= dt;
    if (boosting && boostRemaining < 0.f) {
        boosting = false;
        boostCooldown = settings->boostCooldown;
    }

    ultCooldown -= dt;
    ultRemaining -= dt;
    if (ulting && ultRemaining < 0.f) {
        ulting = false;
        ultCooldown = settings->ultCooldown;
    }
}

void Player::setTargetLocation(glm::vec2 targetLocation) {
    LOG_DEBUG("Set target position for player to [{}, {}]", targetLocation.x, targetLocation.y);
    Actor::setTargetLocation(targetLocation);
}

void Player::shoot(glm::vec2 direction) {
    if (weaponCooldown <= 0) {
        weaponCooldown = settings->weaponSettings.cooldown;

        glm::vec2 targetPosition = position + direction * settings->weaponSettings.pRange;

        orientation = targetOrientation = -glm::orientedAngle(direction, glm::vec2(1, 0));
        turning = false;

        glm::vec2 targetDirection = glm::normalize(targetPosition - position);

        LOG_DEBUG("Shooting at [{}, {}]", targetPosition.x, targetPosition.y);

        auto* bullet = new Bullet(
                &settings->weaponSettings,
                position,
                targetDirection);

        // todo: enable
        Game::get().addBullet(bullet);
    }
}

void Player::boost() {
    if (!boosting && boostCooldown < 0.f) {
        boosting = true;
        boostRemaining = settings->boostDuration;
    }
}

void Player::ult() {
    if (!ulting && ultCooldown < 0.f) {
        ulting = true;
        ultRemaining = settings->ultDuration;
    }
}

void Player::flash(glm::vec2 targetLocation) {
    if (flashCooldown < 0.f) {
        flashCooldown = settings->flashCooldown;
        glm::vec2 dx = targetLocation - position;
        glm::vec2 dxN = glm::normalize(dx);
        orientation = targetOrientation = -glm::orientedAngle(dxN, glm::vec2(1, 0));

        float dist = glm::length(dx);
        if (dist < settings->flashDistance) {
            position = targetLocation;
        }
        else {
            position += dxN * settings->flashDistance;
        }
    }
}

float Player::getSpeed() const {
    float speed = Actor::getSpeed();
    if (boosting) {
        speed *= settings->boostSpeedMod;
    }
    if (ulting) {
        speed *= settings->ultSpeedMod;
    }
    return speed;
}

bool Player::isInvulnerable() {
    return ulting;
}
