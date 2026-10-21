/*
 * =========================================================================
 *  Project 05: Smart Irrigation System
 *  Board: NodeMCU ESP8266 (ESP-12E)
 *  Sensors: Capacitive Soil Moisture Sensor, DHT11
 *  Actuator: Relay + Water Pump
 *  Cloud: ThingSpeak + Blynk
 * =========================================================================
 */

#define BLYNK_TEMPLATE_ID   "TMPL_XXXXXX"
#define BLYNK_TEMPLATE_NAME "Smart Irrigation"
#define BLYNK_AUTH_TOKEN     "YOUR_BLYNK_AUTH_TOKEN"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <ThingSpeak.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ======================== PIN DEFINITIONS ========================
#define SOIL_PIN      A0        // Soil moisture sensor analog pin
#define DHTPIN        D4        // DHT11 data pin
#define DHTTYPE       DHT11
#define RELAY_PIN     D5        // Relay control pin (GPIO14)
#define GREEN_LED     D6        // Pump OFF indicator (GPIO12)
#define BLUE_LED      D7        // Pump ON indicator (GPIO13)

// ======================== CALIBRATION VALUES =====================
// Calibrate these for your specific soil and sensor
#define SOIL_DRY      800       // Analog value in completely dry soil
#define SOIL_WET      300       // Analog value in saturated soil

// ======================== THRESHOLDS ============================
#define MOISTURE_LOW  30        // Below this → start pumping (%)
#define MOISTURE_HIGH 60        // Above this → stop pumping (%)

// ======================== LCD SETTINGS ==========================
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ======================== WI-FI CREDENTIALS =====================
char ssid[] = "YOUR_WIFI_SSID";
char pass[] = "YOUR_WIFI_PASSWORD";

// ======================== THINGSPEAK CONFIG ======================
unsigned long channelID  = 0000000;
const char* writeAPIKey  = "YOUR_API_KEY";

// ======================== OBJECTS ================================
DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;

// ======================== VARIABLES ==============================
unsigned long previousMillis = 0;
const long    interval       = 15000;
bool          pumpState      = false;
bool          manualOverride = false;

// ======================== BLYNK MANUAL OVERRIDE ==================
BLYNK_WRITE(V5) {
  manualOverride = true;
  int value = param.asInt();
  if (value == 1) {
    pumpON();
    Serial.println("Blynk: Manual Pump ON");
  } else {
    pumpOFF();
    Serial.println("Blynk: Manual Pump OFF");
  }
}

BLYNK_WRITE(V6) {
  // Auto mode toggle — disables manual override
  manualOverride = !param.asInt();
  Serial.println(manualOverride ? "Mode: Manual" : "Mode: Automatic");
}

// ======================== SETUP ==================================
void setup() {
  Serial.begin(115200);
  Serial.println("\n--- Smart Irrigation System ---");

  // Pin setup
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);  // Relay OFF (active LOW)
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(BLUE_LED, LOW);

  // Sensors
  dht.begin();

  // LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Smart Irrigation");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");

  // Wi-Fi & Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // ThingSpeak
  ThingSpeak.begin(client);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("System Ready!");
  delay(2000);

  Serial.println("System initialized successfully.");
}

// ======================== MAIN LOOP ==============================
void loop() {
  Blynk.run();

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Read soil moisture
    int soilRaw = analogRead(SOIL_PIN);
    int moisturePercent = map(soilRaw, SOIL_DRY, SOIL_WET, 0, 100);
    moisturePercent = constrain(moisturePercent, 0, 100);

    // Read DHT11
    float temperature = dht.readTemperature();
    float humidity    = dht.readHumidity();

    if (isnan(temperature)) temperature = 0;
    if (isnan(humidity)) humidity = 0;

    // Serial output
    Serial.println("==================================");
    Serial.print("Soil Moisture:  "); Serial.print(moisturePercent); Serial.println(" %");
    Serial.print("Raw Value:      "); Serial.println(soilRaw);
    Serial.print("Temperature:    "); Serial.print(temperature); Serial.println(" °C");
    Serial.print("Humidity:       "); Serial.print(humidity); Serial.println(" %");

    // Automatic pump control (only if not in manual override)
    if (!manualOverride) {
      if (moisturePercent < MOISTURE_LOW) {
        pumpON();
        Serial.println("AUTO: Pump ON (soil dry)");
      } else if (moisturePercent > MOISTURE_HIGH) {
        pumpOFF();
        Serial.println("AUTO: Pump OFF (soil moist)");
      }
    }

    // Update LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Moisture: ");
    lcd.print(moisturePercent);
    lcd.print("%");
    lcd.setCursor(0, 1);
    lcd.print(pumpState ? "Pump:ON " : "Pump:OFF");
    lcd.print(" ");
    lcd.print((int)temperature);
    lcd.print("C");

    // Update Blynk widgets
    Blynk.virtualWrite(V1, moisturePercent);
    Blynk.virtualWrite(V2, temperature);
    Blynk.virtualWrite(V3, humidity);
    Blynk.virtualWrite(V4, pumpState ? 1 : 0);

    // Upload to ThingSpeak
    ThingSpeak.setField(1, moisturePercent);
    ThingSpeak.setField(2, temperature);
    ThingSpeak.setField(3, humidity);
    ThingSpeak.setField(4, pumpState ? 1 : 0);

    int response = ThingSpeak.writeFields(channelID, writeAPIKey);
    Serial.println(response == 200 ? "ThingSpeak: OK ✓" : "ThingSpeak: Error");
  }
}

// ======================== PUMP CONTROL FUNCTIONS =================
void pumpON() {
  pumpState = true;
  digitalWrite(RELAY_PIN, LOW);   // Active LOW relay
  digitalWrite(BLUE_LED, HIGH);
  digitalWrite(GREEN_LED, LOW);
}

void pumpOFF() {
  pumpState = false;
  digitalWrite(RELAY_PIN, HIGH);
  digitalWrite(BLUE_LED, LOW);
  digitalWrite(GREEN_LED, HIGH);
}
