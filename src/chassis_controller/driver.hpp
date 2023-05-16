#pragma once

#include "sketch.h"

struct DriverPins {
    int pwm;
    int forward;
    int backward;
};

class Driver : public Sketch {
public:
    explicit Driver(const DriverPins& pins, float* value)
        : pins_{pins}, value_{value}
    {
        //
    }

    void setup() override {
        pinMode(pins_.pwm, OUTPUT);
        pinMode(pins_.forward, OUTPUT);
        pinMode(pins_.backward, OUTPUT);
    }

    void start() override {
        //
    }

    bool ready() override {
        return true;
    }

    void process() override {
        //
    }

    void finish() override {
        const int int_value = constrain(*value_ * 255, -255, 255);
        const bool direction = int_value >= 0;
        digitalWrite(pins_.forward, direction);
        digitalWrite(pins_.backward, !direction);
        analogWrite(pins_.pwm, abs(int_value));
    }

private:
    const DriverPins pins_;
    float* const value_{nullptr};
};
