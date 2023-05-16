#pragma once

#include <interrupt.hpp>

class Stopwatch {
public:

    void tick() {
        t_start_ = micros();
    }

    void tock() {
        elapsed_ = micros() - t_start_;
    }

    float elapsed() const {
        return 1e-6f * elapsed_;
    }

private:
    unsigned long t_start_{0};
    unsigned long elapsed_{0};
};

class InterruptableStopwatch : public Interruptable, public Stopwatch {
public:
    void isr() override {
        tock();
        tocktick();
        tick();
    }

    virtual void tocktick() = 0;
};
