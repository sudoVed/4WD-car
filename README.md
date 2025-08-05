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
9. Chassis Boards, Wires, Screws, and other minor details

---

Features:

* Remote control via local webpage
* PWM speed control
* Independent motor control
* Power regulation using buck converter
* Safe charging with TP4056 modules

---

## Connections

Should be pretty clear from the diagram. Battery is connected in series through a switch (negative of first battery to positive of second battery through switch). It’s as simple as connecting one -ve and one +ve through switch and using the other two terminals for output. But no point in doing all that unless you wanna f around. For simplicity, just use a 2S battery holder with charging capabilities. I used separate TP4056s to charge each battery, which again is way too extra — much simpler and safer to use the 2S battery holder/charger.

Connect output to buck converter and L298N motor driver. Buck converter lowers voltage to 5V for the needy ESP32. L298N needs higher voltage to power up the motors, so we can use a higher voltage (within 12V) to power the entire thing and use buck converter to lower it down for ESP32.

Next up is the L298N connections:
- Let the jumper in the center of the board stay (if you remove it, you need to put 5V input to L298N for logic).
- Remove jumpers from ENA and ENB to enable PWM speed control.
- Connect GPIO pins from ESP32 to ENA, IN1, IN2, IN3, IN4, ENB.
- Connect two motors to OUT1 and OUT2, and two motors to OUT3 and OUT4 (left side motors and right side motors).
- For turning: right side motors slowing down gives you a right turn, and vice versa.
- Make sure the motor sets don’t run in opposite directions.

---

## Things I Wish I Knew To Save Myself From A World Of Pain

- Don’t worry too much about battery power and connections. Before connecting the more fragile components, you can use a multimeter to check voltage and even connectivity.
- Use a multimeter to check connectivity for even slight worries. I was told to make sure ESP32 and L298N share GND and I worried too much about it. Turns out I didn’t need to — my initial plan did share GND between them even though it wasn’t direct. Use multimeter.
- Use Dupont male-female combos for some connections so you can unplug them anytime. I used Dupont connections as makeshift switches so I could cut off the TP4056 circuits when the main power was on. Also used the same combo to facilitate the battery series connection.
- ESP32 is best used with a breadboard. Just plug in the ESP32 into the breadboard (make sure the pin size of breadboard can accommodate the ESP32). Breadboards have the adhesive pack to stick onto surfaces. Without this, it's kinda rough to mount the ESP32 onto your project.
- Don’t throw away the jumpers from L298N — you never know when you might wanna put 'em back on.
- Download ESP32 libraries for Arduino IDE. Arduino Microcontrollers are good but ESP32 also has WiFi connectivity which is very useful.
- Most ESP32 dev boards can handle two power sources (like from a battery via buck converter and USB for coding), so there's usually no need to worry too much about that. Obviously, it's better to disconnect the external power when you're only coding or uploading to the ESP32. But during test runs, you <i>do</i> need to power the motors and monitor the ESP32 output simultaneously—so the ESP32 ends up getting powered from both sources.


