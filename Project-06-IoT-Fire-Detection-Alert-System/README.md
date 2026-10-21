# Project 06: IoT Fire Detection & Alert System 🔥🚨

<p align="center">
  <img src="https://img.shields.io/badge/Board-Arduino%20UNO-blue?style=flat-square&logo=arduino" />
  <img src="https://img.shields.io/badge/Sensors-Flame%20%2B%20MQ--2-green?style=flat-square" />
  <img src="https://img.shields.io/badge/Alert-Buzzer%20%2B%20GSM-red?style=flat-square" />
  <img src="https://img.shields.io/badge/Status-Complete-brightgreen?style=flat-square" />
</p>

## 📋 Project Description

This project builds a **multi-sensor fire detection and alert system** using **Arduino UNO**. It combines an **IR flame sensor** (detects infrared light from fire) and an **MQ-2 gas/smoke sensor** (detects smoke, LPG, methane) for reliable dual-verification fire detection. When both sensors detect a hazard, the system activates a **loud buzzer alarm**, flashes a **red warning LED**, displays the alert on a **16×2 LCD**, and can optionally send an **SMS alert** via a GSM module (SIM800L). This dual-sensor approach minimizes false alarms.

---

## 🧰 Components Required

| # | Component | Quantity | Specification |
|---|-----------|----------|---------------|
| 1 | Arduino UNO | 1 | ATmega328P microcontroller |
| 2 | IR Flame Sensor Module | 1 | Digital + Analog output, 760–1100nm detection |
| 3 | MQ-2 Gas/Smoke Sensor | 1 | Detects smoke, LPG, methane, hydrogen |
| 4 | 16×2 LCD Display (I2C) | 1 | HD44780 + I2C backpack |
| 5 | Buzzer | 1 | 5V Piezo buzzer (loud) |
| 6 | LED (Red) | 2 | 5mm — Fire warning |
| 7 | LED (Green) | 1 | 5mm — System OK |
| 8 | Resistors (220Ω) | 3 | For LEDs |
| 9 | SIM800L GSM Module (Optional) | 1 | For SMS alerts |
| 10 | Breadboard | 1 | Full-size |
| 11 | Jumper Wires | ~25 | Assorted |
| 12 | USB Cable (Type B) | 1 | For programming |
| 13 | 9V Battery / Adapter | 1 | External power |

---

## 🔌 Circuit Diagram

```
                          +---------------------+
                          |     Arduino UNO     |
                          |                     |
    +-----------+         |                     |       +----------+
    | Flame     |         |                     |       | LCD 16x2 |
    | Sensor    |         |                     |       | (I2C)    |
    |  DO ------+-------->| D2            A4 ---|------>| SDA      |
    |  AO ------+-------->| A1            A5 ---|------>| SCL      |
    |  VCC -----+-------->| 5V            5V ---|------>| VCC      |
    |  GND -----+-------->| GND          GND ---|------>| GND      |
    +-----------+         |                     |       +----------+
                          |                     |
    +-----------+         |                     |
    | MQ-2      |         |                     |
    | Smoke     |         |                     |
    |  DO ------+-------->| D3                  |
    |  AO ------+-------->| A0                  |
    |  VCC -----+-------->| 5V                  |
    |  GND -----+-------->| GND                 |
    +-----------+         |                     |
                          |                     |
    Buzzer (+) ---------->| D8                  |
    Buzzer (-) ---------->| GND                 |
                          |                     |
    Red LED 1 ---[220Ω]-->| D9                  |
    Red LED 2 ---[220Ω]-->| D10                 |
    Green LED ---[220Ω]-->| D11                 |
    All LED GND --------->| GND                 |
                          |                     |
    (Optional GSM)        |                     |
    SIM800L TX ---------->| D7 (SoftSerial RX)  |
    SIM800L RX ---------->| D6 (SoftSerial TX)  |
    SIM800L GND --------->| GND                 |
                          +---------------------+
```

---

## 🔗 Connection Details

### IR Flame Sensor → Arduino UNO

| Flame Sensor Pin | Arduino Pin | Description |
|------------------|-------------|-------------|
| DO (Digital Out) | D2 | Digital output (LOW when flame detected) |
| AO (Analog Out) | A1 | Analog flame intensity (0–1023) |
| VCC | 5V | Power supply |
| GND | GND | Ground |

> **Note:** The digital output is **active LOW** — it reads LOW when flame is detected.

### MQ-2 Smoke/Gas Sensor → Arduino UNO

| MQ-2 Pin | Arduino Pin | Description |
|----------|-------------|-------------|
| DO (Digital Out) | D3 | Digital output (LOW when smoke detected) |
| AO (Analog Out) | A0 | Analog smoke concentration (0–1023) |
| VCC | 5V | Power supply |
| GND | GND | Ground |

> **Warm-Up:** MQ-2 requires **2–5 minutes** of warm-up for accurate readings.

### LCD Display (I2C) → Arduino UNO

| LCD Pin | Arduino Pin | Description |
|---------|-------------|-------------|
| VCC | 5V | Power |
| GND | GND | Ground |
| SDA | A4 | I2C Data |
| SCL | A5 | I2C Clock |

### Buzzer & LEDs → Arduino UNO

| Component | Arduino Pin | Description |
|-----------|-------------|-------------|
| Buzzer (+) | D8 | Alarm sound |
| Red LED 1 (220Ω) | D9 | Fire alert indicator 1 |
| Red LED 2 (220Ω) | D10 | Fire alert indicator 2 |
| Green LED (220Ω) | D11 | System normal indicator |

### SIM800L GSM Module (Optional) → Arduino UNO

| GSM Pin | Arduino Pin | Description |
|---------|-------------|-------------|
| TX | D7 (SoftSerial RX) | GSM → Arduino |
| RX | D6 (SoftSerial TX) | Arduino → GSM (via voltage divider) |
| GND | GND | Common ground |
| VCC | 3.7V LiPo Battery | Separate power (not 5V!) |

---

## 📚 Required Libraries

| Library | Author | Purpose |
|---------|--------|---------|
| `Wire` | Arduino | I2C communication (built-in) |
| `LiquidCrystal_I2C` | Frank de Brabander | I2C LCD control |
| `SoftwareSerial` | Arduino | Software serial for GSM (built-in) |

---

## 💻 Source Code

📄 **File:** [`fire_detection_alert.ino`](./fire_detection_alert.ino)

---

## ⚙️ How It Works

1. **Initialization:** Arduino initializes both sensors, LCD, buzzer, LEDs, and the optional GSM module. The MQ-2 enters a warm-up phase.
2. **Continuous Monitoring:** Both sensors are polled every 500ms:
   - **Flame Sensor:** Checks digital pin for flame detection, reads analog value for intensity.
   - **MQ-2 Sensor:** Checks digital pin for smoke detection, reads analog value for concentration.
3. **Detection Logic:**
   - **Flame Only:** Warning — possible fire. Red LED 1 blinks.
   - **Smoke Only:** Warning — possible gas leak. Red LED 2 blinks.
   - **Flame + Smoke:** **FIRE CONFIRMED** 🔥 — Both LEDs flash, buzzer sounds continuously, LCD shows "FIRE ALERT!", and SMS is sent.
   - **Neither:** Green LED stays on. LCD shows "Status: Normal".
4. **Alert Actions:** Loud buzzer alarm, alternating red LED flash pattern, LCD warning, and optional SMS to pre-configured phone number.

---

## ✅ Advantages

- 🔥 **Dual Verification:** Using both flame + smoke sensors drastically reduces false alarms.
- ⚡ **Fast Response:** Detection within 500ms of fire/smoke presence.
- 📢 **Multi-Modal Alert:** Visual (LEDs), audible (buzzer), display (LCD), and remote (SMS) alerts.
- 🛡️ **Wide Gas Detection:** MQ-2 detects smoke, LPG, methane, and hydrogen — covers multiple hazards.
- 💰 **Low Cost:** Total system under ₹600 (without GSM module).
- 🏠 **Indoor Deployment:** Ideal for homes, kitchens, laboratories, and small offices.
- 🔧 **Adjustable Sensitivity:** Both sensors have onboard potentiometers for threshold adjustment.

---

## 🔮 Future Scope

- 🌐 **IoT Integration:** Replace Arduino UNO with NodeMCU/ESP32 for Wi-Fi connectivity and cloud alerts.
- 📱 **Mobile Notifications:** Push notifications via Firebase Cloud Messaging or Telegram Bot API.
- 🎥 **Camera Integration:** Add an ESP32-CAM to capture images during fire detection and send to authorities.
- 🏢 **Multi-Zone System:** Deploy sensors in different rooms/zones with a central monitoring hub.
- 🚿 **Automatic Sprinkler:** Activate a solenoid-controlled sprinkler system upon fire detection.
- 🤖 **AI Vision:** Use thermal imaging (MLX90640) and AI to distinguish fire from other heat sources.
- 📊 **Data Logging:** Log all sensor events to an SD card for post-incident analysis.
- 🏭 **Industrial Safety:** Scale to industrial environments with explosion-proof enclosures and compliance with fire safety standards.
- 🔋 **Battery Backup:** Add a rechargeable battery with charging circuit for uninterrupted operation during power outages.

---

## 📸 Expected Output

- **LCD:** `Status: Normal` or `!! FIRE ALERT !!` with smoke/flame levels.
- **Serial Monitor:** Continuous analog readings from both sensors.
- **LEDs:** Green (normal), Red flashing (alert).
- **Buzzer:** Continuous loud alarm during confirmed fire.

---

## 🔙 [Back to Main Page](../README.md)
