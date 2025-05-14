#pragma once

class PID
{
public:
    PID(double p, double i, double d);
    double compute(double setpoint, double actual, double delta_time);

private:
    const double kp, ki, kd;
    double prevError = 0.0;
    double integral = 0.0;
};