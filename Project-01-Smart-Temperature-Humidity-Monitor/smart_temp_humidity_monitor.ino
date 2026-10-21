/*
 * =========================================================================
 *  Project 01: Smart Temperature & Humidity Monitor
 *  Board: NodeMCU ESP8266 (ESP-12E)
 *  Sensor: DHT11
 *  Display: 0.96" OLED SSD1306 (I2C)
 *  Cloud: ThingSpeak
 * =========================================================================
 *  Description:
 *  Reads temperature and humidity from DHT11 sensor, displays on OLED,
 *  uploads data to ThingSpeak cloud, and triggers buzzer/LED alerts
 *  when temperature exceeds threshold.
 * =========================================================================
 */

#include <ESP8266WiFi.h>
#include <DHT.h>
#include <ThingSpeak.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ======================== PIN DEFINITIONS ========================
#define DHTPIN        D4        // DHT11 data pin connected to D4 (GPIO2)
#define DHTTYPE       DHT11     // DHT sensor type
#define BUZZER_PIN    D5        // Buzzer connected to D5 (GPIO14)
#define LED_PIN       D6        // Red LED connected to D6 (GPIO12)

// ======================== OLED SETTINGS =========================
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1        // No reset pin
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ======================== WI-FI CREDENTIALS =====================
const char* ssid     = "YOUR_WIFI_SSID";       // Replace with your Wi-Fi SSID
const char* password = "YOUR_WIFI_PASSWORD";    // Replace with your Wi-Fi Password

// ======================== THINGSPEAK CONFIG ======================
unsigned long channelID    = 0000000;           // Replace with your Channel ID
const char*  writeAPIKey   = "YOUR_API_KEY";    // Replace with your Write API Key

// ======================== THRESHOLD ==============================
#define TEMP_THRESHOLD  40.0    // Temperature alert threshold in °C

// ======================== OBJECTS ================================
DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;

// ======================== VARIABLES ==============================
unsigned long previousMillis = 0;
const long    interval       = 15000;   // Send data every 15 seconds

// ======================== SETUP ==================================
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  Serial.println("\n--- Smart Temperature & Humidity Monitor ---");

  // Initialize pins
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_PIN, LOW);

  // Initialize DHT sensor
  dht.begin();

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("ERROR: SSD1306 OLED not found!"));
    while (true);   // Halt if display not found
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 20);
  display.println("Connecting to");
  display.setCursor(10, 35);
  display.println("Wi-Fi...");
  display.display();

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Initialize ThingSpeak
  ThingSpeak.begin(client);

  // Display success
  display.clearDisplay();
  display.setCursor(10, 20);
  display.println("Wi-Fi Connected!");
  display.setCursor(10, 40);
  display.print("IP: ");
  display.println(WiFi.localIP());
  display.display();
  delay(2000);
}

// ======================== MAIN LOOP ==============================
void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Read sensor data
    float humidity    = dht.readHumidity();
    float temperature = dht.readTemperature();    // Celsius

    // Validate readings
    if (isnan(humidity) || isnan(temperature)) {
      Serial.println("ERROR: Failed to read from DHT11 sensor!");
      displayError("Sensor Error!");
      return;
    }

    // Print to Serial Monitor
    Serial.println("---------------------------");
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");
    Serial.print("Humidity:    ");
    Serial.print(humidity);
    Serial.println(" %");

    // Update OLED Display
    updateOLED(temperature, humidity);

    // Check threshold and trigger alert
    if (temperature > TEMP_THRESHOLD) {
      Serial.println("⚠ ALERT: Temperature exceeded threshold!");
      digitalWrite(BUZZER_PIN, HIGH);
      digitalWrite(LED_PIN, HIGH);
    } else {
      digitalWrite(BUZZER_PIN, LOW);
      digitalWrite(LED_PIN, LOW);
    }

    // Upload to ThingSpeak
    ThingSpeak.setField(1, temperature);
    ThingSpeak.setField(2, humidity);
    int responseCode = ThingSpeak.writeFields(channelID, writeAPIKey);

    if (responseCode == 200) {
      Serial.println("ThingSpeak: Data sent successfully ✓");
    } else {
      Serial.print("ThingSpeak Error: ");
      Serial.println(responseCode);
    }
  }
}

// ======================== OLED UPDATE FUNCTION ===================
void updateOLED(float temp, float hum) {
  display.clearDisplay();

  // Title
  display.setTextSize(1);
  display.setCursor(15, 0);
  display.println("ENVIRONMENT MONITOR");
  display.drawLine(0, 10, 128, 10, SSD1306_WHITE);

  // Temperature
  display.setTextSize(1);
  display.setCursor(0, 16);
  display.print("Temp:");
  display.setTextSize(2);
  display.setCursor(40, 14);
  display.print(temp, 1);
  display.setTextSize(1);
  display.print(" C");

  // Humidity
  display.setTextSize(1);
  display.setCursor(0, 38);
  display.print("Hum:");
  display.setTextSize(2);
  display.setCursor(40, 36);
  display.print(hum, 1);
  display.setTextSize(1);
  display.print(" %");

  // Status bar
  display.drawLine(0, 54, 128, 54, SSD1306_WHITE);
  display.setCursor(0, 56);
  if (temp > TEMP_THRESHOLD) {
    display.print("!! HIGH TEMP ALERT !!");
  } else {
    display.print("Status: Normal");
  }

  display.display();
}

// ======================== ERROR DISPLAY ==========================
void displayError(const char* message) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(10, 25);
  display.println(message);
  display.display();
}
