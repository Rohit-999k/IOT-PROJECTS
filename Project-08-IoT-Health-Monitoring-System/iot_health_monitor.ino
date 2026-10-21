/*
 * =========================================================================
 *  Project 08: IoT Health Monitoring System
 *  Board: NodeMCU ESP8266 (ESP-12E)
 *  Sensors: Pulse Sensor + LM35 Temperature Sensor
 *  Display: 0.96" OLED SSD1306
 *  Cloud: ThingSpeak
 * =========================================================================
 *  Description:
 *  Monitors heart rate (BPM) and body temperature, displays on OLED,
 *  uploads to ThingSpeak, and alerts for abnormal readings.
 * =========================================================================
 */

#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ======================== PIN DEFINITIONS ========================
#define PULSE_PIN     A0        // Pulse sensor analog pin
#define BUZZER_PIN    D5        // Alert buzzer (GPIO14)
#define LED_PIN       D6        // Heartbeat LED indicator (GPIO12)

// ======================== OLED SETTINGS =========================
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ======================== WI-FI CREDENTIALS =====================
const char* ssid     = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// ======================== THINGSPEAK CONFIG ======================
unsigned long channelID  = 0000000;
const char* writeAPIKey  = "YOUR_API_KEY";

// ======================== OBJECTS ================================
WiFiClient client;

// ======================== PULSE VARIABLES ========================
int pulseSignal       = 0;          // Raw analog value from pulse sensor
int BPM               = 0;          // Calculated beats per minute
int beatThreshold      = 550;       // Threshold for detecting a beat
bool beatDetected      = false;     // Flag for beat detection
unsigned long lastBeat = 0;         // Time of last detected beat
int beatCount          = 0;         // Number of beats in measurement window
unsigned long measureStart = 0;     // Start of measurement window

// ======================== TEMPERATURE ============================
float bodyTemp         = 0.0;       // Body temperature in °C
// Simulated temperature for demo (replace with actual LM35 reading
// if using external ADC like ADS1115)
float simulatedTemp    = 36.5;

// ======================== TIMING =================================
unsigned long lastUpload    = 0;
const long    uploadInterval = 15000;   // Upload every 15 seconds
unsigned long lastDisplay   = 0;
const long    displayInterval = 1000;   // Update display every second

// ======================== NORMAL RANGES ==========================
#define BPM_LOW       50
#define BPM_HIGH      120
#define TEMP_LOW      35.0
#define TEMP_HIGH     38.0

// ======================== SETUP ==================================
void setup() {
  Serial.begin(115200);
  Serial.println("\n--- IoT Health Monitoring System ---");

  // Pin setup
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("ERROR: OLED not found!");
    while (true);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(15, 20);
  display.println("Health Monitor");
  display.setCursor(15, 35);
  display.println("Starting...");
  display.display();

  // Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected!");

  // ThingSpeak
  ThingSpeak.begin(client);

  measureStart = millis();
  lastBeat = millis();

  display.clearDisplay();
  display.setCursor(20, 25);
  display.println("Place finger");
  display.setCursor(20, 40);
  display.println("on sensor...");
  display.display();
  delay(2000);
}

// ======================== MAIN LOOP ==============================
void loop() {
  unsigned long currentMillis = millis();

  // ---- Read pulse sensor ----
  pulseSignal = analogRead(PULSE_PIN);

  // Simple beat detection
  if (pulseSignal > beatThreshold && !beatDetected) {
    beatDetected = true;
    beatCount++;
    digitalWrite(LED_PIN, HIGH);

    // Calculate instantaneous BPM from inter-beat interval
    unsigned long ibi = currentMillis - lastBeat;
    lastBeat = currentMillis;
    if (ibi > 300 && ibi < 2000) {   // Valid range: 30-200 BPM
      BPM = 60000 / ibi;
    }
  }

  if (pulseSignal < beatThreshold - 50) {
    beatDetected = false;
    digitalWrite(LED_PIN, LOW);
  }

  // ---- Calculate average BPM every 10 seconds ----
  if (currentMillis - measureStart >= 10000) {
    if (beatCount > 0) {
      BPM = beatCount * 6;   // beats in 10 sec × 6 = BPM
    }
    beatCount = 0;
    measureStart = currentMillis;
  }

  // ---- Read temperature ----
  // For demo: simulated body temperature with slight variation
  // Replace with actual LM35 reading using external ADC
  bodyTemp = simulatedTemp + random(-5, 5) * 0.1;

  // ---- Update display every second ----
  if (currentMillis - lastDisplay >= displayInterval) {
    lastDisplay = currentMillis;
    updateDisplay();
    checkAlerts();

    Serial.print("Pulse Signal: "); Serial.print(pulseSignal);
    Serial.print(" | BPM: "); Serial.print(BPM);
    Serial.print(" | Temp: "); Serial.print(bodyTemp, 1);
    Serial.println(" °C");
  }

  // ---- Upload to ThingSpeak ----
  if (currentMillis - lastUpload >= uploadInterval) {
    lastUpload = currentMillis;

    ThingSpeak.setField(1, BPM);
    ThingSpeak.setField(2, bodyTemp);

    int response = ThingSpeak.writeFields(channelID, writeAPIKey);
    Serial.println(response == 200 ? "ThingSpeak: OK ✓" : "ThingSpeak: Error");
  }
}

// ======================== DISPLAY UPDATE =========================
void updateDisplay() {
  display.clearDisplay();

  // Title
  display.setTextSize(1);
  display.setCursor(15, 0);
  display.println("HEALTH MONITOR");
  display.drawLine(0, 9, 128, 9, SSD1306_WHITE);

  // Heart icon and BPM
  display.setTextSize(1);
  display.setCursor(0, 14);
  display.print("BPM: ");
  display.setTextSize(2);
  display.setCursor(35, 12);
  if (BPM > 0 && BPM < 250) {
    display.print(BPM);
  } else {
    display.print("--");
  }

  // Temperature
  display.setTextSize(1);
  display.setCursor(0, 34);
  display.print("Temp:");
  display.setTextSize(2);
  display.setCursor(35, 32);
  display.print(bodyTemp, 1);
  display.setTextSize(1);
  display.print("C");

  // Status bar
  display.drawLine(0, 52, 128, 52, SSD1306_WHITE);
  display.setCursor(0, 55);
  if (BPM >= BPM_LOW && BPM <= BPM_HIGH && bodyTemp >= TEMP_LOW && bodyTemp <= TEMP_HIGH) {
    display.print("Status: NORMAL");
  } else {
    display.print("!! ALERT !!");
  }

  display.display();
}

// ======================== ALERT CHECK ============================
void checkAlerts() {
  bool alert = false;

  if (BPM > 0 && (BPM < BPM_LOW || BPM > BPM_HIGH)) {
    alert = true;
    Serial.println("ALERT: Abnormal heart rate!");
  }
  if (bodyTemp > TEMP_HIGH) {
    alert = true;
    Serial.println("ALERT: High body temperature!");
  }
  if (bodyTemp < TEMP_LOW && bodyTemp > 0) {
    alert = true;
    Serial.println("ALERT: Low body temperature!");
  }

  if (alert) {
    // Sound buzzer in pattern
    for (int i = 0; i < 3; i++) {
      digitalWrite(BUZZER_PIN, HIGH);
      delay(100);
      digitalWrite(BUZZER_PIN, LOW);
      delay(100);
    }
  } else {
    digitalWrite(BUZZER_PIN, LOW);
  }
}
