/**
  * Created by Joscha Vack on 2/20/2020.
  *
  **/

#include "Random.h"

namespace Engine { namespace Util {

    std::random_device Random::random_device;
    std::mt19937 Random::engine(random_device());
    std::uniform_real_distribution<float> Random::dist(0, 1);
    std::uniform_real_distribution<float> Random::distSign(-1, 1);

    float Random::uRandom(float min, float max) {
        return min + dist(engine) * (max - min);
    }

    float Random::sRandom(float min, float max) {
        return copysignf(min + dist(engine) * (max - min), distSign(engine));
    }

}}
