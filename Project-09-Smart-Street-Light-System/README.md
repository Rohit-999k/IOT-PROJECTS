# Project 09: Smart Street Light System 🌙💡

<p align="center">
  <img src="https://img.shields.io/badge/Board-Arduino%20UNO-blue?style=flat-square&logo=arduino" />
  <img src="https://img.shields.io/badge/Sensors-LDR%20%2B%20PIR-green?style=flat-square" />
  <img src="https://img.shields.io/badge/Focus-Energy%20Saving-yellow?style=flat-square" />
  <img src="https://img.shields.io/badge/Status-Complete-brightgreen?style=flat-square" />
</p>

## 📋 Project Description

This project implements a **Smart Street Light System** using **Arduino UNO** that combines an **LDR (Light Dependent Resistor)** and a **PIR motion sensor** for intelligent, energy-efficient street lighting. The system operates in two modes:

- **Ambient Light Mode:** The LDR detects ambient light levels. Lights turn ON **only after sunset** (when it's dark) and OFF during daytime.
- **Motion-Adaptive Mode:** When it's dark, the PIR sensor detects pedestrian/vehicle motion. Lights glow at **full brightness during motion** and dim to **30% brightness** when no motion is detected for a set period — saving up to **60% energy**.

This dual-sensor approach mimics real-world smart city street lighting systems.

---

## 🧰 Components Required

| # | Component | Quantity | Specification |
|---|-----------|----------|---------------|
| 1 | Arduino UNO | 1 | ATmega328P |
| 2 | LDR (Photoresistor) | 1 | 5mm, 10kΩ–1MΩ range |
| 3 | PIR Motion Sensor (HC-SR501) | 2 | Passive infrared, 120° detection |
| 4 | LED (White, High Brightness) | 3 | 5mm — Represent street lights |
| 5 | MOSFET (IRF520) or Transistor | 3 | For PWM dimming of LEDs |
| 6 | Resistor (10kΩ) | 1 | Voltage divider for LDR |
| 7 | Resistor (220Ω) | 3 | Current limiting for LEDs |
| 8 | 16×2 LCD Display (I2C) | 1 | Status display |
| 9 | Breadboard | 1 | Full-size |
| 10 | Jumper Wires | ~20 | Assorted |
| 11 | USB Cable (Type B) | 1 | Programming |

---

## 🔌 Circuit Diagram

```
                          +---------------------+
                          |     Arduino UNO     |
                          |                     |
    +--------+            |                     |       +----------+
    | LDR    |            |                     |       | LCD 16x2 |
    |  (one  |            |                     |       | (I2C)    |
    |  end)--+--+-------->| A0            A4 ---|------>| SDA      |
    |        |  |         |               A5 ---|------>| SCL      |
    +--------+  |         |               5V ---|------>| VCC      |
              10kΩ        |              GND ---|------>| GND      |
                |         |                     |       +----------+
               GND        |                     |
    LDR other end -> 5V   |                     |
                          |                     |
    +-----------+         |                     |
    | PIR #1    |         |                     |
    | HC-SR501  |         |                     |
    |  OUT -----+-------->| D2                  |
    |  VCC -----+-------->| 5V                  |
    |  GND -----+-------->| GND                 |
    +-----------+         |                     |
                          |                     |
    +-----------+         |                     |
    | PIR #2    |         |                     |
    |  OUT -----+-------->| D3                  |
    |  VCC -----+-------->| 5V                  |
    |  GND -----+-------->| GND                 |
    +-----------+         |                     |
                          |                     |
    Street Light LEDs (PWM capable pins):       |
    LED 1 ---[220Ω]------>| D9  (PWM)           |
    LED 2 ---[220Ω]------>| D10 (PWM)           |
    LED 3 ---[220Ω]------>| D11 (PWM)           |
    All LED GND --------->| GND                 |
                          +---------------------+

    LDR Voltage Divider:
    5V ---[LDR]---+---[10kΩ]--- GND
                  |
                  +---> A0
    (Dark = High resistance = Low voltage at A0)
    (Light = Low resistance = High voltage at A0)
```

---

## 🔗 Connection Details

### LDR (Light Dependent Resistor) → Arduino

| Connection | Arduino Pin | Description |
|------------|-------------|-------------|
| LDR + 10kΩ voltage divider mid-point | A0 | Analog light level (0–1023) |
| LDR one end | 5V | Power |
| 10kΩ other end | GND | Ground |

> **Reading:** Dark → A0 reads LOW (< 300). Bright → A0 reads HIGH (> 700).

### PIR Motion Sensors (HC-SR501) → Arduino

| PIR Sensor | Arduino Pin | Coverage |
|------------|-------------|----------|
| PIR #1 OUT | D2 | Zone 1 (LED 1 & 2) |
| PIR #2 OUT | D3 | Zone 2 (LED 2 & 3) |
| VCC (both) | 5V | Power |
| GND (both) | GND | Ground |

> **Settings:** Adjust the onboard potentiometers:
> - **Sensitivity:** Set to medium range (~5m)
> - **Time Delay:** Set to minimum (~3 seconds)
> - **Mode Jumper:** Set to **H** (retriggerable mode)

### Street Light LEDs → Arduino (PWM)

| LED | Arduino Pin | Zone |
|-----|-------------|------|
| LED 1 (via 220Ω) | D9 (PWM) | Zone 1 |
| LED 2 (via 220Ω) | D10 (PWM) | Zone 1 & 2 overlap |
| LED 3 (via 220Ω) | D11 (PWM) | Zone 2 |

### LCD (I2C) → Arduino

| LCD Pin | Arduino Pin |
|---------|-------------|
| SDA | A4 |
| SCL | A5 |
| VCC | 5V |
| GND | GND |

---

## 📚 Required Libraries

| Library | Author | Purpose |
|---------|--------|---------|
| `Wire` | Arduino | I2C communication (built-in) |
| `LiquidCrystal_I2C` | Frank de Brabander | LCD display |

> This project uses only built-in Arduino functions — minimal library dependencies!

---

## 💻 Source Code

📄 **File:** [`smart_street_light.ino`](./smart_street_light.ino)

---

## ⚙️ How It Works

1. **Daylight Check:** The LDR continuously measures ambient light. If it's daytime (bright), all lights stay OFF regardless of motion.
2. **Night Mode Activation:** When the LDR detects darkness (analog < threshold), the system enters night mode.
3. **Idle Dimming:** In night mode with no motion, all LEDs glow at **30% brightness** (PWM value: 75/255) — providing basic visibility while saving energy.
4. **Motion Detection:** When a PIR sensor detects motion in its zone:
   - LEDs in that zone ramp up to **100% brightness** (PWM value: 255).
   - A timeout counter starts.
5. **Auto Dim-back:** After no motion for 10 seconds, LEDs gradually dim back to 30%.
6. **Zone-Based:** PIR #1 controls Zone 1 (LED 1+2), PIR #2 controls Zone 2 (LED 2+3). LED 2 is shared.
7. **LCD Display:** Shows current mode (DAY/NIGHT), motion status, and brightness level.

---

## ✅ Advantages

- ⚡ **Energy Efficient:** Up to 60% energy savings by dimming when no motion detected.
- 🌙 **Automatic Day/Night:** No manual switching needed — fully autonomous operation.
- 🚶 **Motion Responsive:** Lights brighten instantly when pedestrians or vehicles approach.
- 🏗️ **Zone-Based:** Multi-zone coverage ensures progressive lighting as a person walks through.
- 💰 **Very Low Cost:** Under ₹400 — one of the cheapest projects in this collection.
- 🌿 **Eco-Friendly:** Reduced energy consumption means lower carbon footprint.
- 🔧 **Simple Design:** Uses basic components, easy to build and maintain.
- 📊 **PWM Dimming:** Smooth brightness transitions using analog PWM, not harsh ON/OFF switching.

---

## 🔮 Future Scope

- 🌐 **IoT Integration:** Add NodeMCU/ESP32 for cloud monitoring of energy usage and fault detection.
- ☀️ **Solar Powered:** Integrate solar panels and battery storage for off-grid deployment.
- 📡 **LoRa Mesh Network:** Connect multiple street light poles via LoRa for centralized smart city management.
- 🎯 **Radar Sensor:** Replace PIR with RCWL-0516 microwave radar for more reliable detection through rain/fog.
- 📊 **Energy Metering:** Add INA219 current sensor to monitor actual power consumption and log data.
- 💡 **Adaptive Brightness:** Adjust dim level based on time of night (dimmer at 2 AM vs 7 PM).
- 🚨 **Fault Detection:** Monitor LED health and report burnt-out lights automatically.
- 🏙️ **Smart City Platform:** Integrate with Google Cloud or AWS IoT for city-wide street light management dashboard.
- 🔋 **Battery Backup:** Add rechargeable batteries for operation during power outages.

---

## 📸 Expected Output

- **Daytime:** All LEDs OFF. LCD shows `Mode: DAY | OFF`.
- **Night (No Motion):** All LEDs at 30%. LCD shows `Mode: NIGHT | DIM`.
- **Night (Motion):** LEDs in active zone at 100%. LCD shows `Mode: NIGHT | BRIGHT`.
- **Serial Monitor:** Continuous LDR values, PIR states, and PWM levels.

---

## 🔙 [Back to Main Page](../README.md)
