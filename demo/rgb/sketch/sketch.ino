const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;
int color[3];
int index;

void setup() {
  while (!Serial) {
    ;
  }
  Serial.write("Sketch begins.\r\n");
  Serial.begin(9600);
  index = 0;
}

void loop() {
  if (Serial && Serial.available()) {
    color[index++] = Serial.read();
    if (index == 3) {
      analogWrite(redPin, color[0]);
      analogWrite(greenPin, color[1]);
      analogWrite(bluePin, color[2]);
      Serial.print("Set LED to ");
      Serial.print(color[0]);
      Serial.print(", ");
      Serial.print(color[1]);
      Serial.print(", ");
      Serial.print(color[2]);
      Serial.print(".\r\n");
      index = 0;
    }
  }
}
