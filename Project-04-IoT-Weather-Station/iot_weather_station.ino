/*
 * =========================================================================
 *  Project 04: IoT Weather Station
 *  Board: NodeMCU ESP8266 (ESP-12E)
 *  Sensors: DHT22 + BMP280
 *  Display: 0.96" OLED SSD1306 (I2C)
 *  Cloud: ThingSpeak
 * =========================================================================
 *  Description:
 *  Multi-parameter weather station measuring temperature, humidity,
 *  barometric pressure, and altitude with cloud logging.
 * =========================================================================
 */

#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <Wire.h>
#include <DHT.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ======================== PIN DEFINITIONS ========================
#define DHTPIN        D4        // DHT22 data pin
#define DHTTYPE       DHT22     // DHT sensor type

// ======================== DISPLAY SETTINGS ======================
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
DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP280 bmp;       // I2C mode
WiFiClient client;

// ======================== VARIABLES ==============================
unsigned long previousMillis = 0;
const long    interval       = 20000;   // 20 seconds

// ======================== SETUP ==================================
void setup() {
  Serial.begin(115200);
  Serial.println("\n--- IoT Weather Station ---");

  // Initialize DHT22
  dht.begin();

  // Initialize BMP280
  if (!bmp.begin(0x76)) {   // Try 0x77 if 0x76 doesn't work
    Serial.println("ERROR: BMP280 sensor not found!");
    Serial.println("Check wiring and I2C address.");
    while (true);
  }

  // Configure BMP280 settings
  bmp.setSampling(
    Adafruit_BMP280::MODE_NORMAL,
    Adafruit_BMP280::SAMPLING_X2,     // Temperature oversampling
    Adafruit_BMP280::SAMPLING_X16,    // Pressure oversampling
    Adafruit_BMP280::FILTER_X16,      // Filtering
    Adafruit_BMP280::STANDBY_MS_500   // Standby time
  );

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("ERROR: SSD1306 OLED not found!");
    while (true);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 20);
  display.println("Weather Station");
  display.setCursor(10, 35);
  display.println("Starting...");
  display.display();

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

  display.clearDisplay();
  display.setCursor(15, 25);
  display.println("System Ready!");
  display.display();
  delay(2000);
}

// ======================== MAIN LOOP ==============================
void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // ---- Read DHT22 ----
    float humidity    = dht.readHumidity();
    float temperature = dht.readTemperature();   // °C

    // ---- Read BMP280 ----
    float pressure    = bmp.readPressure() / 100.0F;   // Convert Pa to hPa
    float altitude    = bmp.readAltitude(1013.25);     // Sea level pressure (hPa)

    // ---- Validate DHT readings ----
    if (isnan(humidity) || isnan(temperature)) {
      Serial.println("ERROR: DHT22 read failed!");
      temperature = bmp.readTemperature();   // Fallback to BMP280 temperature
      humidity = 0;
    }

    // ---- Print to Serial ----
    Serial.println("====================================");
    Serial.print("Temperature:  "); Serial.print(temperature); Serial.println(" °C");
    Serial.print("Humidity:     "); Serial.print(humidity);    Serial.println(" %");
    Serial.print("Pressure:     "); Serial.print(pressure);   Serial.println(" hPa");
    Serial.print("Altitude:     "); Serial.print(altitude);   Serial.println(" m");

    // ---- Update OLED ----
    updateDisplay(temperature, humidity, pressure, altitude);

    // ---- Upload to ThingSpeak ----
    ThingSpeak.setField(1, temperature);
    ThingSpeak.setField(2, humidity);
    ThingSpeak.setField(3, pressure);
    ThingSpeak.setField(4, altitude);

    int response = ThingSpeak.writeFields(channelID, writeAPIKey);
    if (response == 200) {
      Serial.println("ThingSpeak: Upload OK ✓");
    } else {
      Serial.print("ThingSpeak Error: ");
      Serial.println(response);
    }
  }
}

// ======================== DISPLAY UPDATE =========================
void updateDisplay(float temp, float hum, float pres, float alt) {
  display.clearDisplay();

  // Title bar
  display.setTextSize(1);
  display.setCursor(20, 0);
  display.println("WEATHER STATION");
  display.drawLine(0, 9, 128, 9, SSD1306_WHITE);

  // Temperature
  display.setCursor(0, 14);
  display.print("Temp:  ");
  display.print(temp, 1);
  display.println(" C");

  // Humidity
  display.setCursor(0, 26);
  display.print("Humid: ");
  display.print(hum, 1);
  display.println(" %");

  // Pressure
  display.setCursor(0, 38);
  display.print("Press: ");
  display.print(pres, 1);
  display.println(" hPa");

  // Altitude
  display.setCursor(0, 50);
  display.print("Alt:   ");
  display.print(alt, 1);
  display.println(" m");

  display.display();
}
