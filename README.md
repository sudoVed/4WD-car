# 4WD-car

A 4WD car built using an ESP32 dev board and controlled via a webpage hosted on ESP32’s Wi-Fi network.

---

**Components Used**:

1. ESP32 (38-pin Dev Board)
2. LM2596S – DC-DC Buck Converter
3. L298N – Dual H-Bridge Motor Driver
4. 2× 18650 Li-ion Batteries
5. 2× TP4056 – 1A Li-ion Charging Modules
6. Breadboard
7. 4× DC Motors
8. Power Switch

---

**Notes**:

Yeah, I _could’ve_ gone for a fancier charging system. But where’s the fun in that? I wanted to experiment with raw components and learn the quirks firsthand.


---

Features:

* Remote control via local webpage
* PWM speed control
* Independent motor control
* Power regulation using buck converter
* Safe charging with TP4056 modules

