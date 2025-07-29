#define RED_PIN   2
#define GREEN_PIN 3
#define BLUE_PIN  4

const int bitDuration = 25;           // milliseconds per bit (~9600 baud)
const int maxChars    = 8 * 100;       // total bits per message
const int msgBytes    = (maxChars + 7) / 8;  // =69 bytes, to hold 552 bit1
// pack bits into bytes
uint8_t redMessage[msgBytes];
uint8_t greenMessage[msgBytes];
uint8_t blueMessage[msgBytes];

int   redCurrentChar[8];
char  redWords[maxChars/8];

int redIndex   = 0, greenIndex   = 0, blueIndex   = 0;
bool redReceiving   = false, greenReceiving   = false, blueReceiving   = false;
unsigned long redStartTime    = 0, greenStartTime    = 0, blueStartTime    = 0;
unsigned long redStartBitTime = 0, greenStartBitTime = 0, blueStartBitTime = 0;
bool redStartBit   = false, greenStartBit   = false, blueStartBit   = false;
bool redEnd = false;

// helper to set/get a single bit in our uint8_t[] buffer
inline void setBit(uint8_t *buf, int idx, bool v) {
  if (v) buf[idx >> 3] |=  (1 << (idx & 7));
  else   buf[idx >> 3] &= ~(1 << (idx & 7));
}
inline bool getBit(const uint8_t *buf, int idx) {
  return (buf[idx >> 3] >> (idx & 7)) & 1;
}

char bitsToChar(int bits[8]) {
  int value = 0;
  for (int i = 0; i < 8; i++) {
    value += bits[i] * (1 << i);
  }
  return char(value);
}

bool isEndSignal(int bits[8]){
  bool isEnd = true;
  for(int i=0; i<8; i++){
    if (bits[i]==0){
      isEnd = false;
      break;
    }
  }
  return isEnd;
}

void setup() {
  Serial.begin(9600);
  pinMode(RED_PIN,   INPUT);
  pinMode(GREEN_PIN, INPUT);
  pinMode(BLUE_PIN,  INPUT);
  Serial.println("READY");
}

void loop() {
  ////// RED CHANNEL ///////
  if (!redReceiving) {
    if (digitalRead(RED_PIN) == 0 && !redStartBit) {
      redStartBit     = true;
      redStartBitTime = millis();
      Serial.println("RED Reading Start Bit");
    } else if (digitalRead(RED_PIN) == 1) {
      redStartBit     = false;
      redStartBitTime = millis();
    }

    if (millis() - redStartBitTime >= 7 * bitDuration) {
      redReceiving   = true;
      redStartTime   = millis();
      redIndex       = 0;
      // clear buffer
      memset(redMessage, 0, msgBytes);
      Serial.println("RED START");
      Serial.println(millis());
    }

  } else if (redIndex < maxChars && (millis() - redStartTime) >= (redIndex + 1.5) * bitDuration) {
    bool bit = digitalRead(RED_PIN);
    setBit(redMessage, redIndex, bit);

    // also build up current char
    redCurrentChar[redIndex & 7] = bit;
    if ((redIndex & 7) == 7) {
      redWords[(redIndex >> 3)] = bitsToChar(redCurrentChar);
      if(isEndSignal(redCurrentChar)){
        redEnd = true;
      }
    }
    redIndex++;

  } else if (redEnd || (redReceiving && redIndex >= maxChars)) {
    Serial.println(millis());
    Serial.print("Red Message bits: ");
    for (int i = 0; i < redIndex; i++) {
      Serial.print(getBit(redMessage, i));
      if ((i & 7) == 7) Serial.print(' ');
    }
    Serial.println();
    redEnd = false;
    /*
    Serial.print("Red Message chars: ");
    for (int i = 0; i < maxChars/8; i++) {
      Serial.print(redWords[i]);
    }
    Serial.println();
    */
    redReceiving = false;
  }

  ////// GREEN CHANNEL ///////
  if (!greenReceiving) {
    if (digitalRead(GREEN_PIN) == 0 && !greenStartBit) {
      greenStartBit     = true;
      greenStartBitTime = millis();
      Serial.println("GREEN Reading Start Bit");
    } else if (digitalRead(GREEN_PIN) == 1) {
      greenStartBit     = false;
      greenStartBitTime = millis();
    }

    if (millis() - greenStartBitTime >= 7 * bitDuration) {
      greenReceiving   = true;
      greenStartTime   = millis();
      greenIndex       = 0;
      memset(greenMessage, 0, msgBytes);
      Serial.println("GREEN START");
    }

  } else if (greenIndex < maxChars && (millis() - greenStartTime) >= (greenIndex + 1.5) * bitDuration) {
    bool bit = digitalRead(GREEN_PIN);
    setBit(greenMessage, greenIndex, bit);
    greenIndex++;

  } else if (greenReceiving && greenIndex >= maxChars) {
    Serial.print("Green Message bits: ");
    for (int i = 0; i < greenIndex; i++) {
      Serial.print(getBit(greenMessage, i));
      if ((i & 7) == 7) Serial.print(' ');
    }
    Serial.println();
    greenReceiving = false;
  }

  ////// BLUE CHANNEL ///////
  if (!blueReceiving) {
    if (digitalRead(BLUE_PIN) == 0 && !blueStartBit) {
      blueStartBit     = true;
      blueStartBitTime = millis();
      Serial.println("BLUE Reading Start Bit");
    } else if (digitalRead(BLUE_PIN) == 1) {
      blueStartBit     = false;
      blueStartBitTime = millis();
    }

    if (millis() - blueStartBitTime >= 7 * bitDuration) {
      blueReceiving   = true;
      blueStartTime   = millis();
      blueIndex       = 0;
      memset(blueMessage, 0, msgBytes);
      Serial.println("BLUE START");
    }

  } else if (blueIndex < maxChars && (millis() - blueStartTime) >= (blueIndex + 1.5) * bitDuration) {
    bool bit = digitalRead(BLUE_PIN);
    setBit(blueMessage, blueIndex, bit);
    blueIndex++;

  } else if (blueReceiving && blueIndex >= maxChars) {
    Serial.print("Blue Message bits: ");
    for (int i = 0; i < blueIndex; i++) {
      Serial.print(getBit(blueMessage, i));
      if ((i & 7) == 7) Serial.print(' ');
    }
    Serial.println();
    blueReceiving = false;
  }
}