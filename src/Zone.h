#pragma once
#include "PID.h"
#include <random>

class Zone {
public:
    Zone(double p, double i, double d, double initial_temp);

    double readSensor(double alpha = 0.2);
    void computeControl(double setpoint, double dt);
    void applyHeat(double ambient, double global_coupling);

    double local_temp;
    double filtered_temp;
    double applied_heat;

private:
    PID pid;
    std::default_random_engine rng;
    std::uniform_real_distribution<double> noise_dist;
};
