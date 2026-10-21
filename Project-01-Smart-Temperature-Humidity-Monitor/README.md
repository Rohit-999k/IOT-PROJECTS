# Project 01: Smart Temperature & Humidity Monitor 🌡️💧

<p align="center">
  <img src="https://img.shields.io/badge/Board-NodeMCU%20ESP8266-blue?style=flat-square&logo=arduino" />
  <img src="https://img.shields.io/badge/Sensor-DHT11-green?style=flat-square" />
  <img src="https://img.shields.io/badge/Cloud-ThingSpeak-orange?style=flat-square" />
  <img src="https://img.shields.io/badge/Status-Complete-brightgreen?style=flat-square" />
</p>

## 📋 Project Description

This project implements a **real-time temperature and humidity monitoring system** using the **DHT11 sensor** and **NodeMCU ESP8266**. The sensor data is transmitted over **Wi-Fi** to the **ThingSpeak cloud platform**, where it is visualized as live graphs. An **OLED display (SSD1306)** provides local real-time readings. The system also triggers a **buzzer alert** when temperature crosses a predefined threshold.

This project demonstrates core IoT concepts: **sensor interfacing**, **Wi-Fi connectivity**, **cloud data logging**, and **real-time monitoring**.

---

## 🧰 Components Required

| # | Component | Quantity | Specification |
|---|-----------|----------|---------------|
| 1 | NodeMCU ESP8266 (ESP-12E) | 1 | Wi-Fi enabled microcontroller |
| 2 | DHT11 Sensor | 1 | Temperature & Humidity sensor |
| 3 | 0.96" OLED Display | 1 | SSD1306, 128×64, I2C interface |
| 4 | Buzzer | 1 | 5V Active Buzzer |
| 5 | LED (Red) | 1 | 5mm indicator LED |
| 6 | Resistor | 1 | 10kΩ (pull-up for DHT11) |
| 7 | Resistor | 1 | 220Ω (for LED) |
| 8 | Breadboard | 1 | Full-size or half-size |
| 9 | Jumper Wires | ~15 | Male-to-Male, Male-to-Female |
| 10 | Micro USB Cable | 1 | For programming & power |

---

## 🔌 Circuit Diagram

```
                          +---------------------+
                          |    NodeMCU ESP8266   |
                          |                     |
    +--------+            |                     |           +----------+
    | DHT11  |            |                     |           |  OLED    |
    |        |---Data---->| D4 (GPIO2)          |           | SSD1306  |
    |  VCC---+----------->| 3V3                 |           |          |
    |  GND---+----------->| GND            D1 --|---------->| SCL      |
    +--------+   |        |                D2 --|---------->| SDA      |
              10kΩ        |                3V3 -|---------->| VCC      |
              pull-up     |                GND -|---------->| GND      |
                          |                     |           +----------+
                          |                     |
    +--------+            |                     |
    | Buzzer |            |                     |
    |  (+)---+----------->| D5 (GPIO14)         |
    |  (-)---+----------->| GND                 |
    +--------+            |                     |
                          |                     |
    +--------+            |                     |
    | LED    |            |                     |
    |  (+)---+---[220Ω]-->| D6 (GPIO12)         |
    |  (-)---+----------->| GND                 |
    +--------+            +---------------------+
```

---

## 🔗 Connection Details

### DHT11 Sensor → NodeMCU

| DHT11 Pin | NodeMCU Pin | Description |
|-----------|-------------|-------------|
| VCC | 3V3 | Power supply (3.3V) |
| DATA | D4 (GPIO2) | Digital data output |
| GND | GND | Ground |

> **Note:** Connect a **10kΩ pull-up resistor** between the DATA pin and VCC.

### OLED Display (SSD1306, I2C) → NodeMCU

| OLED Pin | NodeMCU Pin | Description |
|----------|-------------|-------------|
| VCC | 3V3 | Power supply |
| GND | GND | Ground |
| SCL | D1 (GPIO5) | I2C Clock |
| SDA | D2 (GPIO4) | I2C Data |

### Buzzer → NodeMCU

| Buzzer Pin | NodeMCU Pin | Description |
|------------|-------------|-------------|
| (+) Positive | D5 (GPIO14) | Control signal |
| (−) Negative | GND | Ground |

### LED → NodeMCU

| LED Pin | NodeMCU Pin | Description |
|---------|-------------|-------------|
| Anode (+) | D6 (GPIO12) via 220Ω resistor | Control signal |
| Cathode (−) | GND | Ground |

---

## 📚 Required Libraries

Install these libraries from **Arduino IDE → Sketch → Include Library → Manage Libraries**:

| Library | Author | Purpose |
|---------|--------|---------|
| `DHT sensor library` | Adafruit | Reading DHT11 sensor data |
| `Adafruit Unified Sensor` | Adafruit | Dependency for DHT library |
| `Adafruit SSD1306` | Adafruit | OLED display driver |
| `Adafruit GFX Library` | Adafruit | Graphics primitives for OLED |
| `ESP8266WiFi` | ESP8266 Community | Wi-Fi connectivity (built-in) |
| `ThingSpeak` | MathWorks | ThingSpeak cloud communication |

---

## 💻 Source Code

📄 **File:** [`smart_temp_humidity_monitor.ino`](./smart_temp_humidity_monitor.ino)

---

## ⚙️ How It Works

1. **Initialization:** NodeMCU connects to the configured Wi-Fi network and initializes the DHT11 sensor, OLED display, buzzer, and LED.
2. **Data Acquisition:** The DHT11 sensor reads the ambient temperature (°C) and relative humidity (%) every 15 seconds.
3. **Local Display:** The readings are displayed on the OLED screen in real time with clear labels.
4. **Cloud Upload:** Sensor data is sent to the ThingSpeak cloud platform via HTTP API, where it is logged and visualized as live charts.
5. **Alert System:** If the temperature exceeds the defined threshold (e.g., 40°C), the buzzer sounds and the red LED turns on as a warning.
6. **Continuous Loop:** The process repeats, providing uninterrupted monitoring.

---

## 📊 ThingSpeak Setup

1. Create a free account on [ThingSpeak](https://thingspeak.com/).
2. Create a **New Channel** with two fields:
   - **Field 1:** Temperature (°C)
   - **Field 2:** Humidity (%)
3. Copy the **Channel ID** and **Write API Key**.
4. Paste them into the code in the designated variables.

---

## ✅ Advantages

- 📡 **Remote Monitoring:** Access real-time data from anywhere via ThingSpeak dashboard.
- 💰 **Low Cost:** Uses affordable, widely available components (total cost < ₹500).
- ⚡ **Low Power Consumption:** DHT11 and NodeMCU consume minimal power, suitable for battery operation.
- 📱 **Mobile Accessible:** ThingSpeak provides a mobile-friendly interface for data visualization.
- 🔔 **Instant Alerts:** Buzzer and LED provide immediate local alerts for threshold breaches.
- 📈 **Data Logging:** Historical data is stored on ThingSpeak for trend analysis.
- 🔧 **Easy to Build:** Simple circuit with minimal wiring — ideal for beginners.

---

## 🔮 Future Scope

- 📲 **Mobile App Integration:** Develop a dedicated Android/iOS app using Blynk or MIT App Inventor for push notifications.
- 🔄 **Upgrade to DHT22:** Replace DHT11 with DHT22 for higher accuracy (±0.5°C vs ±2°C) and wider range.
- 📧 **Email/SMS Alerts:** Integrate IFTTT (If This Then That) webhooks for email or SMS notifications.
- 🏭 **Industrial Deployment:** Scale up for warehouse, server room, or greenhouse monitoring.
- 🤖 **Machine Learning:** Apply ML algorithms on historical data to predict temperature trends and anomalies.
- 🔋 **Solar Powered:** Add a solar panel and battery for fully autonomous outdoor deployment.
- 🌐 **Multi-Node Network:** Deploy multiple sensor nodes to monitor different zones and aggregate data on a central dashboard.
- 📊 **Grafana Dashboard:** Replace ThingSpeak with a self-hosted Grafana + InfluxDB stack for advanced visualization.

---

## 📸 Expected Output

- **Serial Monitor:** Displays temperature and humidity readings every 15 seconds.
- **OLED Display:** Shows real-time temperature and humidity with icons.
- **ThingSpeak:** Live line charts updating with each data point.
- **Buzzer/LED:** Activates when temperature > 40°C.

---

## 🔙 [Back to Main Page](../README.md)
