#pragma once

#include <interrupt.hpp>
#include "stopwatch.hpp"
#include "sketch.h"

struct EncoderParams {
    int direction_pin;
    int ticks_per_round;
    float min_rps;
};

struct Measurement {
    bool ready;
    float value;
};

class Encoder : public InterruptableStopwatch, public Sketch {
public:
    explicit Encoder(const EncoderParams& params, Measurement* measurement)
        : direction_pin_{params.direction_pin},
          rounds_per_tick_{1.f / params.ticks_per_round},
          timeout_{rounds_per_tick_ / params.min_rps},
          measurement_{measurement}
    {
        //
    }

    void setup() override {
        pinMode(direction_pin_, INPUT);
    }

    void start() override {
        measurement_->ready = false;      
    }

    bool ready() override {
        return measurement_->ready;
    }

    void process() override {
        noInterrupts();
        check_stopped();
        if (status_ == MEASURE_COMPLETED) {
            status_ = NOT_MEASURING;
            measurement_->value = rounds_per_sec();
            measurement_->ready = true;
        }
        interrupts();
    }

    void finish() override {
        //
    }

private:
    enum Status {
        NOT_MEASURING, WAITING, INITIALIZATION, MEASURING, MEASURE_COMPLETED
    } status_{NOT_MEASURING};

    int direction_pin_;
    const float rounds_per_tick_;
    const float timeout_;

    Measurement* measurement_{nullptr};

private:
    void check_stopped() {
        switch (status_) {
            case NOT_MEASURING:
                break;

            case WAITING:
            case INITIALIZATION:
            case MEASURING:
                if (timeout()) {
                    direction_ = 0;
                    finish_();
                }
                break;

            case MEASURE_COMPLETED:
                break;
        }        
    }

    float rounds_per_sec() const {
        return direction_ ? rounds_per_tick_ * direction_ / elapsed() : 0.f;
    }

    bool timeout() const {
        timeout_sw_.tock();
        return timeout_sw_.elapsed() > timeout_;
    }

public:
    void tocktick() override {
        switch (status_) {
            case NOT_MEASURING:
                break;
              
            case WAITING:
                status_ = INITIALIZATION;
                timeout_sw_.tick();
                break;

            case INITIALIZATION:
                status_ = MEASURING;
                timeout_sw_.tick();
                break;

            case MEASURING:
                direction_ = digitalRead(direction_pin_) * 2 - 1;
                finish_();
                break;

            case MEASURE_COMPLETED:
                break;
        }
    }

    void on_attach() override {
        status_ = WAITING;
        timeout_sw_.tick();
    }

private:
    void finish_() {
        status_ = MEASURE_COMPLETED;
        detach();
    }

private:
    Stopwatch timeout_sw_;
    int direction_{0};
};
