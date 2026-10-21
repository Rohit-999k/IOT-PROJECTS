# Project 03: Smart Home Automation System 🏠🔌

<p align="center">
  <img src="https://img.shields.io/badge/Board-NodeMCU%20ESP8266-blue?style=flat-square&logo=arduino" />
  <img src="https://img.shields.io/badge/Module-4Ch%20Relay-green?style=flat-square" />
  <img src="https://img.shields.io/badge/App-Blynk%20IoT-orange?style=flat-square" />
  <img src="https://img.shields.io/badge/Status-Complete-brightgreen?style=flat-square" />
</p>

## 📋 Project Description

This project implements a **Wi-Fi-based Smart Home Automation System** using **NodeMCU ESP8266** and a **4-channel relay module**. Users can control up to **4 home appliances** (lights, fans, etc.) remotely via the **Blynk IoT mobile app** from anywhere in the world. The system also features **manual override switches** and an **IR remote sensor** for local control without internet, making it a truly hybrid automation solution.

---

## 🧰 Components Required

| # | Component | Quantity | Specification |
|---|-----------|----------|---------------|
| 1 | NodeMCU ESP8266 (ESP-12E) | 1 | Wi-Fi microcontroller |
| 2 | 4-Channel Relay Module | 1 | 5V relay, active LOW |
| 3 | IR Receiver Module (TSOP1738) | 1 | 38kHz IR receiver |
| 4 | IR Remote Control | 1 | Any standard IR remote |
| 5 | Push Buttons | 4 | Tactile switches for manual control |
| 6 | LED Indicators | 4 | 5mm (represent appliances) |
| 7 | Resistors (220Ω) | 4 | For LEDs |
| 8 | Resistors (10kΩ) | 4 | Pull-up for push buttons |
| 9 | Breadboard | 1 | Full-size |
| 10 | Jumper Wires | ~30 | Assorted |
| 11 | Micro USB Cable | 1 | Programming & power |
| 12 | 5V Power Supply | 1 | For relay module |

---

## 🔌 Circuit Diagram

```
                          +---------------------+
                          |    NodeMCU ESP8266   |
                          |                     |
                          |                     |       +------------------+
    +----------+          |                     |       | 4-Channel Relay  |
    | IR Recv  |          |                     |       |                  |
    | TSOP1738 |          |                     |       |  IN1 <-----------| D0
    |  OUT ----+--------->| D3 (GPIO0)          |       |  IN2 <-----------| D1*
    |  VCC ----+--------->| 3V3           D0 ---|------>|  IN3 <-----------| D2*
    |  GND ----+--------->| GND           D1 ---|------>|  IN4 <-----------| D5
    +----------+          |               D2 ---|------>|  VCC  ---- 5V    |
                          |               D5 ---|------>|  GND  ---- GND   |
    Manual Switches:      |                     |       +------------------+
    SW1 ----[10kΩ]------->| SD3 (GPIO10)        |
    SW2 ----[10kΩ]------->| SD2 (GPIO9)         |       * D1, D2 shared with
    SW3 ----[10kΩ]------->| D6 (GPIO12)         |         I2C when LCD used
    SW4 ----[10kΩ]------->| D7 (GPIO13)         |
                          |                     |
    LED1 ---[220Ω]------->| (Relay 1 Output)    |
    LED2 ---[220Ω]------->| (Relay 2 Output)    |
    LED3 ---[220Ω]------->| (Relay 3 Output)    |
    LED4 ---[220Ω]------->| (Relay 4 Output)    |
                          +---------------------+

    Note: In actual deployment, LEDs are replaced with AC
    appliances connected via relay contacts (NO/COM).
```

---

## 🔗 Connection Details

### 4-Channel Relay Module → NodeMCU

| Relay Pin | NodeMCU Pin | Appliance |
|-----------|-------------|-----------|
| IN1 | D0 (GPIO16) | Appliance 1 (e.g., Light 1) |
| IN2 | D1 (GPIO5) | Appliance 2 (e.g., Light 2) |
| IN3 | D2 (GPIO4) | Appliance 3 (e.g., Fan) |
| IN4 | D5 (GPIO14) | Appliance 4 (e.g., Socket) |
| VCC | External 5V | Relay power supply |
| GND | GND (common) | Ground |

> **Note:** The relay module is **active LOW** — sending LOW turns the relay ON, HIGH turns it OFF.

### IR Receiver (TSOP1738) → NodeMCU

| IR Pin | NodeMCU Pin | Description |
|--------|-------------|-------------|
| OUT | D3 (GPIO0) | IR signal data |
| VCC | 3V3 | Power |
| GND | GND | Ground |

### Manual Push Buttons → NodeMCU

| Button | NodeMCU Pin | Function |
|--------|-------------|----------|
| SW1 | SD3 (GPIO10) | Toggle Appliance 1 |
| SW2 | SD2 (GPIO9) | Toggle Appliance 2 |
| SW3 | D6 (GPIO12) | Toggle Appliance 3 |
| SW4 | D7 (GPIO13) | Toggle Appliance 4 |

> Each button is connected with a **10kΩ pull-up resistor** to 3V3.

---

## 📚 Required Libraries

| Library | Author | Purpose |
|---------|--------|---------|
| `BlynkSimpleEsp8266` | Blynk Inc. | Blynk IoT platform integration |
| `ESP8266WiFi` | ESP8266 Community | Wi-Fi connectivity |
| `IRremoteESP8266` | David Conran | IR remote decoding |

---

## 💻 Source Code

📄 **File:** [`smart_home_automation.ino`](./smart_home_automation.ino)

---

## ⚙️ How It Works

1. **Initialization:** NodeMCU connects to Wi-Fi and the Blynk cloud server, then initializes all relay pins, button inputs, and the IR receiver.
2. **Blynk App Control:** Virtual pins (V1–V4) in the Blynk app are mapped to the 4 relay channels. Toggling a button in the app sends a command to NodeMCU, which switches the corresponding relay.
3. **Manual Override:** Physical push buttons allow local control even when Wi-Fi or the app is unavailable. The button state is synced back to Blynk.
4. **IR Remote Control:** Pre-programmed IR codes from a standard remote are decoded to toggle specific relays. The first run uses `IRrecvDemo` to capture button codes.
5. **State Synchronization:** All control methods (app, button, IR) keep the relay states synchronized across the system.

---

## 📱 Blynk App Setup

1. Download **Blynk IoT** from Google Play / App Store.
2. Create a new **Template** and **Device**.
3. Add **4 Button widgets** mapped to Virtual Pins **V1, V2, V3, V4**.
4. Set button mode to **Switch** (not Push).
5. Copy the **Template ID**, **Device Name**, and **Auth Token** into the code.

---

## ✅ Advantages

- 📱 **Global Control:** Operate home appliances from anywhere using the Blynk app.
- 🔘 **Hybrid Control:** Supports App + Manual Switches + IR Remote — works even without internet.
- ⚡ **Low Latency:** Near-instant response when toggling appliances.
- 🔌 **Scalable:** Can be expanded to 8 or 16 channels with additional relay modules.
- 💡 **Energy Saving:** Schedule appliances to turn off automatically, reducing power consumption.
- 🛡️ **Safe Operation:** Relay provides electrical isolation between the microcontroller and high-voltage AC appliances.
- 💰 **Cost Effective:** Complete system costs under ₹800, far cheaper than commercial smart home kits.

---

## 🔮 Future Scope

- 🗣️ **Voice Control:** Integrate with Google Assistant or Alexa using IFTTT or Sinric Pro for hands-free operation.
- ⏰ **Scheduling & Timers:** Add automated time-based schedules (e.g., turn lights on at sunset).
- 📊 **Energy Monitoring:** Add ACS712 current sensors to monitor power consumption of each appliance.
- 🌡️ **Sensor Automation:** Integrate temperature/humidity sensors to auto-control fans and ACs.
- 🔒 **OTA Updates:** Implement Over-The-Air firmware updates for maintenance without physical access.
- 🏠 **Home Dashboard:** Create a web-based dashboard hosted on NodeMCU for local network control.
- 🔄 **Scene Modes:** Implement preset scenes ("Movie Night", "Sleep", "Morning") to control multiple appliances simultaneously.
- 🧠 **AI Integration:** Use occupancy patterns and ML to automate appliances intelligently.

---

## 📸 Expected Output

- **Blynk App:** 4 toggle buttons to control appliances with real-time status.
- **Serial Monitor:** Logs connection status, button presses, IR codes, and relay states.
- **Physical:** Relay clicks ON/OFF, appliances (or demo LEDs) turn on/off accordingly.

---

## 🔙 [Back to Main Page](../README.md)
