/*
 * =========================================================================
 *  Project 09: Smart Street Light System
 *  Board: Arduino UNO
 *  Sensors: LDR (Photoresistor) + 2x PIR Motion Sensor (HC-SR501)
 *  Output: 3x PWM-controlled LEDs (Street Lights) + LCD
 * =========================================================================
 *  Description:
 *  Energy-efficient street lighting that turns on at night, dims when
 *  no motion detected, and brightens on motion detection using PWM.
 * =========================================================================
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ======================== PIN DEFINITIONS ========================
#define LDR_PIN       A0        // LDR analog input (voltage divider)
#define PIR_1_PIN     2         // PIR sensor 1 (Zone 1)
#define PIR_2_PIN     3         // PIR sensor 2 (Zone 2)
#define LED_1_PIN     9         // Street light 1 (PWM)
#define LED_2_PIN     10        // Street light 2 (PWM) — shared zone
#define LED_3_PIN     11        // Street light 3 (PWM)

// ======================== CONSTANTS =============================
#define LDR_THRESHOLD     400   // Below this = Dark (Night)
#define BRIGHTNESS_FULL   255   // 100% brightness (PWM)
#define BRIGHTNESS_DIM    75    // ~30% brightness (PWM)
#define BRIGHTNESS_OFF    0     // 0% brightness
#define MOTION_TIMEOUT    10000 // 10 seconds after last motion

// ======================== LCD ==================================
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ======================== VARIABLES ==============================
unsigned long lastMotion_Z1 = 0;    // Last motion time - Zone 1
unsigned long lastMotion_Z2 = 0;    // Last motion time - Zone 2
int currentBrightness[3] = {0, 0, 0};
int targetBrightness[3]  = {0, 0, 0};
int ledPins[3] = {LED_1_PIN, LED_2_PIN, LED_3_PIN};
bool isNight = false;

// ======================== SETUP ==================================
void setup() {
  Serial.begin(9600);
  Serial.println("--- Smart Street Light System ---");

  // Pin modes
  pinMode(PIR_1_PIN, INPUT);
  pinMode(PIR_2_PIN, INPUT);
  pinMode(LED_1_PIN, OUTPUT);
  pinMode(LED_2_PIN, OUTPUT);
  pinMode(LED_3_PIN, OUTPUT);

  // All LEDs OFF initially
  analogWrite(LED_1_PIN, 0);
  analogWrite(LED_2_PIN, 0);
  analogWrite(LED_3_PIN, 0);

  // LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Smart Street");
  lcd.setCursor(0, 1);
  lcd.print("Light System");
  delay(2000);

  Serial.println("System ready. Monitoring light and motion...");
}

// ======================== MAIN LOOP ==============================
void loop() {
  unsigned long currentMillis = millis();

  // ---- Read LDR (ambient light) ----
  int ldrValue = analogRead(LDR_PIN);
  isNight = (ldrValue < LDR_THRESHOLD);

  // ---- Read PIR sensors ----
  bool motion_Z1 = digitalRead(PIR_1_PIN);
  bool motion_Z2 = digitalRead(PIR_2_PIN);

  // Update last motion timestamps
  if (motion_Z1) lastMotion_Z1 = currentMillis;
  if (motion_Z2) lastMotion_Z2 = currentMillis;

  // ---- Determine target brightness ----
  if (!isNight) {
    // DAYTIME: All lights OFF
    targetBrightness[0] = BRIGHTNESS_OFF;
    targetBrightness[1] = BRIGHTNESS_OFF;
    targetBrightness[2] = BRIGHTNESS_OFF;
  } else {
    // NIGHTTIME: Adaptive lighting

    // Zone 1 (LED 1)
    if (currentMillis - lastMotion_Z1 < MOTION_TIMEOUT) {
      targetBrightness[0] = BRIGHTNESS_FULL;
    } else {
      targetBrightness[0] = BRIGHTNESS_DIM;
    }

    // Zone 2 (LED 3)
    if (currentMillis - lastMotion_Z2 < MOTION_TIMEOUT) {
      targetBrightness[2] = BRIGHTNESS_FULL;
    } else {
      targetBrightness[2] = BRIGHTNESS_DIM;
    }

    // Shared LED 2 (brightest of both zones)
    if (currentMillis - lastMotion_Z1 < MOTION_TIMEOUT ||
        currentMillis - lastMotion_Z2 < MOTION_TIMEOUT) {
      targetBrightness[1] = BRIGHTNESS_FULL;
    } else {
      targetBrightness[1] = BRIGHTNESS_DIM;
    }
  }

  // ---- Smooth brightness transition ----
  for (int i = 0; i < 3; i++) {
    if (currentBrightness[i] < targetBrightness[i]) {
      currentBrightness[i] = min(currentBrightness[i] + 5, targetBrightness[i]);
    } else if (currentBrightness[i] > targetBrightness[i]) {
      currentBrightness[i] = max(currentBrightness[i] - 5, targetBrightness[i]);
    }
    analogWrite(ledPins[i], currentBrightness[i]);
  }

  // ---- Update LCD ----
  lcd.clear();
  lcd.setCursor(0, 0);
  if (!isNight) {
    lcd.print("Mode: DAY | OFF");
  } else if (motion_Z1 || motion_Z2) {
    lcd.print("Mode: NIGHT|FULL");
  } else {
    lcd.print("Mode: NIGHT|DIM ");
  }

  lcd.setCursor(0, 1);
  lcd.print("L:");
  lcd.print(ldrValue);
  lcd.print(" Z1:");
  lcd.print(motion_Z1 ? "M" : "-");
  lcd.print(" Z2:");
  lcd.print(motion_Z2 ? "M" : "-");

  // ---- Serial output ----
  Serial.print("LDR: ");
  Serial.print(ldrValue);
  Serial.print(" | Night: ");
  Serial.print(isNight ? "YES" : "NO");
  Serial.print(" | PIR1: ");
  Serial.print(motion_Z1 ? "MOTION" : "idle");
  Serial.print(" | PIR2: ");
  Serial.print(motion_Z2 ? "MOTION" : "idle");
  Serial.print(" | LED PWM: ");
  Serial.print(currentBrightness[0]);
  Serial.print("/");
  Serial.print(currentBrightness[1]);
  Serial.print("/");
  Serial.println(currentBrightness[2]);

  delay(100);   // Small delay for smooth transitions
}
