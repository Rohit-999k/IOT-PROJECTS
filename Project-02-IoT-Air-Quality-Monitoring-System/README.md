# Project 02: IoT Air Quality Monitoring System 🌬️🏭

<p align="center">
  <img src="https://img.shields.io/badge/Board-NodeMCU%20ESP8266-blue?style=flat-square&logo=arduino" />
  <img src="https://img.shields.io/badge/Sensor-MQ--135-green?style=flat-square" />
  <img src="https://img.shields.io/badge/Cloud-ThingSpeak-orange?style=flat-square" />
  <img src="https://img.shields.io/badge/Status-Complete-brightgreen?style=flat-square" />
</p>

## 📋 Project Description

This project builds an **IoT-based Air Quality Monitoring System** using the **MQ-135 gas sensor** and **NodeMCU ESP8266**. The MQ-135 sensor detects harmful gases such as **NH3 (ammonia), NOx (nitrogen oxides), CO2 (carbon dioxide), benzene, and smoke** present in the atmosphere. The Air Quality Index (AQI) value is displayed on a **16×2 LCD display (I2C)** and simultaneously uploaded to **ThingSpeak** for remote cloud-based monitoring.

The system classifies air quality into categories (**Good, Moderate, Unhealthy, Hazardous**) and triggers visual (LED) and audible (buzzer) alerts when pollution levels are dangerous.

---

## 🧰 Components Required

| # | Component | Quantity | Specification |
|---|-----------|----------|---------------|
| 1 | NodeMCU ESP8266 (ESP-12E) | 1 | Wi-Fi enabled microcontroller |
| 2 | MQ-135 Gas Sensor Module | 1 | Analog output, detects multiple gases |
| 3 | 16×2 LCD Display (I2C) | 1 | HD44780 with PCF8574 I2C backpack |
| 4 | LED (Green) | 1 | 5mm — Good air quality indicator |
| 5 | LED (Yellow) | 1 | 5mm — Moderate quality indicator |
| 6 | LED (Red) | 1 | 5mm — Poor/Hazardous indicator |
| 7 | Buzzer | 1 | 5V Active Buzzer |
| 8 | Resistor (220Ω) | 3 | For LEDs |
| 9 | Breadboard | 1 | Full-size |
| 10 | Jumper Wires | ~20 | Male-to-Male, Male-to-Female |
| 11 | Micro USB Cable | 1 | For programming & power |

---

## 🔌 Circuit Diagram

```
                          +---------------------+
                          |    NodeMCU ESP8266   |
                          |                     |
    +----------+          |                     |         +----------+
    | MQ-135   |          |                     |         | LCD 16x2 |
    | Gas      |          |                     |         | (I2C)    |
    | Sensor   |          |                     |         |          |
    |  AO -----+--------->| A0 (ADC)            |         |          |
    |  VCC ----+--------->| VIN (5V)       D1 --|-------->| SCL      |
    |  GND ----+--------->| GND            D2 --|-------->| SDA      |
    +----------+          |                VIN -|-------->| VCC (5V) |
                          |                GND -|-------->| GND      |
                          |                     |         +----------+
    +--------+            |                     |
    | Buzzer |            |                     |
    |  (+) --+----------->| D5 (GPIO14)         |
    |  (-) --+----------->| GND                 |
    +--------+            |                     |
                          |                     |
    Green LED ---[220Ω]-->| D6 (GPIO12)         |
    Yellow LED --[220Ω]-->| D7 (GPIO13)         |
    Red LED -----[220Ω]-->| D8 (GPIO15)         |
    All LED GND --------->| GND                 |
                          +---------------------+
```

---

## 🔗 Connection Details

### MQ-135 Gas Sensor → NodeMCU

| MQ-135 Pin | NodeMCU Pin | Description |
|------------|-------------|-------------|
| AO (Analog Out) | A0 | Analog air quality reading (0–1023) |
| VCC | VIN (5V) | Sensor requires 5V power supply |
| GND | GND | Ground |

> **Note:** MQ-135 requires **5V** input. Use the **VIN** pin of NodeMCU (which provides USB 5V) instead of 3V3.

### 16×2 LCD Display (I2C) → NodeMCU

| LCD Pin | NodeMCU Pin | Description |
|---------|-------------|-------------|
| VCC | VIN (5V) | Power supply (5V) |
| GND | GND | Ground |
| SCL | D1 (GPIO5) | I2C Clock |
| SDA | D2 (GPIO4) | I2C Data |

### LEDs → NodeMCU

| LED | NodeMCU Pin | Condition |
|-----|-------------|-----------|
| Green LED (via 220Ω) | D6 (GPIO12) | AQI: Good (0–100) |
| Yellow LED (via 220Ω) | D7 (GPIO13) | AQI: Moderate (101–200) |
| Red LED (via 220Ω) | D8 (GPIO15) | AQI: Unhealthy/Hazardous (>200) |

### Buzzer → NodeMCU

| Buzzer Pin | NodeMCU Pin | Description |
|------------|-------------|-------------|
| (+) | D5 (GPIO14) | Alert signal |
| (−) | GND | Ground |

---

## 📚 Required Libraries

| Library | Author | Purpose |
|---------|--------|---------|
| `ESP8266WiFi` | ESP8266 Community | Wi-Fi connectivity (built-in) |
| `ThingSpeak` | MathWorks | Cloud data upload |
| `LiquidCrystal_I2C` | Frank de Brabander | I2C LCD driver |
| `Wire` | Arduino | I2C communication (built-in) |

---

## 💻 Source Code

📄 **File:** [`iot_air_quality_monitor.ino`](./iot_air_quality_monitor.ino)

---

## ⚙️ How It Works

1. **Power On:** NodeMCU boots up, initializes the MQ-135 sensor, LCD display, LEDs, and buzzer, then connects to Wi-Fi.
2. **Sensor Warm-Up:** The MQ-135 requires a **2-3 minute warm-up period** for accurate readings.
3. **Data Reading:** The analog output (0–1023) from MQ-135 is read via the A0 pin. This raw value is mapped to an AQI scale.
4. **Classification:** Based on the AQI value:
   - **0–100:** Good (Green LED ON) ✅
   - **101–200:** Moderate (Yellow LED ON) ⚠️
   - **201–300:** Unhealthy (Red LED ON) 🔴
   - **>300:** Hazardous (Red LED + Buzzer ON) 🚨
5. **Display:** The AQI value and category are displayed on the LCD screen.
6. **Cloud Upload:** Data is sent to ThingSpeak every 15 seconds for remote monitoring and historical analysis.

---

## ✅ Advantages

- 🌍 **Environmental Awareness:** Detects multiple hazardous gases in real time.
- 📡 **Remote Access:** ThingSpeak allows monitoring from any device with internet access.
- 🚨 **Multi-Level Alerts:** Visual (3 LEDs) and audible (buzzer) alerts categorize severity.
- 💰 **Affordable:** Total project cost under ₹600.
- 📈 **Data Analytics:** ThingSpeak stores historical data for trend analysis and pattern identification.
- 🏠 **Versatile Deployment:** Suitable for homes, offices, factories, and outdoor monitoring stations.
- 🔧 **Simple Calibration:** MQ-135 provides a straightforward analog output proportional to gas concentration.

---

## 🔮 Future Scope

- 🌐 **Multi-Sensor Fusion:** Add MQ-7 (CO), MQ-2 (Smoke/LPG), and dust sensors (GP2Y1010AU0F) for comprehensive monitoring.
- 📱 **Mobile App:** Build a dedicated mobile application with push notifications using Firebase Cloud Messaging.
- 📊 **AQI Standard Mapping:** Map raw sensor values to official AQI standards (EPA/CPCB) for regulatory compliance.
- 🗺️ **GPS Integration:** Add a GPS module to create a geo-tagged air quality map of a city.
- 🏭 **Industrial Grade:** Deploy as a mesh network of sensors for factory floor monitoring with LoRa communication.
- 🤖 **AI-Powered Predictions:** Use machine learning on historical AQI data to predict air quality forecasts.
- 🌿 **Green Building Integration:** Integrate with HVAC systems to automatically improve indoor air quality.
- 📧 **IFTTT Automation:** Trigger automated actions (email alerts, smart fan activation) based on AQI levels.

---

## 📸 Expected Output

- **Serial Monitor:** Raw analog value and computed AQI displayed every 15 seconds.
- **LCD Display:** Shows `AQI: <value>` on line 1 and `Status: Good/Moderate/Unhealthy` on line 2.
- **LEDs:** Corresponding LED lights up based on air quality category.
- **ThingSpeak:** Real-time updating line chart showing AQI trend.

---

## 🔙 [Back to Main Page](../README.md)
