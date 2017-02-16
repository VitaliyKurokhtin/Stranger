#define PinLed    0
#define PinInput  4
#define PinOutput 3

#define AddressCommandLength  9
#define MessageLength        13

#define AddressMask 0b0000000111111
#define CommandMask 0b0000111000000
#define DataMask    0b1111000000000

#define LedCount 50

#define RandomModeNone    0
#define RandomModeFlicker 1
#define RandomModeTwinkle 2
#define RandomModeFadeOut 3

#define WaitingForLow    -3
#define WaitingForPrefix -2
#define SkippingPrefix   -1

void setup() {
  pinMode(PinLed, OUTPUT);
  pinMode(PinOutput, OUTPUT);
  pinMode(PinInput, INPUT);

  analogWrite(PinLed, 0);
  digitalWrite(PinOutput, LOW);

  randomSeed(analogRead(1));
}

signed char bitsReceived = WaitingForPrefix;
byte randomMode = RandomModeNone;
int currentLight = 0;
byte twinkleTarget;
bool twinkleUp;

//unsigned long testMks = 0;

void loop() {
  /*
  unsigned long mks = micros();
  if (testMks) {
    testMks = mks - testMks;
    
    for (byte shift = 0; shift < 32; shift++) {
      digitalWrite(PinLed, 255);
      if (testMks & (1 << shift)) {
        delay(900);
      }
      delay(300);
      digitalWrite(PinLed, 0);
      delay(300);
    }
    delay(5000);
    testMks = 0;
  } else {
    testMks = mks;
  }
  return;
  */
  int bits = readBits();

  if (bits >= 0) {
    byte address = GetAddress(bits);

    if (address == 0) {
      processCommand(GetCommand(bits), GetData(bits));
    } else if (address == 0b111111) {
      processCommand(GetCommand(bits), GetData(bits));
      writeBits(bits, MessageLength);
    } else if (address >= LedCount) {
      if (random(MessageLength + 2) < (address - LedCount)) {
        processCommand(GetCommand(bits), GetData(bits));
      }
      writeBits(bits, MessageLength);
    } else {
      writeBits((bits & DataMask) | (bits & CommandMask) | (address - 1), MessageLength);
    }
  }

  if (bitsReceived <= WaitingForPrefix && randomMode != RandomModeNone) {
    switch (randomMode) {
      case RandomModeFlicker:
        if (random(100) >= 97) {
          if (random(277) < currentLight) {
            currentLight = -random(20);
          } else if (currentLight < 256) {
            currentLight++;
          }
          
          analogWrite(PinLed, currentLight >= 0 ? currentLight : 0);
        }
        break;
      case RandomModeTwinkle:
        if (random(100) >= 99) {
          if (currentLight == 0) {
            twinkleUp = true;
            twinkleTarget = random(255) + 1;
          } else if (currentLight == twinkleTarget) {
            twinkleUp = false;
          }
  
          analogWrite(PinLed, currentLight += twinkleUp ? 1 : -1);
        }
        break;
      case RandomModeFadeOut:
        if (random(100) >= 98) {
          if (currentLight == 0) {
            currentLight = 256;
          }
  
          analogWrite(PinLed, --currentLight);
        }
        break;
    }
  }
}

byte GetAddress(int bits) {
  return bits & AddressMask;
}

byte GetCommand(int bits) {
  return (bits & CommandMask) >> 6;
}

byte GetData(int bits) {
  return (bits & DataMask) >> 9;
}
