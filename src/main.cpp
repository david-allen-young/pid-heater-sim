#include "Zone.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <thread>
#include <mutex>
#include <random>

int main(int argc, char* argv[])
{
    // Defaults
    double setpoint = 75.0;
    double p = 2.0, i = 0.5, d = 1.0;
    int duration_sec = 10;
    int num_zones = 2;

    // Parse CLI args if provided
    if (argc > 1) setpoint = std::stod(argv[1]);
    if (argc > 2) p = std::stod(argv[2]);
    if (argc > 3) i = std::stod(argv[3]);
    if (argc > 4) d = std::stod(argv[4]);
    if (argc > 5) duration_sec = std::stoi(argv[5]);

    std::cout << "Starting simulation:" << std::endl
        << "  Setpoint: " << setpoint << " Degrees C" << std::endl
        << "  PID: P=" << p << ", I=" << i << ", D=" << d << std::endl
        << "  Duration: " << duration_sec << " sec" << std::endl
        << "  Zones: " << num_zones << std::endl
        << std::endl;

    //PID pid(p, i, d);
    //double global_temp = 20.0;
    //bool running = true;
    //std::mutex mtx;

    //// Random generator setup
    //std::default_random_engine rng(std::random_device{}());
    //std::uniform_real_distribution<double> noise_dist(-0.05, 0.05);
    //
    //// LPF (low pass filter) setup
    //double alpha = 0.2;  // smoothing factor (0 = max smoothing, 1 = no smoothing)
    //double filtered_temp = global_temp;

    //auto readSensors = [&]()
    //{
    //    std::lock_guard<std::mutex> lock(mtx);
    //    double noise = noise_dist(rng);
    //    double raw = global_temp + noise;
    //    filtered_temp = (alpha * raw) + ((1.0 - alpha) * filtered_temp);
    //    return filtered_temp;
    //};

    //auto computeControlAction = [&](double actual, double delta_time)
    //{
    //    return pid.compute(setpoint, actual, delta_time);
    //};

    //auto sendActuatorCommands = [&](double heat)
    //{
    //    double appliedHeat = std::max(0.0, std::min(1.0, heat / 100.0));
    //    {
    //        std::lock_guard<std::mutex> lock(mtx);
    //        global_temp += appliedHeat * 0.5;
    //        const double ambient_temp = 20.0;
    //        double temp_diff = global_temp - ambient_temp;
    //        double passive_cooling = 0.01 + temp_diff * 0.001;
    //        global_temp -= passive_cooling;
    //    }
    //    auto elapsed = std::chrono::steady_clock::now().time_since_epoch();
    //    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed);
    //    int barLength = static_cast<int>((global_temp / setpoint) * 100.0);
    //    int terminalLineWidth = 100;
    //    int paddingForReadout = 20;
    //    barLength = std::min(barLength, terminalLineWidth - paddingForReadout);
    //    std::cout << std::fixed << std::setw(6) << std::setprecision(2);
    //    std::cout << "Temp: " << std::string(barLength, '=') << "> " << global_temp << " [" << ms.count() << " ms]" << std::endl;
    //    int heatBar = static_cast<int>(appliedHeat * 100);
    //    heatBar = std::min(heatBar, terminalLineWidth - paddingForReadout);
    //    std::cout << "Heat: " << std::string(heatBar, '-') << "> " << appliedHeat << " [" << ms.count() << " ms]" << std::endl;
    //    std::cout << std::endl;
    //};

    std::vector<Zone> zones;
    for (int i = 0; i < num_zones; ++i)
    {
        zones.emplace_back(p, i, d, 20.0);
    }

    bool running = true;
    std::condition_variable cv;
    std::mutex tick_mtx;
    bool tick_ready = false;
    auto start_time = std::chrono::steady_clock::now();

    std::thread timer([&]()
    {
        while (running) {
            {
                std::lock_guard<std::mutex> lock(tick_mtx);
                tick_ready = true;
            }
            cv.notify_one();
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
    });

    std::thread controller([&]()
    {
        auto prev = std::chrono::steady_clock::now() - start_time;

        while (running)
        {
            std::unique_lock<std::mutex> lock(tick_mtx);
            cv.wait(lock, [&] { return tick_ready; });
            tick_ready = false;
            lock.unlock();

            //auto local_temp = readSensors();
            auto now = std::chrono::steady_clock::now() - start_time;
            std::chrono::duration<double> elapsed = now - prev;
            prev = now;
            double delta_time = elapsed.count();

            //auto heat = computeControlAction(local_temp, delta_time);
            //sendActuatorCommands(heat);

            for (auto& zone : zones)
            {
                zone.readSensor();
                zone.computeControl(setpoint, delta_time);
            }

            // Apply thermal coupling
            for (int i = 0; i < zones.size(); ++i)
            {
                double neighbor_avg = 0.0;
                int count = 0;
                for (int j = 0; j < zones.size(); ++j)
                {
                    if (i != j)
                    {
                        neighbor_avg += zones[j].local_temp;
                        count++;
                    }
                }
                double coupling = count ? (neighbor_avg / count - zones[i].local_temp) * 0.05 : 0.0;
                zones[i].applyHeat(20.0, coupling);
            }

            // Print each zone
            //auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
            //auto elapsed_sec = std::chrono::duration_cast<std::chrono::duration<double>>(now.time_since_epoch()).count();
            double elapsed_sec = elapsed.count();
            for (int i = 0; i < zones.size(); ++i)
            {
                //int barLen = static_cast<int>((zones[i].local_temp / setpoint) * 100.0);
                //barLen = std::min(barLen, 80);
                int barLen = std::min(static_cast<int>((zones[i].local_temp / setpoint) * 100.0), 80);
                std::cout << std::fixed << std::setprecision(2);
                std::cout << "Zone " << i << " Temp: " << std::string(barLen, '=') << "> " << zones[i].local_temp << " [" << std::fixed << std::setprecision(4) << elapsed_sec << " sec]\n";
                //int heatBar = static_cast<int>(std::clamp(zones[i].applied_heat / 100.0, 0.0, 1.0) * 100);
                double heat_frac = std::max(0.0, std::min(1.0, zones[i].applied_heat / 100.0));
                int heatBar = static_cast<int>(heat_frac * 100);
                std::cout << "Zone " << i << " Heat: " << std::string(heatBar, '-') << "> " << (zones[i].applied_heat / 100.0) << " [" << std::fixed << std::setprecision(4) << elapsed_sec << " sec]\n\n";
            }
        }
     });


    std::this_thread::sleep_for(std::chrono::seconds(duration_sec));
    running = false;
    {
        std::lock_guard<std::mutex> lock(tick_mtx);
        tick_ready = true;
    }
    cv.notify_one();

    timer.join();
    controller.join();

    std::cout << std::endl << "Simulation complete." << std::endl;
    return 0;
}
