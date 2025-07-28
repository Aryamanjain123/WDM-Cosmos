// Assign PWM-capable pins for RGB lasers
const int redLaser   = 9;
const int greenLaser = 10;
const int blueLaser  = 11;

// Set brightness/power levels (0–255)
const int redPower   = 150;
const int greenPower = 0;
const int bluePower  = 0;
unsigned long bitRate = 200;

void setup() {
  Serial.begin(9600);
  pinMode(redLaser, OUTPUT);
  pinMode(greenLaser, OUTPUT);
  pinMode(blueLaser, OUTPUT);
}
void sendStartSignal(char color) {
  int laserPin = (color == 'R') ? redLaser :
                 (color == 'G') ? greenLaser :
                                  blueLaser;
  if (color == 'R' || color == 'G' || color == 'B') {
    for (int bit = 0; bit < 8; bit++) {
      analogWrite(laserPin, 0);
      delay(bitRate);
    }}
  else{
    analogWrite(redLaser, 0);
    analogWrite(greenLaser, 0);
    analogWrite(blueLaser, 0);
    delay(bitRate * 8);
  }

  // Turn off lasers briefly after start signal
  analogWrite(redLaser, 255);
  analogWrite(greenLaser, 255);
  analogWrite(blueLaser, 255);
}
void sendWDM(String input) {
  sendStartSignal('W');

  // Pad message with spaces so length is multiple of 3
  while (input.length() % 3 != 0) {
    input += ' ';
  }

  int len = input.length();
  for (int i = 0; i < len; i += 3) {
    char r = input.charAt(i);
    char g = input.charAt(i + 1);
    char b = input.charAt(i + 2);

    Serial.print("Chunk: R=");
    Serial.print(r); Serial.print(" ("); Serial.print((int)r); Serial.print("), ");
    Serial.print("G="); Serial.print(g); Serial.print(" ("); Serial.print((int)g); Serial.print("), ");
    Serial.print("B="); Serial.print(b); Serial.print(" ("); Serial.print((int)b); Serial.println(")");
    Serial.print("Bits: ");

    for (int bit = 7; bit >= 0; bit--) {
      int rBit = (r >> bit) & 1;
      int gBit = (g >> bit) & 1;
      int bBit = (b >> bit) & 1;

      analogWrite(redLaser,   rBit ? 255 : redPower);
      analogWrite(greenLaser, gBit ? 255 : greenPower);
      analogWrite(blueLaser,  bBit ? 255 : bluePower);

      Serial.print(rBit);
      Serial.print(gBit);
      Serial.print(bBit);
      Serial.print(" ");

      delay(bitRate);
    }

    Serial.println(); // Newline after each 8-bit chunk
  }
  sendStartSignal('W');
  // Turn off all lasers after message
  analogWrite(redLaser, 0);
  analogWrite(greenLaser, 0);
  analogWrite(blueLaser, 0);
}

void sendSingleColor(String input, char color) {
  sendStartSignal(color);

  int power = (color == 'R') ? redPower :
              (color == 'G') ? greenPower :
                               bluePower;

  int laserPin = (color == 'R') ? redLaser :
                 (color == 'G') ? greenLaser :
                                  blueLaser;

  for (int i = 0; i < input.length(); i++) {
    char c = input.charAt(i);
    Serial.print("Char: ");
    Serial.print(c);
    Serial.print(" (");
    Serial.print((int)c);
    Serial.println(") Bits: ");
    
    for (int bit = 7; bit >= 0; bit--) {
      int bitVal = (c >> bit) & 1;
      analogWrite(laserPin, bitVal ? 255 : power);
      Serial.print(bitVal);
      delay(bitRate);
    }
    Serial.println();  // move to next line after 8 bits
  }
  sendStartSignal(color);

  analogWrite(laserPin, 255); // Turn off
}

void loop() {
  if (Serial.available() > 0) {
    char mode = Serial.read();  // 'R', 'G', 'B', or 'W'
    delay(5);
    String msg = Serial.readStringUntil('\n');

    if (mode == 'W') {
      sendWDM(msg);
    } else if (mode =='Z'){
      analogWrite(redLaser,0);
      analogWrite(greenLaser,0);
      analogWrite(blueLaser,0);
      delay(1000000);
    }  else if (mode == 'R' || mode == 'G' || mode == 'B') {
      sendSingleColor(msg, mode);
    }
  }
  else{
    analogWrite(redLaser,255);
    analogWrite(greenLaser,255);
    analogWrite(blueLaser,255);
  }
}



