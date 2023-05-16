#pragma once

#include "sketch.h"

const float Kp = -0.05f;
const float Ki = -1.0f;
const float Kd = 0.0f;

struct Control {
    bool ready;
    float value;
};

class WheelController : public Sketch {
public:
    explicit WheelController(Measurement* measurement, Control* control, float* target)
        : measurement_{measurement},
          control_{control},
          target_{target},
          pid_{Kp, Ki, Kd}
    {
        //
    }

    void setup() override {
        verbose_ = false;
    }

    void start() override {
        control_->ready = false;
    }

    bool ready() override {
        return control_->ready;
    }    

    void process() override {
        if (measurement_->ready) {
            control_->value = pid_.control(measurement_->value, *target_);            
            control_->ready = true;
        }
    }

    void finish() override {
        if (verbose_) {
            log_values();
        }
    }

private:
    PID pid_;
    bool verbose_{false};

    Measurement* measurement_{nullptr};
    Control* control_{nullptr};
    float* target_{nullptr};

private:
    void log_values() const {
        Serial.print(measurement_->value); Serial.print(" ");
        // Serial.print(target_); Serial.print(" ");
        Serial.print(control_->value); Serial.print(" ");
    }
};
