#include "interrupt_pool.h"

void InterruptPool::enqueue(IInterrupt& interrupt) {
    interrupt.pool_ = this;
    queue_[tail_] = &interrupt;
    const bool attach = head_ == tail_;
    tail_ = (tail_ + 1) % max_size_;
    if (attach) {
        queue_[head_]->attach();
    }
}

void InterruptPool::notify() {
    head_ = (head_ + 1) % max_size_;
    if (head_ != tail_) {
        queue_[head_]->attach();
    }
}
