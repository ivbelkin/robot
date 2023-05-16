#pragma once

#define PS2_DAT        52  //14    
#define PS2_CMD        51  //15
#define PS2_SEL        53  //16
#define PS2_CLK        50  //17
#define pressures   false
#define rumble      true

class PSController : public Sketch {
public:
    PSController(Velocity& target_velocity)
        : target_velocity_{target_velocity}
    {
        //
    }    

    void setup() override {
        ps2x_.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
    }

    void start() override {
        ps2x_.read_gamepad(false, false);
        if (ps2x_.Button(PSB_L1)) {
            const int LX = ps2x_.Analog(PSS_LX);
            const int LY = ps2x_.Analog(PSS_LY);
            const int RX = ps2x_.Analog(PSS_RX);

            target_velocity_.vx = 0.5f * (-LY + 127) / 128.f;
            target_velocity_.vy = 0.5f * (-LX + 128) / 128.f;
            target_velocity_.w = 2.f * (-RX + 128) / 128.f;

            Serial.print(target_velocity_.vy); Serial.print(" ");
            Serial.print(target_velocity_.vx); Serial.print(" ");
            Serial.print(target_velocity_.w); Serial.println();
        }
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
    Velocity& target_velocity_;
    PS2X ps2x_;
};