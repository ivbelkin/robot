#pragma once

struct Velocity {
    float vx{0.f};
    float vy{0.f};
    float w{0.f};
};

class VelocityController : public Sketch {
public:
    VelocityController(const Velocity& target_velocity, float (&target_rps)[4])
        : target_velocity_{target_velocity}, target_rps_{target_rps}
    {
        //
    }

    void setup() override {
        for (int i = 0; i < 4; ++i) {
            target_rps_[i] = 0;
        }
    }

    void start() override {
        const float l = 0.165;
        const float b = 0.215;
        const float d = 0.08;

        MechanumKinematics k{l / 2, b / 2, d / 2};

        float wfl, wfr, wrl, wrr;

        k.forward(target_velocity_.vx, target_velocity_.vy, target_velocity_.w, wfl, wfr, wrl, wrr);

        const float A = M_PI * 2;

        target_rps_[0] = -wfl / A;
        target_rps_[1] =  wfr / A;
        target_rps_[2] = -wrl / A;
        target_rps_[3] =  wrr / A;        
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
    const Velocity& target_velocity_;
    float (&target_rps_)[4];
};
