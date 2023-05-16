#include <ros.h>

// #include <nav_msgs/Odometry.h>
#include <geometry_msgs/Twist.h>

#include <PS2X_lib.h>

#include <interrupt.hpp>
#include <interrupt_pool.h>
#include "wheel.hpp"
#include "pid.hpp"
#include "wheel_controller.hpp"
#include "container.hpp"
#include "mechanum_kinematics.hpp"
#include "interrupt_sketch.hpp"
#include "velocity_controller.hpp"
#include "serial_controller.hpp"
#include "ps_controller.hpp"

constexpr int N = 4;
constexpr int TICKS_PER_ROUND = 360;
constexpr float MIN_RPS = 0.10;

constexpr WheelParams params[N] = {
    WheelParams{EncoderParams{31, TICKS_PER_ROUND, MIN_RPS}, DriverPins{12, 34, 35}},
    WheelParams{EncoderParams{38, TICKS_PER_ROUND, MIN_RPS}, DriverPins{ 8, 37, 36}},
    WheelParams{EncoderParams{49, TICKS_PER_ROUND, MIN_RPS}, DriverPins{ 6, 43, 42}},
    WheelParams{EncoderParams{A1, TICKS_PER_ROUND, MIN_RPS}, DriverPins{ 5, A4, A5}}
};

struct State {
    Measurement measurements[N];

    Velocity target_velocity;

    float target_rps[N];

    Control control[N];
} state;

Encoder encoder_0{params[0].encoder, &state.measurements[0]};
Encoder encoder_1{params[1].encoder, &state.measurements[1]};
Encoder encoder_2{params[2].encoder, &state.measurements[2]};
Encoder encoder_3{params[3].encoder, &state.measurements[3]};
Encoder* encoders[N] = {&encoder_0, &encoder_1, &encoder_2, &encoder_3};

InterruptSketch interrupt_sketch;

ExternalInterruptableInterrupt<Encoder, &encoder_0> interrupt_0{18};
ExternalInterruptableInterrupt<Encoder, &encoder_1> interrupt_1{19};
ExternalInterruptableInterrupt<Encoder, &encoder_2> interrupt_2{ 3};
ExternalInterruptableInterrupt<Encoder, &encoder_3> interrupt_3{ 2};

IInterrupt* InterruptSketch::interrupts_[N] = {&interrupt_0, &interrupt_1, &interrupt_2, &interrupt_3};

Driver drivers[N] = {
    Driver{params[0].driver, &state.control[0].value},
    Driver{params[1].driver, &state.control[1].value},
    Driver{params[2].driver, &state.control[2].value},
    Driver{params[3].driver, &state.control[3].value}
};

WheelController controllers[N] = {
    WheelController{&state.measurements[0], &state.control[0], &state.target_rps[0]},
    WheelController{&state.measurements[1], &state.control[1], &state.target_rps[1]},
    WheelController{&state.measurements[2], &state.control[2], &state.target_rps[2]},
    WheelController{&state.measurements[3], &state.control[3], &state.target_rps[3]}
};

Container container;

VelocityController velocity_controller{state.target_velocity, state.target_rps};

SerialController serial_controller{state.target_velocity};

PSController ps_controller{state.target_velocity};

template<typename CallableT, CallableT* callable>
class Callback {
public:

    static void callback(const geometry_msgs::Twist& msg) {
        (*callable)(msg);
    }

};

class RosController : public Sketch {
public:
    RosController(ros::NodeHandle& nh, Velocity& target_velocity)
        : nh_{nh},
          sub_{"cmd_vel", &RosController::callback},
          target_velocity_{target_velocity}
    {
        //
    }

    void setup() override {
        nh_.subscribe(sub_);
    }

    void start() override {
        //
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

public:
    ros::NodeHandle& nh_;
    ros::Subscriber<geometry_msgs::Twist> sub_;
    Velocity& target_velocity_;
    static RosController* controller_;

    static void callback(const geometry_msgs::Twist& msg) {
        controller_->target_velocity_.vx = msg.linear.x;
        controller_->target_velocity_.vy = msg.linear.y;
        controller_->target_velocity_.w = msg.angular.z;
    }
};

ros::NodeHandle nh;

RosController ros_controller{nh, state.target_velocity};
RosController* RosController::controller_ = &ros_controller;

void setup() {
    // Serial.begin(115200);
    // Serial.begin(57600);
    // Serial.setTimeout(0);

    nh.initNode();

    container.add(interrupt_sketch);
    container.add(velocity_controller);
    // container.add(serial_controller);
    // container.add(ps_controller);
    container.add(ros_controller);
    for (int i = 0; i < 4; ++i) {
        container.add(*encoders[i]);
    }
    for (int i = 0; i < 4; ++i) {
        container.add(controllers[i]);
    }
    for (int i = 0; i < 4; ++i) {
        container.add(drivers[i]);
    }

    container.setup();
}

void loop() {
    container.start();

    while (!container.ready()) {
        container.process();
    }

    container.finish();

    // Serial.println();

    nh.spinOnce();
}
