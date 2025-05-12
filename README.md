# PID Heater Simulation

A real-time simulation of a PID-controlled heating system written in C++14.

## 🎯 What It Does

This project simulates a basic thermal system:
- Temperature starts at 20°C
- The system attempts to reach and maintain a target setpoint (default: 75°C)
- PID feedback determines how much "heat" to apply each cycle
- Passive cooling is applied every step

The simulation runs in real-time using a background thread and logs temperature and control output at each cycle.

---

## 🛠 How to Build

### Prerequisites

- CMake 3.10+
- A C++14-compatible compiler (g++, clang++, MSVC)

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
[100234567 ms] Temp: 21.5 | Heat: 0.75
[100234577 ms] Temp: 22.4 | Heat: 0.82
...
```

---

## 🧠 Learning Highlights

- Real-time control loop logic
- PID feedback control theory
- Threading and synchronization
- Command-line argument parsing

---

## 📄 License

MIT License
