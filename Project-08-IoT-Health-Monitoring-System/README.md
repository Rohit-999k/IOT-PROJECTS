# Project 08: IoT Health Monitoring System ❤️🩺

<p align="center">
  <img src="https://img.shields.io/badge/Board-NodeMCU%20ESP8266-blue?style=flat-square&logo=arduino" />
  <img src="https://img.shields.io/badge/Sensors-Pulse%20%2B%20LM35-green?style=flat-square" />
  <img src="https://img.shields.io/badge/Cloud-ThingSpeak-orange?style=flat-square" />
  <img src="https://img.shields.io/badge/Status-Complete-brightgreen?style=flat-square" />
</p>

## 📋 Project Description

This project develops an **IoT-based Health Monitoring System** that measures a patient's **heart rate (BPM)** using a **pulse sensor** and **body temperature** using an **LM35 analog temperature sensor**. The vital signs are displayed on a **0.96" OLED display** in real time, uploaded to **ThingSpeak** for remote monitoring by healthcare providers, and alerts are triggered when readings exceed normal ranges. This system is ideal for **remote patient monitoring**, elderly care, and personal health tracking.

---

## 🧰 Components Required

| # | Component | Quantity | Specification |
|---|-----------|----------|---------------|
| 1 | NodeMCU ESP8266 (ESP-12E) | 1 | Wi-Fi microcontroller |
| 2 | Pulse Sensor (SEN-11574) | 1 | Analog heart rate sensor |
| 3 | LM35 Temperature Sensor | 1 | Analog, 10mV/°C, body temperature |
| 4 | 0.96" OLED Display | 1 | SSD1306, 128×64, I2C |
| 5 | Buzzer | 1 | 5V — Alert for abnormal readings |
| 6 | LED (Red) | 1 | 5mm — Heartbeat blink indicator |
| 7 | Resistor (220Ω) | 1 | For LED |
| 8 | Breadboard | 1 | Half-size |
| 9 | Jumper Wires | ~15 | Assorted |
| 10 | Micro USB Cable | 1 | Power & programming |

---

## 🔌 Circuit Diagram

```
                          +---------------------+
                          |    NodeMCU ESP8266   |
                          |                     |
    +----------+          |                     |         +----------+
    | Pulse    |          |                     |         |  OLED    |
    | Sensor   |          |                     |         | SSD1306  |
    |  Signal--+--------->| A0 (ADC)            |         |          |
    |  VCC ----+--------->| 3V3           D1 ---|-------->| SCL      |
    |  GND ----+--------->| GND           D2 ---|-------->| SDA      |
    +----------+          |               3V3 --|-------->| VCC      |
                          |               GND --|-------->| GND      |
    +----------+          |                     |         +----------+
    | LM35     |          |                     |
    | Temp     |          |                     |
    |  Vout ---+--------->| D4 (GPIO2)*         |
    |  VCC ----+--------->| 3V3                 |
    |  GND ----+--------->| GND                 |
    +----------+          |                     |
                          |                     |
    * Note: Since NodeMCU has only 1 ADC (A0),  |
      LM35 is read via a multiplexing trick     |
      or via an external ADC. In this code,     |
      we alternate readings on A0.              |
                          |                     |
    Buzzer (+) ---------->| D5 (GPIO14)         |
    Buzzer (-) ---------->| GND                 |
                          |                     |
    Red LED ---[220Ω]---->| D6 (GPIO12)         |
    LED GND ------------->| GND                 |
                          +---------------------+
    
    * Alternative: Use analog multiplexer (CD4051)
      or external ADC (ADS1115) for multiple analog
      sensors on NodeMCU.
```

---

## 🔗 Connection Details

### Pulse Sensor → NodeMCU

| Pulse Sensor Pin | NodeMCU Pin | Description |
|------------------|-------------|-------------|
| Signal (S) | A0 | Analog pulse signal |
| VCC (+) | 3V3 | Power supply |
| GND (−) | GND | Ground |

> **Usage:** Place finger gently on the sensor surface. Avoid pressing too hard.

### LM35 Temperature Sensor → NodeMCU

| LM35 Pin | NodeMCU Pin | Description |
|----------|-------------|-------------|
| Vout (Center) | A0 (shared, alternating reads) | Analog temperature (10mV/°C) |
| VCC (Left, flat side facing) | 3V3 | Power supply |
| GND (Right) | GND | Ground |

> **Note:** Since NodeMCU has only **one ADC pin (A0)**, we use a software-based time-division multiplexing approach, or use an **ADS1115 external ADC** for simultaneous readings. The code provides both approaches.

### OLED Display (I2C) → NodeMCU

| OLED Pin | NodeMCU Pin | Description |
|----------|-------------|-------------|
| VCC | 3V3 | Power |
| GND | GND | Ground |
| SCL | D1 (GPIO5) | I2C Clock |
| SDA | D2 (GPIO4) | I2C Data |

### Buzzer & LED → NodeMCU

| Component | NodeMCU Pin | Description |
|-----------|-------------|-------------|
| Buzzer (+) | D5 (GPIO14) | Alert buzzer |
| Red LED (via 220Ω) | D6 (GPIO12) | Heartbeat indicator |

---

## 📚 Required Libraries

| Library | Author | Purpose |
|---------|--------|---------|
| `ESP8266WiFi` | ESP8266 Community | Wi-Fi (built-in) |
| `ThingSpeak` | MathWorks | Cloud data upload |
| `Adafruit SSD1306` | Adafruit | OLED display driver |
| `Adafruit GFX Library` | Adafruit | Graphics for OLED |
| `PulseSensorPlayground` | Joel Murphy & Yury Gitman | Pulse sensor BPM calculation |
| `Wire` | Arduino | I2C (built-in) |

---

## 💻 Source Code

📄 **File:** [`iot_health_monitor.ino`](./iot_health_monitor.ino)

---

## ⚙️ How It Works

1. **Pulse Detection:** The pulse sensor uses a green LED and photodetector to measure blood volume changes in the fingertip (photoplethysmography). Each heartbeat causes a spike in the signal.
2. **BPM Calculation:** The PulseSensorPlayground library analyzes the analog waveform to calculate **Beats Per Minute (BPM)** using inter-beat interval (IBI) timing.
3. **Temperature Reading:** LM35 outputs an analog voltage proportional to temperature (10mV per °C). The ADC converts this to a digital value, which is then calculated as: `Temperature = (analogValue * 3.3 / 1024) * 100`.
4. **Display:** The OLED shows a real-time dashboard with BPM (with heart icon animation), body temperature, and status.
5. **Alert System:**
   - Normal: BPM 60–100, Temp 36.1–37.5°C → Green status.
   - Abnormal: BPM < 50 or > 120, Temp > 38°C → Buzzer alarm + warning on OLED.
6. **Cloud Upload:** BPM and temperature are sent to ThingSpeak every 15 seconds.

---

## ✅ Advantages

- ❤️ **Non-Invasive:** Pulse sensor requires only a gentle finger touch — no needles or invasive procedures.
- 📡 **Remote Monitoring:** Healthcare providers can monitor vitals remotely via ThingSpeak.
- ⏱️ **Real-Time:** Continuous monitoring with instant alerts for abnormal readings.
- 🏥 **Affordable Telehealth:** Costs under ₹600 — democratizes health monitoring for rural areas.
- 🔔 **Automatic Alerts:** Immediate buzzer alarm for critical readings.
- 📊 **Historical Data:** ThingSpeak stores historical data for trend analysis by doctors.
- 🧓 **Elderly Care:** Ideal for continuous monitoring of elderly patients living alone.

---

## 🔮 Future Scope

- 🩸 **SpO2 Monitoring:** Upgrade pulse sensor to MAX30102 for blood oxygen saturation (SpO2) measurement.
- 📱 **Mobile Health App:** Build a dedicated app with push notifications and health reports.
- 📈 **ECG Integration:** Add an AD8232 ECG module for detailed cardiac rhythm monitoring.
- 🤖 **AI Health Analysis:** Use ML on historical data to predict health deterioration patterns.
- ⌚ **Wearable Design:** Miniaturize into a wearable wristband using ESP32 and flexible PCB.
- 🏥 **Hospital Integration:** Connect to hospital management systems (HL7/FHIR protocols).
- 📋 **Multi-Patient:** Extend to monitor multiple patients with individual dashboards.
- 🔐 **Data Security:** Implement encrypted data transmission for HIPAA compliance.
- 📧 **Emergency Contacts:** Auto-send email/SMS to emergency contacts when critical readings detected.

---

## 📸 Expected Output

- **OLED Display:** Shows `♥ BPM: 72` and `Temp: 36.8°C` with status indicator.
- **LED:** Red LED blinks in sync with each heartbeat.
- **Serial Monitor:** Raw pulse signal, BPM, and temperature readings.
- **ThingSpeak:** Live line charts for BPM and temperature over time.
- **Buzzer:** Alerts when BPM or temperature is out of normal range.

---

## 🔙 [Back to Main Page](../README.md)
