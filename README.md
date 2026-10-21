<p align="center">
  <img src="https://img.shields.io/badge/Platform-Arduino%20%7C%20NodeMCU-blue?style=for-the-badge&logo=arduino" />
  <img src="https://img.shields.io/badge/Domain-IoT-green?style=for-the-badge&logo=internet-explorer" />
  <img src="https://img.shields.io/badge/Projects-10-orange?style=for-the-badge" />
  <img src="https://img.shields.io/badge/Branch-ETC-red?style=for-the-badge" />
</p>

# 🌐 IoT Projects Portfolio — Electronics & Telecommunication Engineering

> A curated collection of **10 Internet of Things (IoT) projects** built using **Arduino UNO** and **NodeMCU ESP8266**, featuring a variety of sensors, actuators, and cloud platforms. Each project includes complete source code, circuit diagrams, connection tables, and detailed documentation.

---

## 👤 About

- **Branch:** Electronics & Telecommunication Engineering (ETC)
- **Domain:** Internet of Things (IoT), Embedded Systems
- **Platforms Used:** Arduino UNO, NodeMCU ESP8266 (ESP-12E)
- **Languages:** C/C++ (Arduino IDE)
- **Cloud Platforms:** ThingSpeak, Blynk IoT

---

## 📂 Projects Overview

| # | Project | Microcontroller | Key Sensors/Modules | Difficulty |
|---|---------|----------------|---------------------|------------|
| 01 | [Smart Temperature & Humidity Monitor](./Project-01-Smart-Temperature-Humidity-Monitor/) | NodeMCU ESP8266 | DHT11 | ⭐⭐ |
| 02 | [IoT Air Quality Monitoring System](./Project-02-IoT-Air-Quality-Monitoring-System/) | NodeMCU ESP8266 | MQ-135 Gas Sensor | ⭐⭐⭐ |
| 03 | [Smart Home Automation System](./Project-03-Smart-Home-Automation-System/) | NodeMCU ESP8266 | Relay Module, IR Sensor | ⭐⭐⭐ |
| 04 | [IoT Weather Station](./Project-04-IoT-Weather-Station/) | NodeMCU ESP8266 | BMP280, DHT22 | ⭐⭐⭐ |
| 05 | [Smart Irrigation System](./Project-05-Smart-Irrigation-System/) | NodeMCU ESP8266 | Soil Moisture Sensor | ⭐⭐⭐ |
| 06 | [IoT Fire Detection & Alert System](./Project-06-IoT-Fire-Detection-Alert-System/) | Arduino UNO | Flame Sensor, MQ-2 | ⭐⭐⭐ |
| 07 | [Smart Parking System](./Project-07-Smart-Parking-System/) | Arduino UNO | HC-SR04 Ultrasonic, Servo | ⭐⭐⭐ |
| 08 | [IoT Health Monitoring System](./Project-08-IoT-Health-Monitoring-System/) | NodeMCU ESP8266 | Pulse Sensor, LM35 | ⭐⭐⭐⭐ |
| 09 | [Smart Street Light System](./Project-09-Smart-Street-Light-System/) | Arduino UNO | LDR, PIR Motion Sensor | ⭐⭐ |
| 10 | [IoT Door Security System with RFID](./Project-10-IoT-Door-Security-System-RFID/) | NodeMCU ESP8266 | RFID RC522, Servo | ⭐⭐⭐⭐ |

---

## 🛠️ Common Tools & Software

| Tool | Purpose |
|------|---------|
| [Arduino IDE](https://www.arduino.cc/en/software) | Code compilation and uploading |
| [Fritzing](https://fritzing.org/) | Circuit diagram design |
| [ThingSpeak](https://thingspeak.com/) | IoT cloud data logging & visualization |
| [Blynk IoT](https://blynk.io/) | Mobile app-based IoT control |
| [Serial Monitor](https://www.arduino.cc/en/Guide/Environment#serial-monitor) | Debugging and data viewing |

---

## 🔧 Board Setup in Arduino IDE

### Arduino UNO
- Board: `Arduino UNO` (available by default)
- Port: Select the appropriate COM port

### NodeMCU ESP8266
1. Go to **File → Preferences**
2. Add this URL in **Additional Board Manager URLs**:
   ```
   http://arduino.esp8266.com/stable/package_esp8266com_index.json
   ```
3. Go to **Tools → Board → Board Manager** → Search for `ESP8266` → Install
4. Select **Board: NodeMCU 1.0 (ESP-12E Module)**

---

## 📁 Repository Structure

```
ETC/
├── README.md                                          ← You are here
├── Project-01-Smart-Temperature-Humidity-Monitor/
│   ├── README.md
│   └── smart_temp_humidity_monitor.ino
├── Project-02-IoT-Air-Quality-Monitoring-System/
│   ├── README.md
│   └── iot_air_quality_monitor.ino
├── Project-03-Smart-Home-Automation-System/
│   ├── README.md
│   └── smart_home_automation.ino
├── Project-04-IoT-Weather-Station/
│   ├── README.md
│   └── iot_weather_station.ino
├── Project-05-Smart-Irrigation-System/
│   ├── README.md
│   └── smart_irrigation_system.ino
├── Project-06-IoT-Fire-Detection-Alert-System/
│   ├── README.md
│   └── fire_detection_alert.ino
├── Project-07-Smart-Parking-System/
│   ├── README.md
│   └── smart_parking_system.ino
├── Project-08-IoT-Health-Monitoring-System/
│   ├── README.md
│   └── iot_health_monitor.ino
├── Project-09-Smart-Street-Light-System/
│   ├── README.md
│   └── smart_street_light.ino
├── Project-10-IoT-Door-Security-System-RFID/
│   ├── README.md
│   └── iot_door_security_rfid.ino
```

---

## 🚀 How to Use

1. **Clone this repository:**
   ```bash
   git clone https://github.com/<your-username>/ETC-IoT-Projects.git
   ```
2. **Open any project folder** and read its `README.md` for full details.
3. **Open the `.ino` file** in Arduino IDE.
4. **Install required libraries** (mentioned in each project's README).
5. **Connect the circuit** as per the connection table and diagram.
6. **Upload the code** to your board and test!

---

## 📜 License

This repository is licensed under the [MIT License](./LICENSE). Feel free to use, modify, and distribute.

---

<p align="center">
  <b>⭐ If you found this helpful, please give it a star! ⭐</b>
</p>
