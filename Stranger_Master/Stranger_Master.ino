#define PinOutput 2
#define LowMks 50
#define HighMks 100

#define MinCommandWithData 0b111

void setup() {
  pinMode(PinOutput, OUTPUT);
  digitalWrite(PinOutput, LOW);
  Serial.begin(115200);
  randomSeed(analogRead(0));
}

unsigned long ms = 0;
byte mode = 0;
byte var = 0;

void loop() {
  if (millis() - ms > 10000) {
    ms = millis();
    var = 0;
    if (++mode > 8) {
      mode = 0;
    }
  }

  switch (mode) {
    case 0:
      send(0b111111, var = !var, 0);
      delay(1000);
      break;
    case 1:
      send(57, 0b010, 0);
      delay(500);
      break;
    case 2:
      send(0b111111, 0b010, 0);
      delay(500);
      break;
    case 3:
      send(0b111111, 0b011, 0);
      delay(100);
      break;
    case 4:
      nightRider();
      break;
    case 5:
      send(0b111111, 0b111, ++var);
      delay(100);
      break;
    case 6:
      send(0b111111, 0b100, 0);
      delay(10000);
      break;
    case 7:
      send(0b111111, 0b101, 0);
      delay(10000);
      break;
    case 8:
      send(0b111111, 0b110, 0);
      delay(20000);
      break;
  }
}

int dir = 1;
int len = 3;
int shift = 0;
void nightRider() {
  for (int index = 0; index < 5; index++) {
    byte bits = index;
    delay(10);
    if (index >= shift && index < (shift + len)) {
      send(index, 1, 0);
    } else {
      send(index, 0, 0);
    }
  }
  delay(100);
  
  shift += dir;
  if (shift == -len + 1) {
    dir = 1;
  } else if (shift == 4) {
    dir = -1;
  }
}

void send(int address, int command, int data) {
  writeBits(address | (command << 6) | (data << 9), (command >= MinCommandWithData) ? 13 : 9);
}

int writeBits(int bits, byte count) {
  digitalWrite(PinOutput, HIGH);
  delayMicroseconds(HighMks * 3);

  int state = HIGH;
  for (byte shift = 0; shift < count; shift++) {
    state = state == LOW ? HIGH : LOW;
    digitalWrite(PinOutput, state);
    delayMicroseconds((bits & (1 << shift)) ? HighMks : LowMks);
  }

  if (state == LOW) {
    digitalWrite(PinOutput, HIGH);
    delayMicroseconds(LowMks);
  }
  digitalWrite(PinOutput, LOW);
}

/*
// Manchester code
void send(byte address, byte command, byte data) {
  digitalWrite(PinOutput, HIGH);
  delayMicroseconds(Period / 2);
  digitalWrite(PinOutput, LOW);
  delayMicroseconds(Period / 2);

  writeBits(address, 6);
  writeBits(command, 3);
  if (data) {
    writeBits(data, 4);
  }
}

void writeBits(int bits, byte count) {
  for (byte shift = 0; shift < count; shift++) {
    byte value = bits & (1 << shift);
    digitalWrite(PinOutput, value ? LOW : HIGH);
    delayMicroseconds(Period / 2);
    digitalWrite(PinOutput, value ? HIGH : LOW);
    delayMicroseconds(Period / 2);
  }
}
*/

