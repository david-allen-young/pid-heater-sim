#include "Zone.h"
#include <algorithm>

Zone::Zone(double p, double i, double d, double initial_temp)
    : local_temp(initial_temp),
    filtered_temp(initial_temp),
    applied_heat(0.0),
    pid(p, i, d),
    rng(std::random_device{}()),
    noise_dist(-0.05, 0.05) {
}

double Zone::readSensor(double alpha)
{
    double noise = noise_dist(rng);
    double raw = local_temp + noise;
    filtered_temp = (alpha * raw) + ((1.0 - alpha) * filtered_temp);
    return filtered_temp;
}

void Zone::computeControl(double setpoint, double dt)
{
    applied_heat = pid.compute(setpoint, filtered_temp, dt);
}

void Zone::applyHeat(double ambient, double global_coupling)
{
    auto heat_frac = std::max(0.0, std::min(1.0, applied_heat / 100.0));
    local_temp += heat_frac * 0.5;
    double temp_diff = local_temp - ambient;
    double passive_cooling = 0.01 + temp_diff * 0.001;
    local_temp -= passive_cooling;
    local_temp += global_coupling;
}
