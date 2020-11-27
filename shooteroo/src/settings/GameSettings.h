#pragma once

/**
  * Created by Joscha Vack on 11/23/2020.
  *
  **/

#include <string>
#include <glm/glm.hpp>

#include <ini.h>

struct TextureSettings {
    std::string filePath;
    float scale, rotation, offsetX, offsetY;
};

struct WeaponSettings {
    TextureSettings texture;
    float cooldown,
            pRange, pSize, pSpeed;
};

struct PlayerSettings {
    TextureSettings texture;
    WeaponSettings weaponSettings;

    float size, speed, turningSpeed,
            flashCooldown, flashDistance,
            boostCooldown, boostDuration, boostSpeedMod,
            ultCooldown, ultDuration, ultSpeedMod, ultCooldownMod;
};

struct ProjectileSettings {
    TextureSettings texture;
    float probability, size, speed;
};

struct HostileSettings {
    TextureSettings texture;
    float probability,
            sizeSmall, sizeBig,
            speed, turningSpeed;
};

struct ScoreSettings {
    float highscore, timeIncrement, killIncrement;
};

// pre define game to friend it
class Game;

class GameSettings {
public:
    explicit GameSettings(const std::string& filePath);

    ~GameSettings();

    void reset();

private:
    mINI::INIFile settingsFile;
    mINI::INIStructure ini;

    TextureSettings grassTexture;
    TextureSettings hitboxTexture;

    PlayerSettings playerSettings;
    ProjectileSettings projectileSettings;
    HostileSettings hostileSettings;

    ScoreSettings scoreSettings;

    friend Game;
};
