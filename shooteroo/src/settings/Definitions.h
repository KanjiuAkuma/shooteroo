#pragma once

/**
  * Created by Joscha Vack on 11/23/2020.
  *
  **/

#include <string>
#include <ini.h>

#include <core/logger/Logger.h>

using Section = typename std::vector<std::pair<std::string, std::string>>;
using string_view_t = fmt::basic_string_view<char>;

template<typename... Args>
void log_warn(string_view_t fmt, const Args& ... args) {
    Engine::Logger::getLogger()->warn(fmt, args...);
}

#define ENSURE_SECTION(obj, name, dValue) {\
    if (!obj.has(name)) {\
        obj[name].set(dValue);\
        log_warn("Section {} not found, using defaults", name);\
    } else {\
        for (auto kv : dValue) {\
            std::string vName = kv.first;\
            if (!obj[name].has(vName)) {\
                log_warn("Value {}.{} nout found, using default", name, vName);\
                obj[name][vName] = kv.second;\
            }\
        }\
    }\
}

#define LOAD_TEXTURE(obj, name) {\
    obj[#name"_texture"]["file"], std::stof(obj[#name"_texture"]["scale"]), std::stof(obj[#name"_texture"]["rotation"]), \
    std::stof(obj[#name"_texture"]["offset_x"]), std::stof(obj[#name"_texture"]["offset_y"]), \
}

#define LOAD_FLOAT(obj, section, name) std::stof(obj[section][name])

#define WRITE_TEXTURE(obj, name, tex)\
    obj[#name"_texture"]["file"] = tex.filePath;\
    obj[#name"_texture"]["scale"] = std::to_string(tex.scale);\
    obj[#name"_texture"]["rotation"] = std::to_string(tex.rotation);\
    obj[#name"_texture"]["offset_x"] = std::to_string(tex.offsetX);\
    obj[#name"_texture"]["offset_y"] = std::to_string(tex.offsetY);

#define WRITE_FLOAT(obj, section, name, val) obj[section][name] = std::to_string(val);

/***
 *  values validators (default values)
 */

/// Entities
const Section playerDefaults = {
        {"size",  "0.06"},
        {"speed", "0.4"},
        {"turning_speed", "0.5"},
};

const Section weaponDefaults = {
        {"cooldown", "0.2"},
        {"p_range",  "0.9"},
        {"p_size",   "0.009"},
        {"p_speed",  "1.3"},
};

const Section projectilesDefaults = {
        {"probability", "0.765"},
        {"size",        "0.04"},
        {"speed",       "0.62"},
};

const Section hostileDefaults = {
        {"probability", "0.55"},
        {"size_small",  "0.055"},
        {"size_big",    "0.06"},
        {"speed",       "0.5"},
        {"turning_speed", "0.3"},
};


/// Textures
const Section grassTextureDefaults = {
        {"file",     R"(resources/textures/shooteroo/grass_tex.png)"},
        {"scale",    "1"},
        {"rotation", "0"},
        {"offset_x", "0"},
        {"offset_y", "0"},
};

const Section hitboxTextureDefaults = {
        {"file",     R"(resources/textures/shooteroo/hitbox_tex.png)"},
        {"scale",    "1"},
        {"rotation", "0"},
        {"offset_x", "0"},
        {"offset_y", "0"},
};

const Section playerTextureDefaults = {
        {"file",     R"(resources/textures/shooteroo/player_tex.png)"},
        {"scale",    "1.6"},
        {"rotation", "-0.45"},
        {"offset_x", "0.33"},
        {"offset_y", "-0.04"},
};

const Section bulletTextureDefaults = {
        {"file",     R"(resources/textures/shooteroo/bullet_tex.png)"},
        {"scale",    "1"},
        {"rotation", "0"},
        {"offset_x", "0"},
        {"offset_y", "0"},
};

const Section projectileTextureDefaults = {
        {"file",     R"(resources/textures/shooteroo/projectile_tex.png)"},
        {"scale",    "1.3"},
        {"rotation", "0"},
        {"offset_x", "0"},
        {"offset_y", "0"},
};

const Section hostileTextureDefaults = {
        {"file",     R"(resources/textures/shooteroo/hostile_tex.png)"},
        {"scale",    "1.3"},
        {"rotation", "0"},
        {"offset_x", "0.1"},
        {"offset_y", "0.2"},
};

/// Score

const Section scoreDefaults = {
        {"time_increment", "1.0"},
        {"kill_increment", "10.0"},
};