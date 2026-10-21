/*
 * =========================================================================
 *  Project 07: Smart Parking System
 *  Board: Arduino UNO
 *  Sensors: 4x HC-SR04 Ultrasonic (3 slots + 1 entry)
 *  Actuator: SG90 Servo Motor (gate barrier)
 *  Display: 16x2 LCD (I2C)
 * =========================================================================
 */

#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ======================== PIN DEFINITIONS ========================
// Entry sensor
#define ENTRY_TRIG    2
#define ENTRY_ECHO    3

// Slot sensors
#define SLOT1_TRIG    4
#define SLOT1_ECHO    5
#define SLOT2_TRIG    6
#define SLOT2_ECHO    7
#define SLOT3_TRIG    8
#define SLOT3_ECHO    9

// Servo
#define SERVO_PIN     13

// LEDs (Green = empty, Red = occupied)
#define SLOT1_GREEN   10
#define SLOT1_RED     11
#define SLOT2_GREEN   12
#define SLOT2_RED     A0    // Using analog pins as digital
#define SLOT3_GREEN   A1
#define SLOT3_RED     A2

// ======================== CONSTANTS =============================
#define SLOT_THRESHOLD  10    // Distance (cm) below which slot is occupied
#define ENTRY_THRESHOLD 30    // Distance (cm) to detect approaching vehicle
#define GATE_OPEN       90    // Servo angle for open gate
#define GATE_CLOSED     0     // Servo angle for closed gate
#define TOTAL_SLOTS     3

// ======================== OBJECTS ================================
Servo gateServo;
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ======================== VARIABLES ==============================
bool slotOccupied[TOTAL_SLOTS] = {false, false, false};
int  trigPins[TOTAL_SLOTS]     = {SLOT1_TRIG, SLOT2_TRIG, SLOT3_TRIG};
int  echoPins[TOTAL_SLOTS]     = {SLOT1_ECHO, SLOT2_ECHO, SLOT3_ECHO};
int  greenPins[TOTAL_SLOTS]    = {SLOT1_GREEN, SLOT2_GREEN, SLOT3_GREEN};
int  redPins[TOTAL_SLOTS]      = {SLOT1_RED, SLOT2_RED, SLOT3_RED};
bool gateOpen = false;

// ======================== SETUP ==================================
void setup() {
  Serial.begin(9600);
  Serial.println("--- Smart Parking System ---");

  // Entry sensor pins
  pinMode(ENTRY_TRIG, OUTPUT);
  pinMode(ENTRY_ECHO, INPUT);

  // Slot sensor and LED pins
  for (int i = 0; i < TOTAL_SLOTS; i++) {
    pinMode(trigPins[i], OUTPUT);
    pinMode(echoPins[i], INPUT);
    pinMode(greenPins[i], OUTPUT);
    pinMode(redPins[i], OUTPUT);
    // Default: all slots empty (green ON)
    digitalWrite(greenPins[i], HIGH);
    digitalWrite(redPins[i], LOW);
  }

  // Servo
  gateServo.attach(SERVO_PIN);
  gateServo.write(GATE_CLOSED);

  // LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Smart Parking");
  lcd.setCursor(0, 1);
  lcd.print("System Ready!");
  delay(2000);

  Serial.println("System initialized.");
}

// ======================== MAIN LOOP ==============================
void loop() {
  // ---- Check all parking slots ----
  int availableSlots = 0;

  for (int i = 0; i < TOTAL_SLOTS; i++) {
    float distance = measureDistance(trigPins[i], echoPins[i]);

    if (distance > 0 && distance < SLOT_THRESHOLD) {
      // Slot occupied
      slotOccupied[i] = true;
      digitalWrite(greenPins[i], LOW);
      digitalWrite(redPins[i], HIGH);
    } else {
      // Slot empty
      slotOccupied[i] = false;
      digitalWrite(greenPins[i], HIGH);
      digitalWrite(redPins[i], LOW);
      availableSlots++;
    }

    Serial.print("Slot ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print(distance, 1);
    Serial.print(" cm → ");
    Serial.println(slotOccupied[i] ? "OCCUPIED" : "EMPTY");
  }

  // ---- Update LCD ----
  lcd.clear();
  if (availableSlots > 0) {
    lcd.setCursor(0, 0);
    lcd.print("Available: ");
    lcd.print(availableSlots);
    lcd.print("/");
    lcd.print(TOTAL_SLOTS);
  } else {
    lcd.setCursor(0, 0);
    lcd.print("!! PARKING FULL !!");
  }

  // ---- Check entry sensor ----
  float entryDist = measureDistance(ENTRY_TRIG, ENTRY_ECHO);
  Serial.print("Entry Sensor: ");
  Serial.print(entryDist, 1);
  Serial.println(" cm");

  if (entryDist > 0 && entryDist < ENTRY_THRESHOLD) {
    // Vehicle approaching
    if (availableSlots > 0 && !gateOpen) {
      // Open gate
      Serial.println(">> Vehicle detected — Opening gate");
      gateServo.write(GATE_OPEN);
      gateOpen = true;

      lcd.setCursor(0, 1);
      lcd.print("Gate: OPEN");

      delay(5000);   // Keep gate open for 5 seconds

      gateServo.write(GATE_CLOSED);
      gateOpen = false;
      Serial.println(">> Gate closed");
    } else if (availableSlots == 0) {
      lcd.setCursor(0, 1);
      lcd.print("Gate: CLOSED");
      Serial.println(">> Parking full — gate remains closed");

      // Short beep to indicate full
      // (Could add buzzer here)
    }
  } else {
    lcd.setCursor(0, 1);
    lcd.print("Gate: Closed");
  }

  Serial.println("----------------------------");
  delay(500);
}

// ======================== DISTANCE MEASUREMENT ===================
float measureDistance(int trigPin, int echoPin) {
  // Send trigger pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure echo duration
  long duration = pulseIn(echoPin, HIGH, 25000);  // Timeout: 25ms

  if (duration == 0) return -1;   // No echo received

  // Calculate distance: Speed of sound = 343 m/s
  float distance = duration * 0.0343 / 2.0;
  return distance;
}
