#pragma once

class MechanumKinematics {
public:
    MechanumKinematics(const float lx, const float ly, const float r)
        : lx_{lx}, ly_{ly}, r_{r}
    {
        //
    }

    void forward(const float vx, const float vy, const float w,
                 float& wfl, float& wfr, float& wrl, float& wrr) const {
        const float A = vx - vy;
        const float B = vx + vy;
        const float C = (lx_ + ly_) * w;
        wfl = (A - C) / r_;
        wfr = (B + C) / r_;
        wrl = (B - C) / r_;
        wrr = (A + C) / r_;
    }

    void inverse(const float wfl, const float wfr, const float wrl, const float wrr,
                 float& vx, float& vy, float& w) const {
        const float A = r_ / 4;
        const float B = 1.f / (lx_ + ly_);
        vx = ( wfl + wfr + wrl + wrr) * A;
        vy = (-wfl + wfr + wrl - wrr) * A;
        w  = (-wfl + wfr - wrl + wrr) * A * B;
    }

private:
    const float lx_;
    const float ly_;
    const float r_;
};