/*
 * =========================================================================
 *  Project 03: Smart Home Automation System
 *  Board: NodeMCU ESP8266 (ESP-12E)
 *  Modules: 4-Channel Relay, IR Receiver (TSOP1738)
 *  App: Blynk IoT
 * =========================================================================
 *  Description:
 *  Control 4 home appliances via Blynk app, manual push buttons, or
 *  IR remote. Features hybrid control with state synchronization.
 * =========================================================================
 */

#define BLYNK_TEMPLATE_ID   "TMPL_XXXXXX"
#define BLYNK_TEMPLATE_NAME "Smart Home"
#define BLYNK_AUTH_TOKEN     "YOUR_BLYNK_AUTH_TOKEN"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <IRrecv.h>
#include <IRutils.h>

// ======================== WI-FI CREDENTIALS =====================
char ssid[] = "YOUR_WIFI_SSID";
char pass[] = "YOUR_WIFI_PASSWORD";

// ======================== PIN DEFINITIONS ========================
#define RELAY_1       D0      // GPIO16 — Appliance 1
#define RELAY_2       D1      // GPIO5  — Appliance 2
#define RELAY_3       D2      // GPIO4  — Appliance 3
#define RELAY_4       D5      // GPIO14 — Appliance 4

#define SWITCH_1      10      // GPIO10 (SD3) — Manual switch 1
#define SWITCH_2      9       // GPIO9  (SD2) — Manual switch 2
#define SWITCH_3      D6      // GPIO12 — Manual switch 3
#define SWITCH_4      D7      // GPIO13 — Manual switch 4

#define IR_RECV_PIN   D3      // GPIO0 — IR Receiver

// ======================== IR REMOTE CODES ========================
// Replace these with your remote's decoded values (use IRrecvDemo)
#define IR_BTN_1      0xFFA25D
#define IR_BTN_2      0xFF629D
#define IR_BTN_3      0xFFE21D
#define IR_BTN_4      0xFF22DD

// ======================== OBJECTS ================================
IRrecv irrecv(IR_RECV_PIN);
decode_results results;

// ======================== STATE VARIABLES ========================
bool relayState[4] = {HIGH, HIGH, HIGH, HIGH};   // HIGH = OFF (active LOW)
bool lastSwitchState[4] = {HIGH, HIGH, HIGH, HIGH};

int relayPins[4]  = {RELAY_1, RELAY_2, RELAY_3, RELAY_4};
int switchPins[4] = {SWITCH_1, SWITCH_2, SWITCH_3, SWITCH_4};

// ======================== BLYNK VIRTUAL PIN HANDLERS =============
// These functions are called when buttons are toggled in the Blynk app

BLYNK_WRITE(V1) {
  relayState[0] = param.asInt() ? LOW : HIGH;
  digitalWrite(RELAY_1, relayState[0]);
  Serial.println(relayState[0] == LOW ? "Relay 1: ON" : "Relay 1: OFF");
}

BLYNK_WRITE(V2) {
  relayState[1] = param.asInt() ? LOW : HIGH;
  digitalWrite(RELAY_2, relayState[1]);
  Serial.println(relayState[1] == LOW ? "Relay 2: ON" : "Relay 2: OFF");
}

BLYNK_WRITE(V3) {
  relayState[2] = param.asInt() ? LOW : HIGH;
  digitalWrite(RELAY_3, relayState[2]);
  Serial.println(relayState[2] == LOW ? "Relay 3: ON" : "Relay 3: OFF");
}

BLYNK_WRITE(V4) {
  relayState[3] = param.asInt() ? LOW : HIGH;
  digitalWrite(RELAY_4, relayState[3]);
  Serial.println(relayState[3] == LOW ? "Relay 4: ON" : "Relay 4: OFF");
}

// ======================== SETUP ==================================
void setup() {
  Serial.begin(115200);
  Serial.println("\n--- Smart Home Automation System ---");

  // Initialize relay pins as OUTPUT
  for (int i = 0; i < 4; i++) {
    pinMode(relayPins[i], OUTPUT);
    digitalWrite(relayPins[i], HIGH);   // All relays OFF initially
  }

  // Initialize switch pins as INPUT with pull-up
  for (int i = 0; i < 4; i++) {
    pinMode(switchPins[i], INPUT_PULLUP);
  }

  // Initialize IR receiver
  irrecv.enableIRIn();
  Serial.println("IR Receiver initialized.");

  // Connect to Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  Serial.println("Connected to Blynk!");
}

// ======================== MAIN LOOP ==============================
void loop() {
  Blynk.run();           // Handle Blynk communication
  handleManualSwitches(); // Check physical push buttons
  handleIRRemote();       // Check for IR signals
}

// ======================== MANUAL SWITCH HANDLER ==================
void handleManualSwitches() {
  for (int i = 0; i < 4; i++) {
    bool currentState = digitalRead(switchPins[i]);

    // Detect button press (HIGH → LOW transition with debounce)
    if (currentState == LOW && lastSwitchState[i] == HIGH) {
      delay(50);  // Simple debounce
      currentState = digitalRead(switchPins[i]);
      if (currentState == LOW) {
        // Toggle relay state
        relayState[i] = (relayState[i] == HIGH) ? LOW : HIGH;
        digitalWrite(relayPins[i], relayState[i]);

        // Sync state back to Blynk app
        Blynk.virtualWrite(V1 + i, relayState[i] == LOW ? 1 : 0);

        Serial.print("Switch ");
        Serial.print(i + 1);
        Serial.println(relayState[i] == LOW ? ": Toggled ON" : ": Toggled OFF");
      }
    }
    lastSwitchState[i] = currentState;
  }
}

// ======================== IR REMOTE HANDLER =======================
void handleIRRemote() {
  if (irrecv.decode(&results)) {
    unsigned long code = results.value;
    Serial.print("IR Code Received: 0x");
    Serial.println(code, HEX);

    int relayIndex = -1;
    switch (code) {
      case IR_BTN_1: relayIndex = 0; break;
      case IR_BTN_2: relayIndex = 1; break;
      case IR_BTN_3: relayIndex = 2; break;
      case IR_BTN_4: relayIndex = 3; break;
    }

    if (relayIndex >= 0) {
      relayState[relayIndex] = (relayState[relayIndex] == HIGH) ? LOW : HIGH;
      digitalWrite(relayPins[relayIndex], relayState[relayIndex]);
      Blynk.virtualWrite(V1 + relayIndex, relayState[relayIndex] == LOW ? 1 : 0);

      Serial.print("IR → Relay ");
      Serial.print(relayIndex + 1);
      Serial.println(relayState[relayIndex] == LOW ? ": ON" : ": OFF");
    }

    irrecv.resume();   // Ready for next signal
  }
}
