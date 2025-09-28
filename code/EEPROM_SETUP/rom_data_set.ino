#include <EEPROM.h>

const int potWrist = A3;
const int potElbow = A2;
const int potShoulder = A1;
const int potBase = A0;
const int buttonPin = 13;

String command = "";
unsigned long lastPrintTime = 0;
const unsigned long printInterval = 1000; // print every 1 second

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.println("=== Robotic Arm EEPROM Setup ===");
  Serial.println("Live pot values will display every second.");
  Serial.println("Type SAVE to confirm and store current values to EEPROM.\n");
}

void loop() {
  // Print live values every 1 second
  if (millis() - lastPrintTime >= printInterval) {
    lastPrintTime = millis();
    printLiveValues();
  }

  // Read command from Serial Monitor
  if (Serial.available()) {
    char c = Serial.read();
    if (c == '\n' || c == '\r') {
      processCommand(command);
      command = "";
    } else {
      command += c;
    }
  }
}

void printLiveValues() {
  int valWrist = analogRead(potWrist);
  int valElbow = analogRead(potElbow);
  int valShoulder = analogRead(potShoulder);
  int valBase = analogRead(potBase);
  int btnState = digitalRead(buttonPin) == LOW ? 1 : 0;

  Serial.println("Current Pot Values:");
  Serial.print("  Wrist: ");    Serial.println(valWrist);
  Serial.print("  Elbow: ");    Serial.println(valElbow);
  Serial.print("  Shoulder: "); Serial.println(valShoulder);
  Serial.print("  Base: ");     Serial.println(valBase);
  Serial.print("  Hand: ");     Serial.println(btnState == 1 ? "Closed (Button Pressed)" : "Open");
  Serial.println("Type SAVE to store these values.\n");
}

void processCommand(String cmd) {
  cmd.trim();
  cmd.toUpperCase();

  if (cmd == "SAVE") {
    int valWrist = analogRead(potWrist);
    int valElbow = analogRead(potElbow);
    int valShoulder = analogRead(potShoulder);
    int valBase = analogRead(potBase);
    int btnState = digitalRead(buttonPin) == LOW ? 1 : 0;

    EEPROM.put(0, valWrist);
    EEPROM.put(2, valElbow);
    EEPROM.put(4, valShoulder);
    EEPROM.put(6, valBase);
    EEPROM.put(8, btnState);

    Serial.println("✅ EEPROM saved with the following values:");
    Serial.print("  Wrist: ");    Serial.println(valWrist);
    Serial.print("  Elbow: ");    Serial.println(valElbow);
    Serial.print("  Shoulder: "); Serial.println(valShoulder);
    Serial.print("  Base: ");     Serial.println(valBase);
    Serial.print("  Hand: ");     Serial.println(btnState == 1 ? "Closed" : "Open");
    Serial.println("You may now unplug and switch to the main arm control code.\n");
  } else {
    Serial.println("❌ Unknown command. Type SAVE to confirm storing current values.");
  }
}
