#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>
#include <mutex>
#include <random>

class PID
{
public:
    PID(double p, double i, double d)
        : kp(p), ki(i), kd(d)
    {
    }
    double compute(double setpoint, double actual, double delta_time)
    {
        double error = setpoint - actual;
        integral += error * delta_time;
        double derivative = (error - prevError) / delta_time;
        prevError = error;
        return kp * error + ki * integral + kd * derivative;
    }

private:
    const double kp, ki, kd;
    double prevError = 0.0;
    double integral = 0.0;
};

int main(int argc, char* argv[])
{
    // Defaults
    double setpoint = 75.0;
    double p = 2.0, i = 0.5, d = 1.0;
    int duration_sec = 10;

    // Parse CLI args if provided
    if (argc > 1) setpoint = std::stod(argv[1]);
    if (argc > 2) p = std::stod(argv[2]);
    if (argc > 3) i = std::stod(argv[3]);
    if (argc > 4) d = std::stod(argv[4]);
    if (argc > 5) duration_sec = std::stoi(argv[5]);

    std::cout << "Starting simulation:\n"
        << "  Setpoint: " << setpoint << " °C\n"
        << "  PID: P=" << p << ", I=" << i << ", D=" << d << "\n"
        << "  Duration: " << duration_sec << " sec\n\n";

    PID pid(p, i, d);
    double global_temp = 20.0;
    const double passive_cooling = 0.05;
    bool running = true;
    std::mutex mtx;

    // Random generator setup
    std::default_random_engine rng(std::random_device{}());
    std::uniform_real_distribution<double> noise_dist(-0.05, 0.05);
    
    // LPF (low pass filter) setup
    double alpha = 0.2;  // smoothing factor (0 = max smoothing, 1 = no smoothing)
    double filtered_temp = global_temp;

    auto readSensors = [&]()
    {
        std::lock_guard<std::mutex> lock(mtx);
        double noise = noise_dist(rng);
        double raw = global_temp + noise;
        filtered_temp = (alpha * raw) + ((1.0 - alpha) * filtered_temp);
        return filtered_temp;
    };

    auto computeControlAction = [&](double actual, double delta_time)
    {
        return pid.compute(setpoint, actual, delta_time);
    };

    auto sendActuatorCommands = [&](double heat)
    {
        double appliedHeat = std::max(0.0, std::min(1.0, heat / 100.0));
        {
            std::lock_guard<std::mutex> lock(mtx);
            global_temp += appliedHeat * 0.5;
            global_temp -= passive_cooling;
        }
        auto elapsed = std::chrono::steady_clock::now().time_since_epoch();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed);
        std::cout << "[" << ms.count() << " ms] Temp: " << global_temp << " | Heat: " << appliedHeat << "\n";
    };

    auto waitUntilNextCycle = [](std::chrono::steady_clock::time_point now)
    {
        auto then = now + std::chrono::microseconds(10);
        std::this_thread::sleep_until(then);
    };

    auto prev = std::chrono::steady_clock::now();
    std::thread T1([&]()
    {
        while (running)
        {
            auto local_temp = readSensors();
            auto now = std::chrono::steady_clock::now();
            std::chrono::duration<double> elapsed = now - prev;
            prev = now;
            double delta_time = elapsed.count();
            auto heat = computeControlAction(local_temp, delta_time);
            sendActuatorCommands(heat);
            waitUntilNextCycle(now);
        }
    });

    std::this_thread::sleep_for(std::chrono::seconds(duration_sec));
    running = false;
    T1.join();

    std::cout << std::endl << "Simulation complete." << std::endl;
    return 0;
}
