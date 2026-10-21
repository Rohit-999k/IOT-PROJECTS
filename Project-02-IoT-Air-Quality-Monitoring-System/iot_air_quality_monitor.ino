/*
 * =========================================================================
 *  Project 02: IoT Air Quality Monitoring System
 *  Board: NodeMCU ESP8266 (ESP-12E)
 *  Sensor: MQ-135 Gas Sensor
 *  Display: 16x2 LCD (I2C)
 *  Cloud: ThingSpeak
 * =========================================================================
 *  Description:
 *  Reads air quality data from MQ-135 gas sensor, classifies it into
 *  AQI categories, displays on LCD, triggers LED/Buzzer alerts, and
 *  uploads data to ThingSpeak for remote monitoring.
 * =========================================================================
 */

#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ======================== PIN DEFINITIONS ========================
#define MQ135_PIN     A0        // MQ-135 analog output connected to A0
#define BUZZER_PIN    D5        // Buzzer connected to D5 (GPIO14)
#define GREEN_LED     D6        // Green LED — Good air quality
#define YELLOW_LED    D7        // Yellow LED — Moderate air quality
#define RED_LED       D8        // Red LED — Poor/Hazardous

// ======================== LCD SETTINGS ==========================
LiquidCrystal_I2C lcd(0x27, 16, 2);   // I2C address 0x27, 16 columns, 2 rows

// ======================== WI-FI CREDENTIALS =====================
const char* ssid     = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// ======================== THINGSPEAK CONFIG ======================
unsigned long channelID  = 0000000;
const char* writeAPIKey  = "YOUR_API_KEY";

// ======================== OBJECTS ================================
WiFiClient client;

// ======================== VARIABLES ==============================
unsigned long previousMillis = 0;
const long    interval       = 15000;   // 15 seconds

// ======================== AQI THRESHOLDS =========================
#define AQI_GOOD        100
#define AQI_MODERATE    200
#define AQI_UNHEALTHY   300

// ======================== SETUP ==================================
void setup() {
  Serial.begin(115200);
  Serial.println("\n--- IoT Air Quality Monitoring System ---");

  // Initialize pins
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  // Turn off all indicators
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED, LOW);

  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Air Quality Mon.");
  lcd.setCursor(0, 1);
  lcd.print("Connecting WiFi");

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi Connected!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  // Initialize ThingSpeak
  ThingSpeak.begin(client);

  // Display ready status
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi Connected!");
  lcd.setCursor(0, 1);
  lcd.print("Warming up...");

  // MQ-135 warm-up period (sensor needs 2-3 min for accuracy)
  Serial.println("MQ-135 warming up (30 seconds)...");
  delay(30000);  // Shortened for demo; ideally 2-3 minutes

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("System Ready!");
  delay(1000);
}

// ======================== MAIN LOOP ==============================
void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Read analog value from MQ-135 (0-1023)
    int sensorValue = analogRead(MQ135_PIN);

    // Map sensor value to AQI scale (approximate)
    // MQ-135 analog output: lower = cleaner air, higher = polluted
    int aqi = map(sensorValue, 0, 1023, 0, 500);

    // Determine air quality category
    String category;
    classifyAQI(aqi, category);

    // Print to Serial Monitor
    Serial.println("================================");
    Serial.print("Raw Sensor Value: ");
    Serial.println(sensorValue);
    Serial.print("AQI Value:        ");
    Serial.println(aqi);
    Serial.print("Category:         ");
    Serial.println(category);

    // Update LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("AQI: ");
    lcd.print(aqi);
    lcd.setCursor(0, 1);
    lcd.print(category);

    // Upload to ThingSpeak
    ThingSpeak.setField(1, aqi);
    ThingSpeak.setField(2, sensorValue);
    int response = ThingSpeak.writeFields(channelID, writeAPIKey);

    if (response == 200) {
      Serial.println("ThingSpeak: Upload OK ✓");
    } else {
      Serial.print("ThingSpeak Error: ");
      Serial.println(response);
    }
  }
}

// ======================== AQI CLASSIFICATION =====================
void classifyAQI(int aqi, String &category) {
  // Turn off all indicators first
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  if (aqi <= AQI_GOOD) {
    category = "Good";
    digitalWrite(GREEN_LED, HIGH);
  }
  else if (aqi <= AQI_MODERATE) {
    category = "Moderate";
    digitalWrite(YELLOW_LED, HIGH);
  }
  else if (aqi <= AQI_UNHEALTHY) {
    category = "Unhealthy";
    digitalWrite(RED_LED, HIGH);
  }
  else {
    category = "HAZARDOUS!";
    digitalWrite(RED_LED, HIGH);
    // Sound the buzzer for hazardous levels
    for (int i = 0; i < 3; i++) {
      digitalWrite(BUZZER_PIN, HIGH);
      delay(200);
      digitalWrite(BUZZER_PIN, LOW);
      delay(200);
    }
  }
}
