#pragma once

class Sketch {
public:
    virtual void setup() = 0;

    virtual void start() = 0;

    virtual bool ready() = 0;

    virtual void process() = 0;

    virtual void finish() = 0;
};
