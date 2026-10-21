# Project 04: IoT Weather Station ☁️🌦️

<p align="center">
  <img src="https://img.shields.io/badge/Board-NodeMCU%20ESP8266-blue?style=flat-square&logo=arduino" />
  <img src="https://img.shields.io/badge/Sensors-BMP280%20%2B%20DHT22-green?style=flat-square" />
  <img src="https://img.shields.io/badge/Cloud-ThingSpeak-orange?style=flat-square" />
  <img src="https://img.shields.io/badge/Status-Complete-brightgreen?style=flat-square" />
</p>

## 📋 Project Description

This project creates a **compact IoT Weather Station** capable of measuring **temperature, humidity, barometric pressure, and altitude**. It uses the **DHT22 sensor** for temperature and humidity, and the **BMP280 sensor** for pressure and altitude measurements. All data is displayed on a **0.96" OLED display** and uploaded to **ThingSpeak** for cloud-based visualization and historical data logging. The system provides accurate, real-time environmental data comparable to commercial weather stations.

---

## 🧰 Components Required

| # | Component | Quantity | Specification |
|---|-----------|----------|---------------|
| 1 | NodeMCU ESP8266 (ESP-12E) | 1 | Wi-Fi microcontroller |
| 2 | BMP280 Sensor Module | 1 | I2C barometric pressure sensor |
| 3 | DHT22 Sensor | 1 | High-accuracy temp & humidity |
| 4 | 0.96" OLED Display | 1 | SSD1306, 128×64, I2C |
| 5 | Resistor (10kΩ) | 1 | Pull-up for DHT22 |
| 6 | Breadboard | 1 | Half-size or full-size |
| 7 | Jumper Wires | ~15 | Male-to-Male, Male-to-Female |
| 8 | Micro USB Cable | 1 | Power & programming |

---

## 🔌 Circuit Diagram

```
                          +---------------------+
                          |    NodeMCU ESP8266   |
                          |                     |
    +--------+            |                     |         +----------+
    | DHT22  |            |                     |         |  OLED    |
    |  DATA--+--[10kΩ]--->| D4 (GPIO2)          |         | SSD1306  |
    |  VCC --+----------->| 3V3                 |         |          |
    |  GND --+----------->| GND                 |         |          |
    +--------+            |                     |         |          |
                          |               D1 ---|-------->| SCL      |
    +--------+            |               D2 ---|-------->| SDA      |
    | BMP280 |            |               3V3 --|-------->| VCC      |
    |  SCL --+----------->| D1 (GPIO5)*   GND --|-------->| GND      |
    |  SDA --+----------->| D2 (GPIO4)*         |         +----------+
    |  VCC --+----------->| 3V3                 |
    |  GND --+----------->| GND                 |
    +--------+            +---------------------+
    
    * BMP280 and OLED share the I2C bus (D1=SCL, D2=SDA)
      BMP280 default I2C address: 0x76
      OLED default I2C address:   0x3C
```

---

## 🔗 Connection Details

### DHT22 Sensor → NodeMCU

| DHT22 Pin | NodeMCU Pin | Description |
|-----------|-------------|-------------|
| VCC | 3V3 | Power supply |
| DATA | D4 (GPIO2) | Digital data (with 10kΩ pull-up to 3V3) |
| GND | GND | Ground |

### BMP280 Sensor (I2C) → NodeMCU

| BMP280 Pin | NodeMCU Pin | Description |
|------------|-------------|-------------|
| VCC | 3V3 | Power supply |
| GND | GND | Ground |
| SCL | D1 (GPIO5) | I2C Clock (shared bus) |
| SDA | D2 (GPIO4) | I2C Data (shared bus) |

> **I2C Address:** BMP280 default address is **0x76**. If your module uses 0x77, update the code accordingly.

### OLED Display (SSD1306) → NodeMCU

| OLED Pin | NodeMCU Pin | Description |
|----------|-------------|-------------|
| VCC | 3V3 | Power supply |
| GND | GND | Ground |
| SCL | D1 (GPIO5) | I2C Clock (shared bus) |
| SDA | D2 (GPIO4) | I2C Data (shared bus) |

> **Note:** BMP280 and OLED **share the same I2C bus** (D1/D2). They have different I2C addresses so they work simultaneously.

---

## 📚 Required Libraries

| Library | Author | Purpose |
|---------|--------|---------|
| `DHT sensor library` | Adafruit | DHT22 sensor readings |
| `Adafruit Unified Sensor` | Adafruit | Sensor abstraction layer |
| `Adafruit BMP280` | Adafruit | BMP280 pressure sensor driver |
| `Adafruit SSD1306` | Adafruit | OLED display driver |
| `Adafruit GFX Library` | Adafruit | Graphics library for OLED |
| `ThingSpeak` | MathWorks | Cloud data upload |
| `ESP8266WiFi` | ESP8266 Community | Wi-Fi (built-in) |

---

## 💻 Source Code

📄 **File:** [`iot_weather_station.ino`](./iot_weather_station.ino)

---

## ⚙️ How It Works

1. **Boot Sequence:** NodeMCU initializes both I2C sensors (BMP280 and OLED), the DHT22, and connects to Wi-Fi.
2. **Sensor Readings (every 20 seconds):**
   - **DHT22:** Reads temperature (°C) and relative humidity (%).
   - **BMP280:** Reads barometric pressure (hPa) and computes approximate altitude (m) using the barometric formula.
3. **Local Display:** All 4 parameters are displayed on the OLED screen in a clean, formatted layout with labels and units.
4. **Cloud Upload:** Data is sent to ThingSpeak across 4 fields for live chart visualization.
5. **Continuous Monitoring:** The system runs indefinitely, updating all readings and the cloud at regular intervals.

---

## ✅ Advantages

- 🎯 **Multi-Parameter:** Measures 4 environmental parameters (temp, humidity, pressure, altitude) simultaneously.
- 🔬 **High Accuracy:** DHT22 provides ±0.5°C temperature accuracy; BMP280 provides ±1 hPa pressure accuracy.
- 📡 **Cloud-Connected:** Historical data available on ThingSpeak for weather trend analysis.
- 🖥️ **Local Display:** OLED provides instant, at-a-glance readings without needing a phone.
- 💰 **Budget Friendly:** All components cost under ₹700.
- 🔋 **Low Power:** Both sensors have minimal power draw, suitable for battery-powered deployment.
- 📐 **Compact Design:** All components fit on a small breadboard — easy to enclose in a weather-proof case.

---

## 🔮 Future Scope

- 🌧️ **Rain Sensor:** Add a rain detection module to monitor precipitation.
- 💨 **Wind Sensor:** Integrate an anemometer for wind speed and direction measurement.
- ☀️ **UV Index:** Add a UV sensor (VEML6075) for UV radiation monitoring.
- 🌐 **Web Dashboard:** Host a real-time web dashboard on the NodeMCU itself using ESPAsyncWebServer.
- 📱 **Weather App:** Build a custom mobile app that fetches data from ThingSpeak API.
- 🔄 **Multi-Station Network:** Deploy multiple weather stations and aggregate data for micro-climate mapping.
- ⚡ **Solar Powered:** Design a solar-powered enclosure for outdoor, off-grid deployment.
- 📈 **Weather Prediction:** Use historical barometric pressure trends to predict upcoming weather changes (rising pressure = fair weather, falling = storm).
- 🏫 **Educational Tool:** Use in school/college labs for teaching meteorology and IoT concepts.

---

## 📸 Expected Output

- **OLED Display:** Shows Temp, Humidity, Pressure, and Altitude in a 4-line format.
- **Serial Monitor:** Prints all 4 readings with timestamps.
- **ThingSpeak:** 4 separate live charts updating every 20 seconds.

---

## 🔙 [Back to Main Page](../README.md)
