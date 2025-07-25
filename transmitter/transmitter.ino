// Assign PWM-capable pins for RGB lasers
const int redLaser   = 9;
const int greenLaser = 10;
const int blueLaser  = 11;

// Set brightness/power levels (0–255)
const int redPower   = 150;
const int greenPower = 0;
const int bluePower  = 0;

void setup() {
  Serial.begin(9600);
  pinMode(redLaser, OUTPUT);
  pinMode(greenLaser, OUTPUT);
  pinMode(blueLaser, OUTPUT);
}

// Send start signal: 8 bits of all 1s (0xFF)
void sendStartSignal() {
  for (int bit = 0; bit < 8; bit++) {
    analogWrite(redLaser,   redPower);
    analogWrite(greenLaser, greenPower);
    analogWrite(blueLaser,  bluePower);
    delay(500);
  }

  // Turn off lasers briefly after start signal
  analogWrite(redLaser, 255);
  analogWrite(greenLaser, 255);
  analogWrite(blueLaser, 255);
}
void sendWDM(String input) {
  sendStartSignal();

  // Pad message with spaces so length is multiple of 3
  while (input.length() % 3 != 0) {
    input += ' ';
  }

  int len = input.length();
  for (int i = 0; i < len; i += 3) {
    char r = input.charAt(i);
    char g = input.charAt(i + 1);
    char b = input.charAt(i + 2);

    for (int bit = 7; bit >= 0; bit--) {
      analogWrite(redLaser,   ((r >> bit) & 1) ? redPower   : 255);
      analogWrite(greenLaser, ((g >> bit) & 1) ? greenPower : 255);
      analogWrite(blueLaser,  ((b >> bit) & 1) ? bluePower  : 255);
      delay(500);
    }
  }

  // Turn off all lasers after message
  analogWrite(redLaser, 0);
  analogWrite(greenLaser, 0);
  analogWrite(blueLaser, 0);
}
void sendSingleColor(String input, char color) {
  sendStartSignal();
  int power = (color == 'R') ? redPower :
              (color == 'G') ? greenPower :
                               bluePower;

  int laserPin = (color == 'R') ? redLaser :
                 (color == 'G') ? greenLaser :
                                  blueLaser;

  for (int i = 0; i < input.length(); i++) {
    char c = input.charAt(i);
    for (int bit = 7; bit >= 0; bit--) {
      analogWrite(laserPin, ((c >> bit) & 1) ? 255 : power);
      delay(500);
    }
  }
  analogWrite(laserPin, 255);
}
void loop() {
  if (Serial.available() > 0) {
    char mode = Serial.read();  // 'R', 'G', 'B', or 'W'
    delay(5);
    String msg = Serial.readStringUntil('\n');

    if (mode == 'W') {
      sendWDM(msg);
    } else if (mode == 'R' || mode == 'G' || mode == 'B') {
      sendSingleColor(msg, mode);
    }
  }
  else{
    analogWrite(redLaser,255);
    analogWrite(greenLaser,255);
    analogWrite(blueLaser,255);
  }
}



