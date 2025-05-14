#include "PID.h"

PID::PID(double p, double i, double d)
    : kp(p), ki(i), kd(d)
{
}
double PID::compute(double setpoint, double actual, double delta_time)
{
    double error = setpoint - actual;
    integral += error * delta_time;
    //double derivative = (error - prevError) / delta_time;
    double derivative = 0.0;
    if (delta_time > 1e-6)
    {
        derivative = (error - prevError) / delta_time;
    }
    prevError = error;
    return kp * error + ki * integral + kd * derivative;
}