/**
  * Created by Joscha Vack on 11/23/2020.
  *
  **/

#include "Shooteroo.h"

#include <core/logger/Logger.h>

#include "Game.h"

Shooteroo::Shooteroo() : Application(ApplicationProps(
        "Shooteroo",
        WindowProps("Shooteroo", true, 16, true))),
        gameSettings (new GameSettings("settings.ini")) {

    std::string settingsPath = "settings.json";

    #ifdef NDEBUG
    bool showSettings = false;
    #else
    bool showSettings = true;
    #endif
    auto* layer = new Game(gameSettings, showSettings);

    pushLayer(layer);
}

Shooteroo::~Shooteroo() {
    delete gameSettings;
}
