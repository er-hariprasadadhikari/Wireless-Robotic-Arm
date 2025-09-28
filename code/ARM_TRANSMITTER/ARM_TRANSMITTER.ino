// Transmitter Code for Robotic Arm Control

// Potentiometer Pins
const int potWrist = A3;
const int potElbow = A2;
const int potShoulder = A1;
const int potBase = A0;

// Button Pin
const int buttonPin = 13;

void setup() {
  Serial.begin(9600);        // Bluetooth module connected to default Serial
  pinMode(buttonPin, INPUT_PULLUP);

  // Optional: Startup message
  Serial.println("Transmitter Ready...");
}

void loop() {
  // Read potentiometer values
  int valWrist = analogRead(potWrist);
  int valElbow = analogRead(potElbow);
  int valShoulder = analogRead(potShoulder);
  int valBase = analogRead(potBase);

  // Read button state (0 = pressed, 1 = released)
  int buttonState = digitalRead(buttonPin) == LOW ? 1 : 0;

  // Send data in structured format: <wrist,elbow,shoulder,base,button>
  Serial.print("<");
  Serial.print(valWrist); Serial.print(",");
  Serial.print(valElbow); Serial.print(",");
  Serial.print(valShoulder); Serial.print(",");
  Serial.print(valBase); Serial.print(",");
  Serial.print(buttonState);
  Serial.println(">");

  // Debug output (optional)
  Serial.print("Sent: ");
  Serial.print(valWrist); Serial.print(", ");
  Serial.print(valElbow); Serial.print(", ");
  Serial.print(valShoulder); Serial.print(", ");
  Serial.print(valBase); Serial.print(", ");
  Serial.println(buttonState == 1 ? "PRESSED" : "RELEASED");

  delay(100);  // Adjust transmission rate (100ms)
}
