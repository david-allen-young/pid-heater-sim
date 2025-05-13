# PID Heater Simulation

A real-time simulation of a PID-controlled heating system written in C++14.

## 🎯 What It Does

This project simulates a basic thermal system using:
- A **PID feedback controller** to regulate temperature toward a setpoint
- **Real-time logic** with timer-driven execution using threads and condition variables
- **Simulated sensor noise** to reflect real-world uncertainty
- A **low-pass filter (LPF)** to smooth noisy sensor input
- **Variable cooling** based on deviation from ambient temperature
- A **live ASCII bar graph** to visualize temperature and heat output in real time

Temperature starts at 20°C and rises toward the user-defined setpoint (default: 75°C) with heat applied each cycle according to PID output and offset by passive cooling.

---

## 🛠 How to Build

### Prerequisites

- CMake 3.10+
- A C++14-compatible compiler (e.g., g++, clang++, or MSVC)

### Build Steps

```bash
git clone https://github.com/yourusername/pid-heater-sim.git
cd pid-heater-sim
mkdir build && cd build
cmake ..
make
./pid_heater_sim
```

---

## ⚙️ Usage

```bash
./pid_heater_sim [setpoint] [p] [i] [d] [duration_sec]
```

### Defaults:
- `setpoint = 75.0`
- `p = 2.0`, `i = 0.5`, `d = 1.0`
- `duration = 10 sec`

### Example:

```bash
./pid_heater_sim 80 1.5 0.3 0.7 15
```

---

## 📈 Sample Output

```
Temp: =============================================================> 46.05 [492885434 ms]
Heat: --------------------------------------------------------------------------------> 0.88 [492885434 ms]

Temp: =========================================================================> 54.81 [492886163 ms]
Heat: --------------------------------------------------------------------------------> 0.80 [492886163 ms]

Temp: ===============================================================================> 59.64 [492886609 ms]
Heat: ----------------------------------------------------------------------------> 0.76 [492886609 ms]

Temp: ================================================================================> 65.54 [492887212 ms]
Heat: ---------------------------------------------------------------------> 0.70 [492887212 ms]
```

---

## 🧠 Learning Highlights

This project is ideal for studying:

- PID control loop design and tuning
- The effect of noisy inputs and sensor filtering
- Real-time programming with threads and condition variables
- Timer-driven simulation architecture
- Console-based data visualization

---

## 🔄 Future Enhancements (Ideas)

- Overheat protection / safety lockout logic
- Dual-zone PID control simulation
- Ramping setpoints and step disturbances
- CSV logging for analysis in Python or Excel

---

## 📄 License

MIT License
