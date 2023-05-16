#pragma once

class SerialController : public Sketch {
public:
    SerialController(Velocity& target_velocity)
        : target_velocity_{target_velocity}
    {
        //
    }

    void setup() override {
        //
    }

    void start() override {
        if (Serial.available()) {
            target_velocity_.vx = Serial.parseFloat();
            target_velocity_.vy = Serial.parseFloat();
            target_velocity_.w = Serial.parseFloat();
        }
    }

    bool ready() override {
        return true;
    }

    void process() override {
        //
    }

    void finish() override {
        //
    }

private:
    Velocity& target_velocity_;
};