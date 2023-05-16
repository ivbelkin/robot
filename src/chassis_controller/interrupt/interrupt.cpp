#include "interrupt.hpp"
#include "interrupt_pool.h"

void IInterrupt::detach() {
    detach_impl();
    if (pool_) {
        pool_->notify();
    }
}
