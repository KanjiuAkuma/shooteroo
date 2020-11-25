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

void Player::shoot(glm::vec2 direction) {
    if (weaponCooldown <= 0) {
        weaponCooldown = settings->weaponSettings.cooldown;

        glm::vec2 targetPosition = position + direction * settings->weaponSettings.pRange;

        static std::default_random_engine e;
        static std::uniform_real_distribution<float> dis;

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

void Player::onUpdate(float dt) {
    Actor::onUpdate(dt);
    weaponCooldown -= dt;
}

void Player::setTargetLocation(glm::vec2 targetLocation) {
    LOG_DEBUG("Set target position for player to [{}, {}]", targetLocation.x, targetLocation.y);
    Actor::setTargetLocation(targetLocation);
}
