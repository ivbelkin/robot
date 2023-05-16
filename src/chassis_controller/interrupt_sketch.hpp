#pragma once

#include "sketch.h"
// #include "interrupt/interrupt.hpp"

class InterruptSketch : public Sketch {
public:

    void setup() {}

    void start() {
        for (int i = 0; i < 4; ++i) {
            pool_.enqueue(*interrupts_[i]);
        }
        interrupts();
    }

    bool ready() {
        return true;
    }

    void process() {}

    void finish() {
        noInterrupts();
    }

private:
    static IInterrupt* interrupts_[4];
    InterruptPool pool_;
};