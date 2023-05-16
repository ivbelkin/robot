#pragma once

#include "interrupt.hpp"

class IInterrupt;

class InterruptPool {
    friend class IInterrupt;

public:
    constexpr explicit InterruptPool(/*const int max_size*/)
        // : max_size_{max_size}
    {
        //
    }

    void enqueue(IInterrupt& interrupt);

    void join();

private:
    static const int max_size_{10};
    IInterrupt* queue_[max_size_]{nullptr};
    int head_{0};
    int tail_{0};

private:
    void notify();
};
