#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>

// HC-05 connected to pins 10 (RX) and 11 (TX)
SoftwareSerial BTSerial(10, 11);  // RX, TX

#define MIN_PULSE_WIDTH       650
#define MAX_PULSE_WIDTH       2350
#define FREQUENCY             50
#define STEP_SIZE             8
#define STEP_DELAY            5

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Servo channel numbers
int hand = 11;
int wrist = 12;
int elbow = 13;
int shoulder = 14;
int base = 15;

int lastPWM[16] = {0};  // Store last known PWM values

// EEPROM addresses
const int EEPROM_ADDR_WRIST = 0;
const int EEPROM_ADDR_ELBOW = 2;
const int EEPROM_ADDR_SHOULDER = 4;
const int EEPROM_ADDR_BASE = 6;
const int EEPROM_ADDR_HAND = 8;

// Incoming Bluetooth data
String inputString = "";
bool stringComplete = false;
int valWrist, valElbow, valShoulder, valBase, btnState;

void setup() {
  delay(5000);  // Startup delay if needed

  Serial.begin(9600);        // Debug output via Serial Monitor
  BTSerial.begin(9600);      // Bluetooth serial communication

  pwm.begin();
  pwm.setPWMFreq(FREQUENCY);

  // Load saved initial position from EEPROM or set default values
  loadFromEEPROM();

  Serial.println("Robotic Arm Receiver Ready (EEPROM + Bluetooth + Serial)");
}

int angleToPWM(int angle) {
  int pulse = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  return int(float(pulse) / 1000000 * FREQUENCY * 4096);
}

int potToPWM(int val, bool reverse) {
  if (reverse) {
    val = map(val, 800, 240, MAX_PULSE_WIDTH, MIN_PULSE_WIDTH);
  } else {
    val = map(val, 800, 240, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  }
  return int(float(val) / 1000000 * FREQUENCY * 4096);
}

void smoothMove(int motorOut, int targetPWM) {
  int currentPWM = lastPWM[motorOut];
  if (abs(currentPWM - targetPWM) < STEP_SIZE) {
    currentPWM = targetPWM;
  } else {
    if (targetPWM > currentPWM) currentPWM += STEP_SIZE;
    else currentPWM -= STEP_SIZE;
  }
  pwm.setPWM(motorOut, 0, currentPWM);
  lastPWM[motorOut] = currentPWM;
}

void processInput() {
  int values[5];
  int idx = 0;
  char *ptr = strtok((char*)inputString.c_str(), ",");
  while (ptr != NULL && idx < 5) {
    values[idx++] = atoi(ptr);
    ptr = strtok(NULL, ",");
  }

  if (idx == 5) {
    valWrist = values[0];
    valElbow = values[1];
    valShoulder = values[2];
    valBase = values[3];
    btnState = values[4];

    // Debug output to Serial Monitor
    Serial.println("==== Data Received ====");
    Serial.print("Wrist: "); Serial.println(valWrist);
    Serial.print("Elbow: "); Serial.println(valElbow);
    Serial.print("Shoulder: "); Serial.println(valShoulder);
    Serial.print("Base: "); Serial.println(valBase);
    Serial.print("Button: "); Serial.println(btnState == 1 ? "PRESSED" : "RELEASED");
    Serial.println("========================");

    // Move servos
    smoothMove(wrist, potToPWM(valWrist, false));
    smoothMove(elbow, potToPWM(valElbow, true));
    smoothMove(shoulder, potToPWM(valShoulder, true));
    smoothMove(base, potToPWM(valBase, true));

    int targetPWM = (btnState == 1) ? angleToPWM(180) : angleToPWM(90);
    smoothMove(hand, targetPWM);
  } else {
    Serial.println("ERROR: Incomplete or bad data");
  }
}

void saveToEEPROM() {
  // Save the current values to EEPROM
  EEPROM.put(EEPROM_ADDR_WRIST, lastPWM[wrist]);
  EEPROM.put(EEPROM_ADDR_ELBOW, lastPWM[elbow]);
  EEPROM.put(EEPROM_ADDR_SHOULDER, lastPWM[shoulder]);
  EEPROM.put(EEPROM_ADDR_BASE, lastPWM[base]);
  EEPROM.put(EEPROM_ADDR_HAND, lastPWM[hand]);
  Serial.println("✅ New initial state saved to EEPROM.");
}

void loadFromEEPROM() {
  // Read saved initial position from EEPROM, or set default values
  EEPROM.get(EEPROM_ADDR_WRIST, lastPWM[wrist]);
  EEPROM.get(EEPROM_ADDR_ELBOW, lastPWM[elbow]);
  EEPROM.get(EEPROM_ADDR_SHOULDER, lastPWM[shoulder]);
  EEPROM.get(EEPROM_ADDR_BASE, lastPWM[base]);
  EEPROM.get(EEPROM_ADDR_HAND, lastPWM[hand]);

  // If no valid data in EEPROM, set default values
  if (lastPWM[wrist] == 0) lastPWM[wrist] = 765;
  if (lastPWM[elbow] == 0) lastPWM[elbow] = 957;
  if (lastPWM[shoulder] == 0) lastPWM[shoulder] = 143;
  if (lastPWM[base] == 0) lastPWM[base] = 809;
  if (lastPWM[hand] == 0) lastPWM[hand] = angleToPWM(90);  // Default hand position

  // Move servos to their initial positions
  pwm.setPWM(wrist, 0, lastPWM[wrist]);
  pwm.setPWM(elbow, 0, lastPWM[elbow]);
  pwm.setPWM(shoulder, 0, lastPWM[shoulder]);
  pwm.setPWM(base, 0, lastPWM[base]);
  pwm.setPWM(hand, 0, lastPWM[hand]);

  Serial.println("✅ Initial position loaded from EEPROM.");
}

void loop() {
  while (BTSerial.available()) {
    char inChar = (char)BTSerial.read();
    if (inChar == '<') {
      inputString = "";
    } else if (inChar == '>') {
      stringComplete = true;
    } else {
      inputString += inChar;
    }
  }

  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '<') {
      inputString = "";
    } else if (inChar == '>') {
      stringComplete = true;
    } else {
      inputString += inChar;
    }
  }

  if (stringComplete) {
    if (inputString == "newinitialstate") {
      saveToEEPROM();
    } else {
      processInput();
    }
    stringComplete = false;
  }

  delay(STEP_DELAY);
}
