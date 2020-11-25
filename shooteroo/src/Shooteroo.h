#pragma once

/**
  * Created by Joscha Vack on 11/23/2020.
  *
  **/

#include <core/application/Application.h>

#include "settings/GameSettings.h"

using namespace Engine;

class Shooteroo : public Application {
public:
    Shooteroo();
    ~Shooteroo();
private:
    GameSettings* gameSettings;
};
