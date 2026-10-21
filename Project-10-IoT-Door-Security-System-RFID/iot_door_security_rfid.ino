/*
 * =========================================================================
 *  Project 10: IoT Door Security System with RFID
 *  Board: NodeMCU ESP8266 (ESP-12E)
 *  Module: RFID RC522 (SPI), Servo Motor SG90
 *  Display: 0.96" OLED SSD1306
 *  App: Blynk IoT
 * =========================================================================
 *  Description:
 *  RFID-based door lock with authorized card authentication, servo lock
 *  mechanism, OLED display, buzzer feedback, and Blynk IoT integration
 *  for remote control and access logging.
 * =========================================================================
 */

#define BLYNK_TEMPLATE_ID   "TMPL_XXXXXX"
#define BLYNK_TEMPLATE_NAME "Door Security"
#define BLYNK_AUTH_TOKEN     "YOUR_BLYNK_AUTH_TOKEN"

#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ======================== PIN DEFINITIONS ========================
#define SS_PIN        D4        // RFID SDA/SS (GPIO2)
#define RST_PIN       D3        // RFID RST (GPIO0)
#define SERVO_PIN     D8        // Servo signal (GPIO15)
#define BUZZER_PIN    D0        // Buzzer (GPIO16)

// ======================== OLED SETTINGS =========================
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ======================== WI-FI CREDENTIALS =====================
char ssid[] = "YOUR_WIFI_SSID";
char pass[] = "YOUR_WIFI_PASSWORD";

// ======================== OBJECTS ================================
MFRC522 rfid(SS_PIN, RST_PIN);
Servo doorLock;

// ======================== AUTHORIZED CARDS =======================
// Add your authorized RFID card UIDs here (4 bytes each)
// To find your card's UID, scan it and check Serial Monitor
const int NUM_AUTHORIZED = 3;
byte authorizedUIDs[NUM_AUTHORIZED][4] = {
  {0xA1, 0xB2, 0xC3, 0xD4},    // Card 1 — Replace with your UID
  {0xE5, 0xF6, 0x07, 0x18},    // Card 2 — Replace with your UID
  {0x29, 0x3A, 0x4B, 0x5C}     // Card 3 — Replace with your UID
};

String cardNames[NUM_AUTHORIZED] = {
  "User 1",
  "User 2",
  "User 3"
};

// ======================== VARIABLES ==============================
bool doorLocked = true;
int accessCount = 0;
int deniedCount = 0;

// ======================== BLYNK HANDLERS ========================
// Remote lock/unlock via Blynk app
BLYNK_WRITE(V1) {
  int value = param.asInt();
  if (value == 1) {
    unlockDoor("Remote");
    Serial.println("Blynk: Remote UNLOCK");
  } else {
    lockDoor();
    Serial.println("Blynk: Remote LOCK");
  }
}

// ======================== SETUP ==================================
void setup() {
  Serial.begin(115200);
  Serial.println("\n--- IoT Door Security System (RFID) ---");

  // Pin setup
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  // Initialize SPI and RFID
  SPI.begin();
  rfid.PCD_Init();
  Serial.println("RFID RC522 initialized.");
  Serial.print("Firmware version: ");
  rfid.PCD_DumpVersionToSerial();

  // Initialize Servo
  doorLock.attach(SERVO_PIN);
  doorLock.write(0);    // Locked position
  doorLocked = true;

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("ERROR: OLED not found!");
    while (true);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 20);
  display.println("Door Security");
  display.setCursor(10, 35);
  display.println("Initializing...");
  display.display();

  // Connect to Blynk (includes Wi-Fi)
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  Serial.println("Connected to Blynk!");

  // Ready screen
  showReadyScreen();
  Serial.println("System ready. Waiting for RFID cards...");
  Serial.println("==========================================");
}

// ======================== MAIN LOOP ==============================
void loop() {
  Blynk.run();

  // Check for new RFID card
  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;

  // Read the UID
  String uidString = "";
  Serial.print("Card UID: ");
  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(rfid.uid.uidByte[i], HEX);
    uidString += String(rfid.uid.uidByte[i], HEX);
  }
  Serial.println();
  uidString.toUpperCase();

  // Check authorization
  int cardIndex = checkAuthorization(rfid.uid.uidByte);

  if (cardIndex >= 0) {
    // ========= ACCESS GRANTED =========
    accessCount++;
    Serial.print("✓ ACCESS GRANTED — ");
    Serial.println(cardNames[cardIndex]);

    unlockDoor(cardNames[cardIndex]);

    // Log to Blynk
    Blynk.virtualWrite(V2, "GRANTED: " + cardNames[cardIndex]);
    Blynk.virtualWrite(V3, accessCount);
    Blynk.logEvent("access_granted", cardNames[cardIndex] + " entered");

    // Auto-lock after 5 seconds
    delay(5000);
    lockDoor();

  } else {
    // ========= ACCESS DENIED =========
    deniedCount++;
    Serial.println("✗ ACCESS DENIED — Unknown card!");

    denyAccess(uidString);

    // Log to Blynk
    Blynk.virtualWrite(V2, "DENIED: UID=" + uidString);
    Blynk.virtualWrite(V4, deniedCount);
    Blynk.logEvent("access_denied", "Unknown card: " + uidString);
  }

  // Halt PICC (stop reading)
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();

  // Return to ready screen
  delay(2000);
  showReadyScreen();
}

// ======================== AUTHORIZATION CHECK ====================
int checkAuthorization(byte* uid) {
  for (int i = 0; i < NUM_AUTHORIZED; i++) {
    bool match = true;
    for (byte j = 0; j < 4; j++) {
      if (uid[j] != authorizedUIDs[i][j]) {
        match = false;
        break;
      }
    }
    if (match) return i;   // Return index of matched card
  }
  return -1;   // No match found
}

// ======================== UNLOCK DOOR ============================
void unlockDoor(String userName) {
  doorLocked = false;

  // Servo to unlock position
  doorLock.write(90);

  // Success buzzer pattern (two short beeps)
  digitalWrite(BUZZER_PIN, HIGH);
  delay(100);
  digitalWrite(BUZZER_PIN, LOW);
  delay(50);
  digitalWrite(BUZZER_PIN, HIGH);
  delay(100);
  digitalWrite(BUZZER_PIN, LOW);

  // OLED display
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(20, 5);
  display.println("ACCESS GRANTED");
  display.drawLine(0, 15, 128, 15, SSD1306_WHITE);

  display.setTextSize(2);
  display.setCursor(30, 22);
  display.println("OPEN");

  display.setTextSize(1);
  display.setCursor(5, 45);
  display.print("Welcome, ");
  display.println(userName);

  display.setCursor(5, 56);
  display.print("Locks in 5s...");
  display.display();

  // Update Blynk button state
  Blynk.virtualWrite(V1, 1);
}

// ======================== LOCK DOOR ==============================
void lockDoor() {
  doorLocked = true;
  doorLock.write(0);    // Lock position

  // Lock beep
  digitalWrite(BUZZER_PIN, HIGH);
  delay(200);
  digitalWrite(BUZZER_PIN, LOW);

  // Update Blynk
  Blynk.virtualWrite(V1, 0);

  Serial.println("Door LOCKED.");
}

// ======================== DENY ACCESS ============================
void denyAccess(String uid) {
  // Error buzzer pattern (long buzz)
  for (int i = 0; i < 3; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(300);
    digitalWrite(BUZZER_PIN, LOW);
    delay(100);
  }

  // OLED display
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(20, 5);
  display.println("ACCESS DENIED");
  display.drawLine(0, 15, 128, 15, SSD1306_WHITE);

  display.setTextSize(2);
  display.setCursor(15, 22);
  display.println("DENIED");

  display.setTextSize(1);
  display.setCursor(5, 48);
  display.print("UID: ");
  display.println(uid);
  display.display();
}

// ======================== READY SCREEN ===========================
void showReadyScreen() {
  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(10, 5);
  display.println("DOOR SECURITY SYS");
  display.drawLine(0, 15, 128, 15, SSD1306_WHITE);

  display.setTextSize(1);
  display.setCursor(15, 22);
  display.println("Scan your RFID");
  display.setCursor(15, 35);
  display.println("card to enter");

  display.drawLine(0, 48, 128, 48, SSD1306_WHITE);
  display.setCursor(0, 52);
  display.print("OK:");
  display.print(accessCount);
  display.print(" DENY:");
  display.print(deniedCount);
  display.print(" ");
  display.print(doorLocked ? "LOCKED" : "OPEN");

  display.display();
}
