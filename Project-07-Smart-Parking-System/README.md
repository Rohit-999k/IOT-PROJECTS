# Project 07: Smart Parking System 🅿️🚗

<p align="center">
  <img src="https://img.shields.io/badge/Board-Arduino%20UNO-blue?style=flat-square&logo=arduino" />
  <img src="https://img.shields.io/badge/Sensor-HC--SR04%20Ultrasonic-green?style=flat-square" />
  <img src="https://img.shields.io/badge/Actuator-Servo%20Motor-orange?style=flat-square" />
  <img src="https://img.shields.io/badge/Status-Complete-brightgreen?style=flat-square" />
</p>

## 📋 Project Description

This project implements a **Smart Parking System** using **Arduino UNO**, **HC-SR04 ultrasonic sensors**, and a **servo motor** for automated gate control. The system monitors **3 parking slots** using individual ultrasonic sensors to detect vehicle presence. An **entry/exit sensor** detects approaching vehicles and automatically opens the servo gate. The parking slot availability is displayed on a **16×2 LCD** and indicated by **Red/Green LEDs** for each slot. When all slots are full, the gate remains closed and a "FULL" message is displayed.

---

## 🧰 Components Required

| # | Component | Quantity | Specification |
|---|-----------|----------|---------------|
| 1 | Arduino UNO | 1 | ATmega328P |
| 2 | HC-SR04 Ultrasonic Sensor | 4 | 3 for slots + 1 for entry detection |
| 3 | Servo Motor (SG90) | 1 | For gate barrier control |
| 4 | 16×2 LCD Display (I2C) | 1 | HD44780 + I2C backpack |
| 5 | LED (Green) | 3 | 5mm — Slot empty indicator |
| 6 | LED (Red) | 3 | 5mm — Slot occupied indicator |
| 7 | Resistors (220Ω) | 6 | For LEDs |
| 8 | Breadboard | 1 | Full-size |
| 9 | Jumper Wires | ~35 | Assorted |
| 10 | USB Cable (Type B) | 1 | Programming |

---

## 🔌 Circuit Diagram

```
                          +---------------------+
                          |     Arduino UNO     |
                          |                     |
    Entry Sensor          |                     |       +----------+
    +----------+          |                     |       | LCD 16x2 |
    | HC-SR04  |          |                     |       | (I2C)    |
    | #0       |          |                     |       |          |
    | TRIG ----+--------->| D2            A4 ---|------>| SDA      |
    | ECHO ----+--------->| D3            A5 ---|------>| SCL      |
    | VCC -----+--------->| 5V            5V ---|------>| VCC      |
    | GND -----+--------->| GND          GND ---|------>| GND      |
    +----------+          |                     |       +----------+
                          |                     |
    Slot 1 Sensor         |                     |       +----------+
    +----------+          |                     |       | Servo    |
    | HC-SR04  |          |                     |       | SG90     |
    | #1       |          |                     |       |          |
    | TRIG ----+--------->| D4           D13 ---|------>| Signal   |
    | ECHO ----+--------->| D5            5V ---|------>| VCC      |
    +----------+          |              GND ---|------>| GND      |
                          |                     |       +----------+
    Slot 2 Sensor         |                     |
    +----------+          |                     |
    | HC-SR04  |          |                     |
    | #2       |          |                     |
    | TRIG ----+--------->| D6                  |
    | ECHO ----+--------->| D7                  |
    +----------+          |                     |
                          |                     |
    Slot 3 Sensor         |                     |
    +----------+          |                     |     Slot LEDs:
    | HC-SR04  |          |                     |     Green = Empty
    | #3       |          |                     |     Red   = Occupied
    | TRIG ----+--------->| D8                  |
    | ECHO ----+--------->| D9                  |
    +----------+          |                     |
                          |                     |
    Slot 1: Green [220Ω]->| D10                 |
    Slot 1: Red   [220Ω]->| D11                 |
    Slot 2: Green [220Ω]->| D12                 |
    Slot 2: Red   [220Ω]->| A0 (as digital)     |
    Slot 3: Green [220Ω]->| A1 (as digital)     |
    Slot 3: Red   [220Ω]->| A2 (as digital)     |
                          +---------------------+
    All HC-SR04 VCC -> 5V, GND -> GND (shared)
    All LED cathodes -> GND
```

---

## 🔗 Connection Details

### Entry Ultrasonic Sensor (HC-SR04 #0) → Arduino

| Pin | Arduino Pin | Description |
|-----|-------------|-------------|
| TRIG | D2 | Trigger pulse output |
| ECHO | D3 | Echo pulse input |
| VCC | 5V | Power |
| GND | GND | Ground |

### Slot Sensors (HC-SR04 #1, #2, #3) → Arduino

| Sensor | TRIG Pin | ECHO Pin | Monitors |
|--------|----------|----------|----------|
| Slot 1 | D4 | D5 | Parking Slot 1 |
| Slot 2 | D6 | D7 | Parking Slot 2 |
| Slot 3 | D8 | D9 | Parking Slot 3 |

> **Detection:** Distance < 10 cm → Vehicle present (slot occupied).

### Servo Motor (SG90) → Arduino

| Servo Pin | Arduino Pin | Description |
|-----------|-------------|-------------|
| Signal (Orange) | D13 | PWM control signal |
| VCC (Red) | 5V | Power |
| GND (Brown) | GND | Ground |

> **Gate Action:** 0° = Gate closed, 90° = Gate open.

### LED Indicators → Arduino

| Slot | Green LED (Empty) | Red LED (Occupied) |
|------|-------------------|--------------------|
| Slot 1 | D10 | D11 |
| Slot 2 | D12 | A0 (digital mode) |
| Slot 3 | A1 (digital mode) | A2 (digital mode) |

---

## 📚 Required Libraries

| Library | Author | Purpose |
|---------|--------|---------|
| `Servo` | Arduino | Servo motor control (built-in) |
| `Wire` | Arduino | I2C communication (built-in) |
| `LiquidCrystal_I2C` | Frank de Brabander | LCD display |
| `NewPing` | Tim Eckel | Simplified ultrasonic sensor handling |

---

## 💻 Source Code

📄 **File:** [`smart_parking_system.ino`](./smart_parking_system.ino)

---

## ⚙️ How It Works

1. **Slot Monitoring:** Three HC-SR04 sensors continuously measure distance in each parking slot. If distance < 10 cm, a vehicle is present (occupied).
2. **Availability Count:** The system counts empty slots and displays `Available: X/3` on the LCD.
3. **Entry Detection:** The 4th HC-SR04 at the entry detects approaching vehicles (distance < 30 cm).
4. **Gate Control:**
   - If slots are available AND vehicle approaches → **Servo opens gate (90°)** → Waits 5 seconds → **Gate closes (0°)**.
   - If all slots full → Gate stays closed. LCD shows `PARKING FULL`.
5. **LED Indicators:** Each slot has a Green LED (empty) and Red LED (occupied) for visual guidance.

---

## ✅ Advantages

- 🅿️ **Real-Time Monitoring:** Instant parking slot availability display.
- 🚧 **Automated Gate:** No manual gate operation needed — reduces manpower.
- 💡 **Visual Guidance:** LED indicators help drivers quickly identify empty slots.
- 🏗️ **Scalable:** Easily add more ultrasonic sensors for larger parking lots.
- ⏱️ **Fast Detection:** Ultrasonic sensors respond in microseconds.
- 💰 **Low Cost:** Entire system costs under ₹800.
- 🔧 **Easy Maintenance:** All components are modular and replaceable.

---

## 🔮 Future Scope

- 🌐 **IoT Dashboard:** Add ESP8266 for Wi-Fi connectivity and a web dashboard showing real-time slot availability.
- 📱 **Mobile App:** Develop an app that shows available slots and allows slot reservation.
- 💳 **Payment Integration:** Add RFID-based parking payment system for commercial use.
- 📷 **ANPR Camera:** Integrate Automatic Number Plate Recognition for vehicle tracking and security.
- 📊 **Analytics:** Log entry/exit times for parking duration analysis and revenue optimization.
- 🏙️ **Smart City Integration:** Connect to city-wide smart parking networks.
- 🗺️ **Navigation:** Guide drivers to the nearest empty slot using LED strips along the drive lane.
- 🔋 **Solar Powered:** Use solar panels for outdoor parking lots.
- 🤖 **Computer Vision:** Replace ultrasonic sensors with camera + ML for more reliable vehicle detection.

---

## 📸 Expected Output

- **LCD Line 1:** `Available: 2/3` (or `PARKING FULL`)
- **LCD Line 2:** `Gate: Open` / `Gate: Closed`
- **LEDs:** Green/Red per slot showing occupancy.
- **Servo:** Gate arm opens when vehicle approaches and slots available.

---

## 🔙 [Back to Main Page](../README.md)
