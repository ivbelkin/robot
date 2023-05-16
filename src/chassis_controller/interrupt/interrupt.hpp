#pragma once

#include <Arduino.h>

class InterruptPool;

class IInterrupt {
    friend class InterruptPool;

public:  
    virtual void attach() = 0;

    void detach();
    virtual void detach_impl() = 0;

private:
    InterruptPool* pool_{nullptr};
};

class Interruptable {
public:
    void set_interrupt(const IInterrupt& interrupt) {
        interrupt_ = &interrupt;
    }

    void detach() {
        interrupt_->detach();
    }

public:
    virtual void on_attach() = 0;
    virtual void isr() = 0;

private:
    const IInterrupt* interrupt_ = nullptr;
};

// template<typename Callable, Callable* callback>
// class Interrupt : public IInterrupt {
// protected:
//     static void isr() {
//         (*callback)();
//     }
// };

template<typename Interruptable, Interruptable* interruptable>
class InterruptableInterrupt : public IInterrupt {
public:
    void attach() override {
        interruptable->on_attach();
    }

protected:
    static void isr() {
        noInterrupts();
        interruptable->isr();
        interrupts();
    }
};

// template<typename Callable, Callable* callback>
// class ExternalInterrupt : public Interrupt<Callable, callback> {
// public:
//     constexpr explicit ExternalInterrupt(const int pin)
//         : pin_{pin}
//     {
//         //
//     }

//     void attach() const override {
//         pinMode(pin_, INPUT_PULLUP);
//         attachInterrupt(digitalPinToInterrupt(pin_), &Interrupt<Callable, callback>::isr, RISING);
//     }

//     void detach() const override {
//         detachInterrupt(digitalPinToInterrupt(pin_));
//     }

// private:
//     const int pin_;
// };

template<typename Interruptable, Interruptable* interruptable>
class ExternalInterruptableInterrupt : public InterruptableInterrupt<Interruptable, interruptable> {
public:
    explicit ExternalInterruptableInterrupt(const int pin)
        : pin_{pin}
    {
        interruptable->set_interrupt(*this);
    }

    void attach() override {
        pinMode(pin_, INPUT_PULLUP);
        attachInterrupt(digitalPinToInterrupt(pin_), &InterruptableInterrupt<Interruptable, interruptable>::isr, RISING);
        InterruptableInterrupt<Interruptable, interruptable>::attach();
    }

    void detach_impl() override {
        detachInterrupt(digitalPinToInterrupt(pin_));
    }

private:
    const int pin_;
};
