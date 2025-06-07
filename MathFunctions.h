#pragma once

#include <algorithm>

float asProbability(float x) {
    return std::clamp(0.0f, 1.0f, x);
}
