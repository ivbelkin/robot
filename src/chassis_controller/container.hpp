#pragma once

class Container : public Sketch {
public:
    void add(Sketch& sketch) {
        sketches_[size_++] = &sketch;
    }

    void setup() override {
        for (int i = 0; i < size_; ++i) {
            sketches_[i]->setup();
        }
    }

    void start() override {
        for (int i = 0; i < size_; ++i) {
            sketches_[i]->start();
        }
    }

    bool ready() override {
        bool all = true;
        for (int i = 0; i < size_; ++i) {
            all &= sketches_[i]->ready();
        }
        return all;
    }

    void process() override {
        for (int i = 0; i < size_; ++i) {
            if (!sketches_[i]->ready()) {
                sketches_[i]->process();
            }
        }
    }

    void finish() override {
        for (int i = 0; i < size_; ++i) {
            sketches_[i]->finish();
        }
    }

private:
    static const int max_size_{15};
    Sketch* sketches_[max_size_];
    int size_{0};
};
