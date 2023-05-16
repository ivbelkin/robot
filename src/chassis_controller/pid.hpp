#pragma once

class PID {
public:
    PID(const float Kp, const float Ki = 0.f, const float Kd = 0.f)
        : Kp_{Kp}, Ki_{Ki}, Kd_{Kd}
    {
        //
    }

    // u(x, t)
    float control(const float x, float x_trg) {
        x_trg = constrain(x_trg, -4.5, 4.5);
        const unsigned long t = micros();
        const float e = x - x_trg;
        float u = Kp_ * e;
        if (t_prev_ != 0) {
            float dt = t - t_prev_;
            if (t_prev_ > t) {
                dt = UINT32_MAX - t_prev_ + t;
            }
            dt *= 1e-6f;
            I_ += e * dt;
            const float D = (x - x_prev_) / dt;
            u += Ki_ * I_ + Kd_ * D;
        }
        x_prev_ = x;
        t_prev_ = t;
        return constrain(u, -1.f, 1.f);
    }

    void reset() {
        I_ = 0.f;
        x_prev_ = 0.f;
        t_prev_ = 0;      
    }

private:
    const float Kp_;
    const float Ki_;
    const float Kd_;

    float I_{0.f};
    float x_prev_{0.f};
    unsigned long t_prev_{0};
};