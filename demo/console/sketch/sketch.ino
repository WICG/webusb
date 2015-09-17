const int ledPin = 13;

void setup() {
  while (!Serial) {
    ;
  }
  Serial.begin(9600);
  Serial.write("Sketch begins.\r\n> ");
  pinMode(ledPin, OUTPUT);
}

void loop() {
  if (Serial && Serial.available()) {
    int byte = Serial.read();
    Serial.write(byte);
    if (byte == 'H') {
      Serial.write("\r\nTurning LED on.");
      digitalWrite(ledPin, HIGH);
    } else if (byte == 'L') {
      Serial.write("\r\nTurning LED off.");
      digitalWrite(ledPin, LOW);
    }
    Serial.write("\r\n> ");
  }
}
