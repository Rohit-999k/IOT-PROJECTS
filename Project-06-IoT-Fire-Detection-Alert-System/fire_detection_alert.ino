/*
 * =========================================================================
 *  Project 06: IoT Fire Detection & Alert System
 *  Board: Arduino UNO
 *  Sensors: IR Flame Sensor, MQ-2 Smoke/Gas Sensor
 *  Output: Buzzer, LEDs, LCD, GSM (optional)
 * =========================================================================
 *  Description:
 *  Dual-sensor fire detection using flame and smoke sensors with
 *  multi-modal alerts (buzzer, LED, LCD, optional SMS via GSM).
 * =========================================================================
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

// ======================== PIN DEFINITIONS ========================
#define FLAME_DIGITAL   2       // Flame sensor digital output
#define FLAME_ANALOG    A1      // Flame sensor analog output
#define SMOKE_DIGITAL   3       // MQ-2 digital output
#define SMOKE_ANALOG    A0      // MQ-2 analog output
#define BUZZER_PIN      8       // Buzzer
#define RED_LED_1       9       // Fire alert LED 1
#define RED_LED_2       10      // Fire alert LED 2
#define GREEN_LED       11      // Normal status LED

// ======================== GSM MODULE (Optional) ==================
#define GSM_TX          7       // Arduino pin connected to SIM800L TX
#define GSM_RX          6       // Arduino pin connected to SIM800L RX
SoftwareSerial gsmSerial(GSM_TX, GSM_RX);

// ======================== LCD SETTINGS ==========================
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ======================== THRESHOLDS ============================
#define SMOKE_THRESHOLD   400   // Analog threshold for smoke detection
#define FLAME_THRESHOLD   500   // Analog threshold for flame intensity

// ======================== VARIABLES ==============================
bool smsSent = false;           // Flag to prevent repeated SMS
const char* phoneNumber = "+91XXXXXXXXXX";  // Replace with your number

// ======================== SETUP ==================================
void setup() {
  Serial.begin(9600);
  Serial.println("--- Fire Detection & Alert System ---");

  // Pin modes
  pinMode(FLAME_DIGITAL, INPUT);
  pinMode(SMOKE_DIGITAL, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RED_LED_1, OUTPUT);
  pinMode(RED_LED_2, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  // Initial state
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(RED_LED_1, LOW);
  digitalWrite(RED_LED_2, LOW);
  digitalWrite(GREEN_LED, HIGH);  // Green ON = system normal

  // LCD initialization
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Fire Detection");
  lcd.setCursor(0, 1);
  lcd.print("System Ready!");

  // GSM initialization (optional)
  gsmSerial.begin(9600);
  delay(1000);
  gsmSerial.println("AT");
  delay(500);

  Serial.println("System initialized. Warming up MQ-2...");
  Serial.println("(Wait 2-3 minutes for accurate readings)");
  delay(3000);
}

// ======================== MAIN LOOP ==============================
void loop() {
  // Read sensors
  int flameDigital  = digitalRead(FLAME_DIGITAL);   // LOW = flame detected
  int smokeDigital  = digitalRead(SMOKE_DIGITAL);   // LOW = smoke detected
  int flameAnalog   = analogRead(FLAME_ANALOG);
  int smokeAnalog   = analogRead(SMOKE_ANALOG);

  // Print to Serial Monitor
  Serial.println("-------------------------------");
  Serial.print("Flame Sensor: Digital=");
  Serial.print(flameDigital == LOW ? "FLAME" : "OK");
  Serial.print("  Analog=");
  Serial.println(flameAnalog);
  Serial.print("Smoke Sensor: Digital=");
  Serial.print(smokeDigital == LOW ? "SMOKE" : "OK");
  Serial.print("  Analog=");
  Serial.println(smokeAnalog);

  // Detection logic
  bool flameDetected = (flameDigital == LOW) || (flameAnalog < FLAME_THRESHOLD);
  bool smokeDetected = (smokeDigital == LOW) || (smokeAnalog > SMOKE_THRESHOLD);

  if (flameDetected && smokeDetected) {
    // ========= FIRE CONFIRMED =========
    Serial.println("🔥 FIRE CONFIRMED! Flame + Smoke detected!");
    fireAlarm();
    updateLCD("!! FIRE ALERT !!", "Evacuate Now!");

    // Send SMS (only once per event)
    if (!smsSent) {
      sendSMS("FIRE ALERT! Flame and Smoke detected at your location. Evacuate immediately!");
      smsSent = true;
    }
  }
  else if (flameDetected) {
    // ========= FLAME WARNING =========
    Serial.println("⚠ WARNING: Flame detected!");
    warningAlert(RED_LED_1);
    updateLCD("WARNING: Flame", "Detected!");
    smsSent = false;
  }
  else if (smokeDetected) {
    // ========= SMOKE WARNING =========
    Serial.println("⚠ WARNING: Smoke/Gas detected!");
    warningAlert(RED_LED_2);
    updateLCD("WARNING: Smoke", "Detected!");
    smsSent = false;
  }
  else {
    // ========= ALL CLEAR =========
    allClear();
    updateLCD("Status: Normal", "System Active");
    smsSent = false;
  }

  delay(500);   // Poll every 500ms
}

// ======================== ALARM FUNCTIONS ========================
void fireAlarm() {
  // Alternating red LED flash with continuous buzzer
  for (int i = 0; i < 5; i++) {
    digitalWrite(RED_LED_1, HIGH);
    digitalWrite(RED_LED_2, LOW);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(100);
    digitalWrite(RED_LED_1, LOW);
    digitalWrite(RED_LED_2, HIGH);
    delay(100);
  }
  digitalWrite(GREEN_LED, LOW);
}

void warningAlert(int ledPin) {
  // Blink specific warning LED
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(ledPin, HIGH);
  tone(BUZZER_PIN, 1000, 200);   // Short beep
  delay(300);
  digitalWrite(ledPin, LOW);
  delay(200);
}

void allClear() {
  digitalWrite(RED_LED_1, LOW);
  digitalWrite(RED_LED_2, LOW);
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(BUZZER_PIN, LOW);
  noTone(BUZZER_PIN);
}

// ======================== LCD UPDATE ============================
void updateLCD(const char* line1, const char* line2) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print(line2);
}

// ======================== GSM SMS FUNCTION =======================
void sendSMS(const char* message) {
  Serial.println("Sending SMS alert...");
  gsmSerial.println("AT+CMGF=1");    // Set SMS mode to text
  delay(500);
  gsmSerial.print("AT+CMGS=\"");
  gsmSerial.print(phoneNumber);
  gsmSerial.println("\"");
  delay(500);
  gsmSerial.print(message);
  delay(100);
  gsmSerial.write(26);   // CTRL+Z to send
  delay(3000);
  Serial.println("SMS Sent!");
}
