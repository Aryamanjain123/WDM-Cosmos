#define RED_PIN   2
#define GREEN_PIN 3
#define BLUE_PIN  4

const unsigned int bitDuration   = 15;              // milliseconds per bit (~9600 baud)
const unsigned long bitDurationUs = bitDuration * 1000UL;  // microseconds per bit
const int  maxChars    = 8 * 164;                   // total bits per message
const int  msgBytes    = (maxChars + 7) / 8;        // =69 bytes, to hold 552 bits

uint8_t redMessage[msgBytes];
uint8_t greenMessage[msgBytes];
uint8_t blueMessage[msgBytes];

int   redCurrentChar[8];
char  redWords[maxChars / 8];

int   greenCurrentChar[8];
char  greenWords[maxChars / 8];

int   blueCurrentChar[8];
char  blueWords[maxChars / 8];

int redIndex   = 0, greenIndex   = 0, blueIndex   = 0;
bool redReceiving   = false, greenReceiving   = false, blueReceiving   = false;
unsigned long redStartTime    = 0, greenStartTime    = 0, blueStartTime    = 0;
unsigned long redStartBitTime = 0, greenStartBitTime = 0, blueStartBitTime = 0;
bool redStartBit   = false, greenStartBit   = false, blueStartBit   = false;
bool redEnd = false, greenEnd = false, blueEnd = false;

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
    value |= bits[i] << i;
  }
  return char(value);
}

char binaryToChar(String byteStr) {
  return (char)strtol(byteStr.c_str(), NULL, 2);
}

void printBufferAsText(uint8_t *buffer, int totalBits) {
  int totalBytes = totalBits / 8;
  Serial.print("Decoded text: ");
  for (int byteIndex = 0; byteIndex < totalBytes; byteIndex++) {
    String byteStr = "";
    for (int bit = 7; bit >= 0; bit--) {
      if (getBit(buffer, byteIndex * 8 + bit)) {
        byteStr = "1" + byteStr;
      } else {
        byteStr = "0" + byteStr;
      }
    }
    Serial.print(binaryToChar(byteStr));
    if (byteStr == "11111111") {
      Serial.print(" [END]");
      break;
    }
  }
  Serial.println();
}

bool isEndSignal(int bits[8]) {
  for (int i = 0; i < 8; i++) {
    if (bits[i] == 0) return false;
  }
  return true;
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
      redStartBitTime = micros();
      Serial.println("RED Reading Start Bit");
    } else if (digitalRead(RED_PIN) == 1) {
      redStartBit     = false;
      redStartBitTime = micros();
    }

    if (micros() - redStartBitTime >= 7 * bitDurationUs) {
      redReceiving = true;
      redStartTime = micros();
      redIndex     = 0;
      memset(redMessage, 0, msgBytes);
      Serial.println("RED START");
      Serial.println(micros());
    }

  } else if (redIndex < maxChars
             && (micros() - redStartTime) >= ((redIndex + 1) * bitDurationUs + bitDurationUs / 2)) {
    bool bit = digitalRead(RED_PIN);
    setBit(redMessage, redIndex, bit);
    redCurrentChar[redIndex & 7] = bit;
    if ((redIndex & 7) == 7) {
      redWords[redIndex >> 3] = bitsToChar(redCurrentChar);
      if (isEndSignal(redCurrentChar)) redEnd = true;
    }
    redIndex++;

  } else if (redEnd || (redReceiving && redIndex >= maxChars)) {
    Serial.println(micros());
    Serial.print("Red Message bits: ");
    for (int i = 0; i < redIndex; i++) {
      Serial.print(getBit(redMessage, i));
      if ((i & 7) == 7) Serial.print(' ');
    }
    Serial.println();
    printBufferAsText(redMessage, redIndex);
    Serial.println();
    redReceiving = false;
    redEnd = false;
  }

  ////// GREEN CHANNEL ///////
  if (!greenReceiving) {
    if (digitalRead(GREEN_PIN) == 0 && !greenStartBit) {
      greenStartBit     = true;
      greenStartBitTime = micros();
      Serial.println("GREEN Reading Start Bit");
    } else if (digitalRead(GREEN_PIN) == 1) {
      greenStartBit     = false;
      greenStartBitTime = micros();
    }

    if (micros() - greenStartBitTime >= 7 * bitDurationUs) {
      greenReceiving = true;
      greenStartTime = micros();
      greenIndex     = 0;
      memset(greenMessage, 0, msgBytes);
      Serial.println("GREEN START");
    }

  } else if (greenIndex < maxChars
             && (micros() - greenStartTime) >= ((greenIndex + 0.75) * bitDurationUs + bitDurationUs / 2)) {
    bool bit = digitalRead(GREEN_PIN);
    setBit(greenMessage, greenIndex, bit);
    greenCurrentChar[greenIndex & 7] = bit;
    if ((greenIndex & 7) == 7) {
      greenWords[greenIndex >> 3] = bitsToChar(greenCurrentChar);
      if (isEndSignal(greenCurrentChar)) greenEnd = true;
    }
    greenIndex++;

  } else if (greenEnd || (greenReceiving && greenIndex >= maxChars)) {
    Serial.print("Green Message bits: ");
    for (int i = 0; i < greenIndex; i++) {
      Serial.print(getBit(greenMessage, i));
      if ((i & 7) == 7) Serial.print(' ');
    }
    Serial.println();
    printBufferAsText(greenMessage, greenIndex);
    Serial.println();
    greenReceiving = false;
    greenEnd = false;
  }

  ////// BLUE CHANNEL ///////
  if (!blueReceiving) {
    if (digitalRead(BLUE_PIN) == 0 && !blueStartBit) {
      blueStartBit     = true;
      blueStartBitTime = micros();
      Serial.println("BLUE Reading Start Bit");
    } else if (digitalRead(BLUE_PIN) == 1) {
      blueStartBit     = false;
      blueStartBitTime = micros();
    }

    if (micros() - blueStartBitTime >= 7 * bitDurationUs) {
      blueReceiving = true;
      blueStartTime = micros();
      blueIndex     = 0;
      memset(blueMessage, 0, msgBytes);
      Serial.println("BLUE START");
    }

  } else if (blueIndex < maxChars
             && (micros() - blueStartTime) >= ((blueIndex + 0.70) * bitDurationUs + bitDurationUs / 2)) {
    bool bit = digitalRead(BLUE_PIN);
    setBit(blueMessage, blueIndex, bit);
    blueCurrentChar[blueIndex & 7] = bit;
    if ((blueIndex & 7) == 7) {
      blueWords[blueIndex >> 3] = bitsToChar(blueCurrentChar);
      if (isEndSignal(blueCurrentChar)) blueEnd = true;
    }
    blueIndex++;

  } else if (blueEnd || (blueReceiving && blueIndex >= maxChars)) {
    Serial.print("Blue Message bits: ");
    for (int i = 0; i < blueIndex; i++) {
      Serial.print(getBit(blueMessage, i));
      if ((i & 7) == 7) Serial.print(' ');
    }
    Serial.println();
    printBufferAsText(blueMessage, blueIndex);
    Serial.println();
    blueReceiving = false;
    blueEnd = false;
  }
}