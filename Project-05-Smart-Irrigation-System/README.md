# Project 05: Smart Irrigation System 🌱💧

<p align="center">
  <img src="https://img.shields.io/badge/Board-NodeMCU%20ESP8266-blue?style=flat-square&logo=arduino" />
  <img src="https://img.shields.io/badge/Sensor-Soil%20Moisture-green?style=flat-square" />
  <img src="https://img.shields.io/badge/Cloud-ThingSpeak-orange?style=flat-square" />
  <img src="https://img.shields.io/badge/Status-Complete-brightgreen?style=flat-square" />
</p>

## 📋 Project Description

This project implements an **IoT-based Smart Irrigation System** that automatically waters plants based on real-time **soil moisture levels**. The system uses a **capacitive soil moisture sensor** to measure soil water content, and a **5V relay-driven water pump** to irrigate when moisture drops below a set threshold. The **NodeMCU ESP8266** connects to Wi-Fi to send soil moisture data to **ThingSpeak** and the **Blynk app** for remote monitoring and manual override control. A **DHT11 sensor** provides ambient temperature and humidity data to help optimize watering schedules.

---

## 🧰 Components Required

| # | Component | Quantity | Specification |
|---|-----------|----------|---------------|
| 1 | NodeMCU ESP8266 (ESP-12E) | 1 | Wi-Fi microcontroller |
| 2 | Capacitive Soil Moisture Sensor v1.2 | 1 | Analog output, corrosion-resistant |
| 3 | DHT11 Sensor | 1 | Temperature & humidity |
| 4 | Single-Channel Relay Module | 1 | 5V, controls water pump |
| 5 | Mini Submersible Water Pump | 1 | 3–5V DC motor pump |
| 6 | Silicone Tubing | 1m | For water delivery |
| 7 | 16×2 LCD Display (I2C) | 1 | HD44780 + I2C backpack |
| 8 | LED (Green) | 1 | Pump OFF indicator |
| 9 | LED (Blue) | 1 | Pump ON indicator |
| 10 | Resistors (220Ω) | 2 | For LEDs |
| 11 | Resistor (10kΩ) | 1 | Pull-up for DHT11 |
| 12 | Breadboard | 1 | Full-size |
| 13 | Jumper Wires | ~20 | Assorted |
| 14 | External 5V Power Supply | 1 | For relay and pump |

---

## 🔌 Circuit Diagram

```
                          +---------------------+
                          |    NodeMCU ESP8266   |
                          |                     |
    +-------------+       |                     |      +----------+
    | Soil        |       |                     |      | LCD 16x2 |
    | Moisture    |       |                     |      | (I2C)    |
    | Sensor      |       |                     |      |          |
    |  AO --------+------>| A0 (ADC)      D1 ---|----->| SCL      |
    |  VCC -------+------>| 3V3           D2 ---|----->| SDA      |
    |  GND -------+------>| GND          VIN ---|----->| VCC(5V)  |
    +-------------+       |              GND ---|----->| GND      |
                          |                     |      +----------+
    +--------+            |                     |
    | DHT11  |            |                     |      +----------+
    |  DATA--+--[10kΩ]--->| D4 (GPIO2)          |      | Relay    |
    |  VCC --+----------->| 3V3                 |      | Module   |
    |  GND --+----------->| GND           D5 ---|----->| IN       |
    +--------+            |              VIN ---|----->| VCC(5V)  |
                          |              GND ---|----->| GND      |
    Green LED --[220Ω]--->| D6 (GPIO12)         |      |          |
    Blue LED ---[220Ω]--->| D7 (GPIO13)         |      | NO ---+  |
                          +---------------------+      | COM---+  |
                                                       +-------+  |
                                                                   |
                                                       +---------+ |
                                                       | Water   | |
                                                       | Pump    |<+
                                                       | 3-5V DC |
                                                       +---------+
```

---

## 🔗 Connection Details

### Capacitive Soil Moisture Sensor → NodeMCU

| Sensor Pin | NodeMCU Pin | Description |
|------------|-------------|-------------|
| AO (Analog) | A0 | Soil moisture analog value (0–1023) |
| VCC | 3V3 | Power supply |
| GND | GND | Ground |

> **Calibration:** Dry soil ≈ 800–1023, Wet soil ≈ 200–400. Calibrate for your specific soil type.

### DHT11 Sensor → NodeMCU

| DHT11 Pin | NodeMCU Pin | Description |
|-----------|-------------|-------------|
| VCC | 3V3 | Power |
| DATA | D4 (GPIO2) | Data (with 10kΩ pull-up) |
| GND | GND | Ground |

### Relay Module → NodeMCU & Pump

| Relay Pin | Connection | Description |
|-----------|------------|-------------|
| IN | D5 (GPIO14) | Control signal from NodeMCU |
| VCC | VIN (5V) | Relay power |
| GND | GND | Ground |
| NO (Normally Open) | Water Pump (+) | Pump positive terminal |
| COM (Common) | External 5V (+) | External power supply |

> **Pump (−)** connects to External power supply GND.

### LCD Display (I2C) → NodeMCU

| LCD Pin | NodeMCU Pin | Description |
|---------|-------------|-------------|
| VCC | VIN (5V) | Power |
| GND | GND | Ground |
| SCL | D1 (GPIO5) | I2C Clock |
| SDA | D2 (GPIO4) | I2C Data |

### LEDs → NodeMCU

| LED | NodeMCU Pin | Meaning |
|-----|-------------|---------|
| Green (via 220Ω) | D6 (GPIO12) | Soil moist — Pump OFF |
| Blue (via 220Ω) | D7 (GPIO13) | Soil dry — Pump ON |

---

## 📚 Required Libraries

| Library | Author | Purpose |
|---------|--------|---------|
| `ESP8266WiFi` | ESP8266 Community | Wi-Fi (built-in) |
| `ThingSpeak` | MathWorks | Cloud data upload |
| `BlynkSimpleEsp8266` | Blynk Inc. | Remote app control |
| `DHT sensor library` | Adafruit | DHT11 readings |
| `LiquidCrystal_I2C` | Frank de Brabander | LCD display |

---

## 💻 Source Code

📄 **File:** [`smart_irrigation_system.ino`](./smart_irrigation_system.ino)

---

## ⚙️ How It Works

1. **Sensor Reading:** The soil moisture sensor outputs an analog value (0–1023) where lower values indicate more moisture.
2. **Moisture Percentage:** The raw value is mapped to a percentage: `moisturePercent = map(rawValue, wetValue, dryValue, 100, 0)`.
3. **Automatic Pump Control:**
   - Moisture < 30% → **Pump ON** (Blue LED ON, relay activated)
   - Moisture > 60% → **Pump OFF** (Green LED ON, relay deactivated)
   - Hysteresis between 30%–60% prevents rapid switching.
4. **LCD Display:** Shows moisture %, temperature, humidity, and pump status.
5. **Cloud Upload:** Moisture %, temperature, and pump state sent to ThingSpeak.
6. **Blynk Override:** Virtual button V5 in Blynk app allows manual pump ON/OFF control.

---

## ✅ Advantages

- 🌿 **Water Conservation:** Irrigates only when needed, saving up to **40-60%** water compared to manual watering.
- 🤖 **Fully Automatic:** No human intervention required for day-to-day operation.
- 📱 **Remote Monitoring:** Check soil conditions and control pump from anywhere via Blynk.
- 📊 **Data-Driven:** Historical moisture data on ThingSpeak helps optimize watering patterns.
- 🌾 **Crop Protection:** Prevents both overwatering (root rot) and underwatering (wilting).
- 🔧 **Capacitive Sensor:** Unlike resistive sensors, capacitive sensors resist corrosion for longer lifespan.
- 💰 **Affordable:** Total cost under ₹700 — a fraction of commercial irrigation systems.

---

## 🔮 Future Scope

- 🌤️ **Weather API Integration:** Fetch local weather forecasts and skip irrigation before rain.
- ☀️ **Solar Powered:** Add a solar panel for fully autonomous, off-grid operation in farms.
- 📡 **LoRa Connectivity:** Use LoRa modules for long-range communication in large agricultural fields.
- 🌡️ **Multi-Zone Irrigation:** Deploy multiple sensors for zone-based watering with individual control.
- 🤖 **Machine Learning:** Train ML models on soil, weather, and crop data for precision agriculture.
- 💧 **Flow Sensor:** Add a YF-S201 water flow sensor to measure exact water usage.
- 📅 **Scheduling:** Implement time-based scheduling alongside sensor-based automation.
- 🏭 **Commercial Scale:** Scale to greenhouse or farm-level deployment with multiple pumps and valves.
- 🧪 **NPK Sensor:** Add NPK soil nutrient sensors for complete soil health monitoring.

---

## 📸 Expected Output

- **LCD:** `Moisture: 45%` on line 1, `Pump: OFF | 28°C` on line 2.
- **Serial Monitor:** Detailed readings with timestamps.
- **ThingSpeak:** Live charts for moisture trend and pump activation history.
- **Blynk App:** Gauge widget showing moisture level and button for manual pump control.

---

## 🔙 [Back to Main Page](../README.md)
