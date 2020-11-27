/**
  * Created by Joscha Vack on 11/23/2020.
  *
  **/

#include "GameSettings.h"

#include <ini.h>

#include <core/logger/Logger.h>

#include "Definitions.h"


GameSettings::GameSettings(const std::string& filePath) : settingsFile(filePath) {
    LOG_INFO("Loading game settings");
    settingsFile.read(ini);
    if (!settingsFile.read(ini)) {
        LOG_WARN("Error loading settings!");
    }

    /// validate entity settings
    ENSURE_SECTION(ini, "player", playerDefaults);
    ENSURE_SECTION(ini, "weapon", weaponDefaults);
    ENSURE_SECTION(ini, "projectile", projectilesDefaults);
    ENSURE_SECTION(ini, "hostile", hostileDefaults);

    /// validate texture settings
    ENSURE_SECTION(ini, "grass_texture", grassTextureDefaults);
    ENSURE_SECTION(ini, "hitbox_texture", hitboxTextureDefaults);
    ENSURE_SECTION(ini, "player_texture", playerTextureDefaults);
    ENSURE_SECTION(ini, "bullet_texture", bulletTextureDefaults);
    ENSURE_SECTION(ini, "projectile_texture", projectileTextureDefaults);
    ENSURE_SECTION(ini, "hostile_texture", hostileTextureDefaults);

    /// validate score settings
    ENSURE_SECTION(ini, "score", scoreDefaults);

    // load settings
    grassTexture = LOAD_TEXTURE(ini, grass);
    hitboxTexture = LOAD_TEXTURE(ini, hitbox);
    playerSettings = {
            LOAD_TEXTURE(ini, player),
            {  // weapon
                    LOAD_TEXTURE(ini, bullet),
                    LOAD_FLOAT(ini, "weapon", "cooldown"),
                    LOAD_FLOAT(ini, "weapon", "p_range"),
                    LOAD_FLOAT(ini, "weapon", "p_size"),
                    LOAD_FLOAT(ini, "weapon", "p_speed"),
            },
            LOAD_FLOAT(ini, "player", "size"),
            LOAD_FLOAT(ini, "player", "speed"),
            LOAD_FLOAT(ini, "player", "turning_speed"),
            LOAD_FLOAT(ini, "player", "flash_cooldown"),
            LOAD_FLOAT(ini, "player", "flash_distance"),
            LOAD_FLOAT(ini, "player", "boost_cooldown"),
            LOAD_FLOAT(ini, "player", "boost_duration"),
            LOAD_FLOAT(ini, "player", "boost_speed_mod"),
            LOAD_FLOAT(ini, "player", "ult_cooldown"),
            LOAD_FLOAT(ini, "player", "ult_duration"),
            LOAD_FLOAT(ini, "player", "ult_speed_mod"),
            LOAD_FLOAT(ini, "player", "ult_cooldown_mod"),
    };
    projectileSettings = {
            LOAD_TEXTURE(ini, projectile),
            LOAD_FLOAT(ini, "projectile", "probability"),
            LOAD_FLOAT(ini, "projectile", "size"),
            LOAD_FLOAT(ini, "projectile", "speed"),
    };
    hostileSettings = {
            LOAD_TEXTURE(ini, hostile),
            LOAD_FLOAT(ini, "hostile", "probability"),
            LOAD_FLOAT(ini, "hostile", "size_small"),
            LOAD_FLOAT(ini, "hostile", "size_big"),
            LOAD_FLOAT(ini, "hostile", "speed"),
            LOAD_FLOAT(ini, "hostile", "turning_speed"),
    };

    // check if highscore exists
    if (!ini["score"].has("highscore")) {
        ini["score"]["highscore"] = "0.0";
    }

    scoreSettings = {
            LOAD_FLOAT(ini, "score", "highscore"),
            LOAD_FLOAT(ini, "score", "time_increment"),
            LOAD_FLOAT(ini, "score", "kill_increment"),
    };
}

GameSettings::~GameSettings() {
    LOG_DEBUG("Saving game settings");
    ini.clear();

    // update ini
    WRITE_FLOAT(ini, "weapon", "cooldown", playerSettings.weaponSettings.cooldown);
    WRITE_FLOAT(ini, "weapon", "p_range", playerSettings.weaponSettings.pRange);
    WRITE_FLOAT(ini, "weapon", "p_size", playerSettings.weaponSettings.pSize);
    WRITE_FLOAT(ini, "weapon", "p_speed", playerSettings.weaponSettings.pSpeed);

    WRITE_FLOAT(ini, "player", "size", playerSettings.size);
    WRITE_FLOAT(ini, "player", "speed", playerSettings.speed);
    WRITE_FLOAT(ini, "player", "turning_speed", playerSettings.turningSpeed);
    WRITE_FLOAT(ini, "player", "flash_cooldown", playerSettings.flashCooldown);
    WRITE_FLOAT(ini, "player", "flash_distance", playerSettings.flashDistance);
    WRITE_FLOAT(ini, "player", "boost_cooldown", playerSettings.boostCooldown);
    WRITE_FLOAT(ini, "player", "boost_duration", playerSettings.boostDuration);
    WRITE_FLOAT(ini, "player", "boost_speed_mod", playerSettings.boostSpeedMod);
    WRITE_FLOAT(ini, "player", "ult_cooldown", playerSettings.ultCooldown);
    WRITE_FLOAT(ini, "player", "ult_duration", playerSettings.ultDuration);
    WRITE_FLOAT(ini, "player", "ult_speed_mod", playerSettings.ultSpeedMod);
    WRITE_FLOAT(ini, "player", "ult_cooldown_mod", playerSettings.ultCooldownMod);

    WRITE_FLOAT(ini, "projectile", "probability", projectileSettings.probability);
    WRITE_FLOAT(ini, "projectile", "size", projectileSettings.size);
    WRITE_FLOAT(ini, "projectile", "speed", projectileSettings.speed);

    WRITE_FLOAT(ini, "hostile", "probability", hostileSettings.probability);
    WRITE_FLOAT(ini, "hostile", "size_small", hostileSettings.sizeSmall);
    WRITE_FLOAT(ini, "hostile", "size_big", hostileSettings.sizeBig);
    WRITE_FLOAT(ini, "hostile", "speed", hostileSettings.speed);
    WRITE_FLOAT(ini, "hostile", "turning_speed", playerSettings.turningSpeed);

    WRITE_TEXTURE(ini, grass, grassTexture);
    WRITE_TEXTURE(ini, hitbox, hitboxTexture);
    WRITE_TEXTURE(ini, bullet, playerSettings.weaponSettings.texture);
    WRITE_TEXTURE(ini, player, playerSettings.texture);
    WRITE_TEXTURE(ini, projectile, projectileSettings.texture);
    WRITE_TEXTURE(ini, hostile, hostileSettings.texture);

    WRITE_FLOAT(ini, "score", "highscore", scoreSettings.highscore);
    WRITE_FLOAT(ini, "score", "time_increment", scoreSettings.timeIncrement);
    WRITE_FLOAT(ini, "score", "kill_increment", scoreSettings.killIncrement);

    if (!settingsFile.generate(ini, true)) {
        LOG_WARN("Could not save settings!");
    }
}

void GameSettings::reset() {
    // todo: make this cleaner
    ini.clear();

    ini["player"].set(playerDefaults);
    ini["weapon"].set(weaponDefaults);
    ini["hostile"].set(hostileDefaults);
    ini["projectile"].set(projectilesDefaults);

    ini["grass_texture"].set(grassTextureDefaults);
    ini["hitbox_texture"].set(hitboxTextureDefaults);
    ini["player_texture"].set(playerTextureDefaults);
    ini["bullet_texture"].set(bulletTextureDefaults);
    ini["hostile_texture"].set(hostileTextureDefaults);
    ini["projectile_texture"].set(projectileTextureDefaults);

    ini["score"].set(scoreDefaults);

    grassTexture = LOAD_TEXTURE(ini, grass);
    hitboxTexture = LOAD_TEXTURE(ini, hitbox);

    playerSettings = {
            LOAD_TEXTURE(ini, player),
            {  // weapon
                    LOAD_TEXTURE(ini, bullet),
                    LOAD_FLOAT(ini, "weapon", "cooldown"),
                    LOAD_FLOAT(ini, "weapon", "p_range"),
                    LOAD_FLOAT(ini, "weapon", "p_size"),
                    LOAD_FLOAT(ini, "weapon", "p_speed"),
            },
            LOAD_FLOAT(ini, "player", "size"),
            LOAD_FLOAT(ini, "player", "speed"),
            LOAD_FLOAT(ini, "player", "turning_speed"),
            LOAD_FLOAT(ini, "player", "flash_cooldown"),
            LOAD_FLOAT(ini, "player", "flash_distance"),
            LOAD_FLOAT(ini, "player", "boost_cooldown"),
            LOAD_FLOAT(ini, "player", "boost_duration"),
            LOAD_FLOAT(ini, "player", "boost_speed_mod"),
            LOAD_FLOAT(ini, "player", "ult_cooldown"),
            LOAD_FLOAT(ini, "player", "ult_duration"),
            LOAD_FLOAT(ini, "player", "ult_speed_mod"),
            LOAD_FLOAT(ini, "player", "ult_cooldown_mod"),
    };
    projectileSettings = {
            LOAD_TEXTURE(ini, projectile),
            LOAD_FLOAT(ini, "projectile", "probability"),
            LOAD_FLOAT(ini, "projectile", "size"),
            LOAD_FLOAT(ini, "projectile", "speed"),
    };
    hostileSettings = {
            LOAD_TEXTURE(ini, hostile),
            LOAD_FLOAT(ini, "hostile", "probability"),
            LOAD_FLOAT(ini, "hostile", "size_small"),
            LOAD_FLOAT(ini, "hostile", "size_big"),
            LOAD_FLOAT(ini, "hostile", "speed"),
            LOAD_FLOAT(ini, "player", "turning_speed"),
    };
    scoreSettings = {
            scoreSettings.highscore,
            LOAD_FLOAT(ini, "score", "time_increment"),
            LOAD_FLOAT(ini, "score", "kill_increment"),
    };
}
