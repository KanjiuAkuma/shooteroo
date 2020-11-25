#pragma once

/**
  * Created by Joscha Vack on 2/20/2020.
  *
  **/

#include <random>

namespace Engine { namespace Util {

    class Random {
    private:
        static std::random_device random_device;
        static std::mt19937 engine;
        static std::uniform_real_distribution<float> dist;
        static std::uniform_real_distribution<float> distSign;

    public:
        static float uRandom(float min, float max);
        static float sRandom(float min, float max);
    };

}}
