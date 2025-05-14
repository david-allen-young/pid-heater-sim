# PID Heater Simulation

A real-time simulation of a PID-controlled heating system written in C++14.

## 🎯 What It Does

This project simulates a thermal control system using:
- **Multiple PID controllers**—each managing a separate heating zone
- **Real-time logic** using timer-driven threads and condition variables
- **Simulated sensor noise** and low-pass filtering
- **Passive cooling** based on deviation from ambient temperature
- **Inter-zone thermal coupling** to mimic heat diffusion
- **ASCII-based live bar graphs** to show temperature and heat output
- **Time display in seconds** with fixed precision

Each zone begins at 20°C and attempts to reach the user-defined setpoint (default: 75°C). Heating is driven by PID output and offset by cooling and neighbor-zone interaction.

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
Zone 0 Temp: ==============================> 33.40 [4.7812 sec]
Zone 0 Heat: ------------------------------> 0.7432 [4.7812 sec]

Zone 1 Temp: ==============================> 34.01 [4.7812 sec]
Zone 1 Heat: ------------------------------> 1.0000 [4.7812 sec]
```

*Bars are clamped to fit typical terminal widths. Heat is capped at 100%.*

---

## 🧠 Learning Highlights

This project is ideal for studying:

- PID control loop design and tuning
- Multi-zone thermal modeling and coupling
- Effects of noisy inputs and filtering
- Real-time programming with threads and timers
- Console-based simulation visualization

---

## 🔄 Future Enhancements (Ideas)

- Setpoint ramping and step disturbances
- CSV logging for offline analysis
- Support for thermal mass and actuator lag
- Interactive CLI commands during simulation
- Real-time plot visualization via Python/matplotlib

---

## 📄 License

MIT License
