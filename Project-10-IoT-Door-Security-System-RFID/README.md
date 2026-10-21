# Project 10: IoT Door Security System with RFID 🔐🚪

<p align="center">
  <img src="https://img.shields.io/badge/Board-NodeMCU%20ESP8266-blue?style=flat-square&logo=arduino" />
  <img src="https://img.shields.io/badge/Module-RFID%20RC522-green?style=flat-square" />
  <img src="https://img.shields.io/badge/Cloud-Blynk%20IoT-orange?style=flat-square" />
  <img src="https://img.shields.io/badge/Status-Complete-brightgreen?style=flat-square" />
</p>

## 📋 Project Description

This project implements an **IoT-based Door Security System** using **RFID (RC522)** authentication with **NodeMCU ESP8266**. Authorized users tap their **RFID cards or key fobs** to unlock the door via a **servo motor-controlled lock**. The system logs every access attempt (authorized or unauthorized) to the **Blynk IoT app** with timestamps, and sends real-time notifications. An **OLED display** shows the access status, and a **buzzer** provides audio feedback. Unauthorized access attempts trigger alerts, making it a complete smart lock solution.

---

## 🧰 Components Required

| # | Component | Quantity | Specification |
|---|-----------|----------|---------------|
| 1 | NodeMCU ESP8266 (ESP-12E) | 1 | Wi-Fi microcontroller |
| 2 | RFID Reader Module (MFRC522/RC522) | 1 | 13.56 MHz, SPI interface |
| 3 | RFID Cards / Key Fobs | 2–3 | 13.56 MHz MIFARE Classic |
| 4 | Servo Motor (SG90) | 1 | Door lock actuator |
| 5 | 0.96" OLED Display | 1 | SSD1306, 128×64, I2C |
| 6 | Buzzer | 1 | 5V Active Buzzer |
| 7 | LED (Green) | 1 | 5mm — Access granted |
| 8 | LED (Red) | 1 | 5mm — Access denied |
| 9 | Resistors (220Ω) | 2 | For LEDs |
| 10 | Breadboard | 1 | Full-size |
| 11 | Jumper Wires | ~20 | Assorted |
| 12 | Micro USB Cable | 1 | Power & programming |

---

## 🔌 Circuit Diagram

```
                          +---------------------+
                          |    NodeMCU ESP8266   |
                          |                     |
    +----------+          |        SPI Bus       |         +----------+
    | RFID     |          |                     |         |  OLED    |
    | RC522    |          |                     |         | SSD1306  |
    |          |          |                     |         |          |
    | SDA -----+--------->| D4 (GPIO2)          |         |          |
    | SCK -----+--------->| D5 (GPIO14)    D1 --|-------->| SCL      |
    | MOSI ----+--------->| D7 (GPIO13)    D2 --|-------->| SDA      |
    | MISO ----+--------->| D6 (GPIO12)   3V3 --|-------->| VCC      |
    | IRQ -----+          | (Not connected) GND-|-------->| GND      |
    | GND -----+--------->| GND                 |         +----------+
    | RST -----+--------->| D3 (GPIO0)          |
    | 3.3V ----+--------->| 3V3                 |
    +----------+          |                     |
                          |                     |
    +--------+            |                     |
    | Servo  |            |                     |
    | SG90   |            |                     |
    | Signal-+----------->| D8 (GPIO15)         |
    | VCC ---+----------->| VIN (5V)            |
    | GND ---+----------->| GND                 |
    +--------+            |                     |
                          |                     |
    Buzzer (+) ---------->| D0 (GPIO16)         |
    Buzzer (-) ---------->| GND                 |
                          |                     |
    Green LED --[220Ω]--->| TX (GPIO1)*         |
    Red LED ----[220Ω]--->| RX (GPIO3)*         |
    LED GND's ----------->| GND                 |
                          +---------------------+
    
    * TX/RX used as GPIO after Serial.begin() is
      removed. Alternatively, use a GPIO expander.
      For debugging, keep Serial and omit LEDs.
```

---

## 🔗 Connection Details

### RFID RC522 Module (SPI) → NodeMCU

| RC522 Pin | NodeMCU Pin | Description |
|-----------|-------------|-------------|
| SDA (SS) | D4 (GPIO2) | SPI Slave Select |
| SCK | D5 (GPIO14) | SPI Clock |
| MOSI | D7 (GPIO13) | SPI Master Out Slave In |
| MISO | D6 (GPIO12) | SPI Master In Slave Out |
| IRQ | Not Connected | Interrupt (not used) |
| GND | GND | Ground |
| RST | D3 (GPIO0) | Reset |
| 3.3V | 3V3 | Power (**Do NOT use 5V!**) |

> ⚠️ **CRITICAL:** The RC522 operates at **3.3V only**. Connecting to 5V will damage the module!

### Servo Motor (SG90) → NodeMCU

| Servo Pin | NodeMCU Pin | Description |
|-----------|-------------|-------------|
| Signal (Orange) | D8 (GPIO15) | PWM control |
| VCC (Red) | VIN (5V) | Power |
| GND (Brown) | GND | Ground |

> **Lock Positions:** 0° = Locked, 90° = Unlocked.

### OLED Display (I2C) → NodeMCU

| OLED Pin | NodeMCU Pin | Description |
|----------|-------------|-------------|
| VCC | 3V3 | Power |
| GND | GND | Ground |
| SCL | D1 (GPIO5) | I2C Clock |
| SDA | D2 (GPIO4) | I2C Data |

### Buzzer & LEDs → NodeMCU

| Component | NodeMCU Pin | Description |
|-----------|-------------|-------------|
| Buzzer (+) | D0 (GPIO16) | Audio feedback |
| Green LED (via 220Ω) | TX (GPIO1) | Access granted |
| Red LED (via 220Ω) | RX (GPIO3) | Access denied |

> **Note:** If using TX/RX for LEDs, `Serial.begin()` must be removed. For development, use Serial and omit LEDs, adding them for final deployment.

---

## 📚 Required Libraries

| Library | Author | Purpose |
|---------|--------|---------|
| `MFRC522` | Miguel Balboa | RFID RC522 driver (SPI) |
| `SPI` | Arduino | SPI communication (built-in) |
| `Servo` | Arduino | Servo motor control |
| `ESP8266WiFi` | ESP8266 Community | Wi-Fi (built-in) |
| `BlynkSimpleEsp8266` | Blynk Inc. | IoT app integration |
| `Adafruit SSD1306` | Adafruit | OLED display |
| `Adafruit GFX Library` | Adafruit | Graphics for OLED |
| `Wire` | Arduino | I2C (built-in) |

---

## 💻 Source Code

📄 **File:** [`iot_door_security_rfid.ino`](./iot_door_security_rfid.ino)

---

## ⚙️ How It Works

1. **Initialization:** NodeMCU initializes the RFID reader (SPI), servo, OLED, buzzer, and connects to Wi-Fi + Blynk.
2. **Card Scanning:** The system continuously waits for an RFID card/fob to be presented.
3. **UID Reading:** When a card is detected, the RC522 reads its unique **UID (Unique Identifier)** — a 4-byte hexadecimal value.
4. **Authentication:**
   - The UID is compared against a **whitelist** of authorized UIDs stored in the code.
   - **Match → Access Granted:** Green LED ON, success beep, servo rotates to 90° (unlocked), OLED shows "ACCESS GRANTED", Blynk logs the event. After 5 seconds, servo returns to 0° (locked).
   - **No Match → Access Denied:** Red LED ON, error buzzer pattern, OLED shows "ACCESS DENIED", Blynk sends alert notification.
5. **Logging:** Every access attempt (time, UID, result) is logged to the Blynk app's terminal widget.
6. **Remote Lock/Unlock:** A button in the Blynk app (V1) allows remote lock/unlock control.

---

## 🔑 How to Find Your RFID Card UID

1. Upload the code to NodeMCU.
2. Open **Serial Monitor** at 115200 baud.
3. Tap your RFID card on the reader.
4. The Serial Monitor will display: `Card UID: XX XX XX XX`
5. Copy this UID and add it to the `authorizedUIDs[]` array in the code.

---

## ✅ Advantages

- 🔒 **Keyless Entry:** No physical keys needed — reduces risk of key loss or duplication.
- 🆔 **Unique Identification:** Each RFID card has a globally unique ID — cannot be duplicated easily.
- 📱 **Remote Control:** Lock/unlock from anywhere via Blynk app.
- 📋 **Access Log:** Complete log of all access attempts with timestamps for security audit.
- 🔔 **Instant Alerts:** Push notifications on unauthorized access attempts.
- ⚡ **Fast Authentication:** Card scanning takes < 100ms — almost instant.
- 💰 **Affordable:** Total cost under ₹700 including RFID cards.
- 🔧 **Easy Card Management:** Add/remove authorized cards by updating the UID array.

---

## 🔮 Future Scope

- 🧬 **Biometric Integration:** Add a fingerprint sensor (R307) for dual-factor authentication (RFID + fingerprint).
- 📹 **Camera Logging:** Integrate ESP32-CAM to capture photos of every person attempting access.
- 🔢 **Keypad Backup:** Add a 4×4 matrix keypad as a fallback PIN entry method.
- 🗣️ **Voice Control:** Integrate with Google Assistant for voice-activated lock/unlock.
- 📊 **Analytics Dashboard:** Build a web dashboard showing access patterns, peak hours, and anomalies.
- 🔋 **Battery Backup:** Add a LiPo battery for operation during power outages.
- 🏢 **Multi-Door System:** Scale to control multiple doors from a single Blynk dashboard.
- 📡 **BLE Proximity:** Add Bluetooth BLE for proximity-based auto-unlock using smartphones.
- 🤖 **Face Recognition:** Upgrade to ESP32-CAM with on-device face recognition for contactless entry.
- 🔐 **Encrypted Communication:** Implement AES encryption between RFID card and reader for enhanced security.

---

## 📸 Expected Output

- **OLED:** Shows `SCAN YOUR CARD` → `ACCESS GRANTED ✓` or `ACCESS DENIED ✗` with UID.
- **Servo:** Rotates 90° (unlock) on authorized card, stays at 0° (locked) otherwise.
- **Buzzer:** Pleasant beep on success, harsh buzz on failure.
- **Serial Monitor:** Displays UID of every scanned card and authentication result.
- **Blynk App:** Real-time access log and remote lock/unlock button.

---

## 🔙 [Back to Main Page](../README.md)
