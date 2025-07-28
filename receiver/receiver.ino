#define RED_PIN   2
#define GREEN_PIN 3
#define BLUE_PIN  4

const int bitDuration = 50; // milliseconds per bit (~9600 baud)
const int maxChars = 8 * 56;

bool redMessage[maxChars + 1];
bool greenMessage[maxChars + 1];
bool blueMessage[maxChars + 1];

int redCurrentChar[8];
char redWords[maxChars/8];

int redIndex = 0, greenIndex = 0, blueIndex = 0;
bool redReceiving = false, greenReceiving = false, blueReceiving = false;

unsigned long redStartTime = 0, greenStartTime = 0, blueStartTime = 0;
unsigned long redStartBitTime = 0, greenStartBitTime = 0, blueStartBitTime = 0;
bool redStartBit = false, greenStartBit = false, blueStartBit = false;


char bitsToChar(int bits[8]) {
  int value = 0;
  for(int i=0; i<8; i++){
    value = value + bits[i] * pow(2, i);
  }
  return char(value);
}


void setup() {
 Serial.begin(9600);
 pinMode(RED_PIN, INPUT);
 pinMode(GREEN_PIN, INPUT);
 pinMode(BLUE_PIN, INPUT);
 redReceiving = false; greenReceiving = false; blueReceiving = false;
 Serial.println("READY");
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
 } else if (redIndex < maxChars && (millis() - redStartTime) >= (redIndex+1.5)*bitDuration) {
    int r = digitalRead(RED_PIN);
    redMessage[redIndex++] = r;
    redCurrentChar[redIndex%8] = r;
    if(redIndex%8==7){
      redWords[(redIndex+1)/8] = bitsToChar(redCurrentChar);
    }
   /*
   if (r == '\n') {
     redMessage[redIndex] = '\0';
     printMessage("Red", redMessage, redStartTime);
     redReceiving = false;
   }
   */
  } else if (redReceiving && redIndex >= maxChars) {
    Serial.print("Red Message!!! ");
    for(int i=0; i<redIndex; i++){
      Serial.print(redMessage[i]);
      if(i%8==7){
        Serial.print(" ");
      }
    }
    Serial.println(" :)");
    redReceiving = false;

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
  } else if (greenIndex < maxChars && (millis() - greenStartTime) >= (greenIndex+1.5)*bitDuration) {
   //byte r = readByte(GREEN_PIN);
   int r = digitalRead(GREEN_PIN);
   greenMessage[greenIndex++] = r;
   /*
   if (r == '\n') {
     greenMessage[greenIndex] = '\0';
     printMessage("Green", greenMessage, greenStartTime);
     greenReceiving = false;
   } 
  */
  } else if (greenReceiving && greenIndex>=maxChars){
    Serial.print("Green Message! ");
    for(int i=0; i<greenIndex; i++){
      Serial.print(greenMessage[i]);
      if(i%8==7){
        Serial.print(" ");
      }
    }
    Serial.println(" :)");
    greenReceiving = false;
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
  } else if (blueIndex < maxChars && (millis() - blueStartTime) >= (blueIndex+1.5)*bitDuration) {
   //byte r = readByte(BLUE_PIN);
   int r = digitalRead(BLUE_PIN);
   blueMessage[blueIndex++] = r;
   /*
   if (r == '\n') {
     blueMessage[blueIndex] = '\0';
     printMessage("Blue", blueMessage, blueStartTime);
     blueReceiving = false;
   } 
  */
  } else if(blueReceiving && blueIndex>=maxChars){
    Serial.print("Blue Message!! ");
    for(int i=0; i<blueIndex; i++){
      Serial.print(blueMessage[i]);
      if(i%8==7){
        Serial.print(" ");
      }
    }
    Serial.println(" :)");
    blueReceiving = false;
  }
  //BLUE END

}

void printMessage(const char* label, char* msg, unsigned long startTime) {
 unsigned long endTime = micros();
 Serial.print(label); Serial.print(": ");
 Serial.println(msg);
 Serial.print("Time for "); Serial.print(label); Serial.print(": ");
 Serial.print(endTime - startTime); Serial.println(" us");
}

