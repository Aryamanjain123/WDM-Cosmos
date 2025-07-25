#define RED_PIN   2
#define GREEN_PIN 3
#define BLUE_PIN  4




const int bitDuration = 500; // milliseconds per bit (~9600 baud)
const int maxChars = 8 * 6;


int redMessage[maxChars + 1];
int greenMessage[maxChars + 1];
int blueMessage[maxChars + 1];




int redIndex = 0, greenIndex = 0, blueIndex = 0;
bool redReceiving = false, greenReceiving = false, blueReceiving = false;


unsigned long redStartTime = 0, greenStartTime = 0, blueStartTime = 0;
unsigned long redStartBitTime = 0, greenStartBitTime = 0, blueStartBitTime = 0;
bool redStartBit = false, greenStartBit = false, blueStartBit = false;




void setup() {
Serial.begin(9600);
pinMode(RED_PIN, INPUT);
pinMode(GREEN_PIN, INPUT);
pinMode(BLUE_PIN, INPUT);
redReceiving = false; greenReceiving = false; blueReceiving = false;
}




void loop() {


////// RED CHANNEL ///////
//Serial.println("RedIndex :" + String(redIndex));
//int redBit = digitalRead(RED_PIN);
//Serial.print("Red Bit value: ");
//Serial.print(redBit);
if (!redReceiving) {
  if(digitalRead(RED_PIN) == 0 && !redStartBit){
    redStartBit = true;
    redStartBitTime = millis();
    Serial.println("RED Reading Start Bit");
  } else if (digitalRead(RED_PIN) == 1) {
    redStartBit = false;
    redStartBitTime = millis();
    //Serial.println("RESTARTING Start Bit");
  }
  bool redStart = (millis() - redStartBitTime) >= (7*bitDuration);
  if (redStart) {
    redReceiving = true;
    redStartTime = millis();
    redIndex = 0;
    Serial.println("START");
    //delay(bitDuration/3);
  }
} else if (redIndex < maxChars && (millis() - redStartTime) >= (redIndex+1)*bitDuration) {
  //byte r = readByte(RED_PIN);
  int r = digitalRead(RED_PIN);
  redMessage[redIndex++] = r;
  Serial.print("Red Message!!! ");
  for(int i=0; i<redIndex; i++){
    Serial.print(redMessage[i]);
    if(i%8==7){
      Serial.print(" ");
    }
  }
  Serial.println(" :)");
  /*
  if (r == '\n') {
    redMessage[redIndex] = '\0';
    printMessage("Red", redMessage, redStartTime);
    redReceiving = false;
  }
  */
 }


 //RED END








 //GREEN START
 //Serial.println("GreenIndex :" + String(greenIndex));
 //int greenBit = digitalRead(GREEN_PIN);
 //Serial.print("Green Bit value: ");
 //Serial.print(greenBit);
 if (!greenReceiving ) {
   if(digitalRead(GREEN_PIN) == 0 && !greenStartBit){
     greenStartBit = true;
     greenStartBitTime = millis();
     Serial.println("GREEN Reading Start Bit");
   } else if (digitalRead(GREEN_PIN) == 1) {
     greenStartBit = false;
     greenStartBitTime = millis();
     //Serial.println("RESTARTING Start Bit");
   }
   bool greenStart = (millis() - greenStartBitTime) >= (7*bitDuration);
   if (greenStart) {
     greenReceiving = true;
     greenStartTime = millis();
     greenIndex = 0;
     Serial.println("START");
     //delay(bitDuration/3);
   }
 } else if (greenIndex < maxChars && (millis() - greenStartTime) >= (greenIndex+1)*bitDuration) {
  //byte r = readByte(GREEN_PIN);
  int r = digitalRead(GREEN_PIN);
  greenMessage[greenIndex++] = r;
  Serial.print("Green Message! ");
  for(int i=0; i<greenIndex; i++){
    Serial.print(greenMessage[i]);
    if(i%8==7){
      Serial.print(" ");
    }
  }
  Serial.println(" :)");
  /*
  if (r == '\n') {
    greenMessage[greenIndex] = '\0';
    printMessage("Green", greenMessage, greenStartTime);
    greenReceiving = false;
  }
 */
 }


 //GREEN END






 //BLUE START
 //Serial.println("BlueIndex :" + String(blueIndex));
 //int blueBit = digitalRead(BLUE_PIN);
 //Serial.print("Blue Bit value: ");
 //Serial.print(blueBit);
 if (!blueReceiving ) {
   if(digitalRead(BLUE_PIN) == 0 && !blueStartBit){
     blueStartBit = true;
     blueStartBitTime = millis();
     Serial.println("BLUE Reading Start Bit");
   } else if (digitalRead(BLUE_PIN) == 1) {
     blueStartBit = false;
     blueStartBitTime = millis();
     //Serial.println("RESTARTING Start Bit");
   }
   bool blueStart = (millis() - blueStartBitTime) >= (7*bitDuration);
   if (blueStart) {
     blueReceiving = true;
     blueStartTime = millis();
     blueIndex = 0;
     Serial.println("START");
     //delay(bitDuration/3);
   }
 } else if (blueIndex < maxChars && (millis() - blueStartTime) >= (blueIndex+1)*bitDuration) {
  //byte r = readByte(BLUE_PIN);
  int r = digitalRead(BLUE_PIN);
  blueMessage[blueIndex++] = r;
  Serial.print("Blue Message!! ");
  for(int i=0; i<blueIndex; i++){
    Serial.print(blueMessage[i]);
    if(i%8==7){
      Serial.print(" ");
    }
  }
  Serial.println(" :)");
  /*
  if (r == '\n') {
    blueMessage[blueIndex] = '\0';
    printMessage("Blue", blueMessage, blueStartTime);
    blueReceiving = false;
  }
 */
 }
 //BLUE END


}








// Read 1 byte from a single digital pin
byte readByte(int pin) {
byte value = 0;
for (int i = 0; i < 8; i++) {
  delayMicroseconds(bitDuration);
  int bit = digitalRead(pin);
  value |= (bit << i);
}
return value;
}




void printMessage(const char* label, char* msg, unsigned long startTime) {
unsigned long endTime = micros();
Serial.print(label); Serial.print(": ");
Serial.println(msg);
Serial.print("Time for "); Serial.print(label); Serial.print(": ");
Serial.print(endTime - startTime); Serial.println(" us");
}




