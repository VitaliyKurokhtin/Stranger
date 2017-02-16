#define LowMks   50
#define HighMks 100

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
