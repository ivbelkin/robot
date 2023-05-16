#pragma once

#include "encoder.hpp"
#include "driver.hpp"

struct WheelParams {
    EncoderParams encoder;
    DriverPins driver;
};
