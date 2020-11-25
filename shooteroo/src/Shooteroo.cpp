/**
  * Created by Joscha Vack on 11/23/2020.
  *
  **/

#include "Shooteroo.h"

#include <core/logger/Logger.h>

#include "Game.h"

Shooteroo::Shooteroo() : Application(ApplicationProps(
        "Shooteroo",
        WindowProps("Shooteroo", true, 16, false, 1000, 1000))),
        gameSettings (new GameSettings("settings.ini")) {

    std::string settingsPath = "settings.json";


    bool showSettings = false;
    auto* layer = new Game(gameSettings, showSettings);

    pushLayer(layer);
}

Shooteroo::~Shooteroo() {
    delete gameSettings;
}
