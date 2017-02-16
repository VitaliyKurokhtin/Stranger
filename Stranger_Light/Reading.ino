#define MaxLowMks  75
#define MaxHighMks 125

#define MinCommandWithData 0b111000000

unsigned long lastStateMks = 0;
int lastState = LOW;

//signed char bitsReceived = WaitingForPrefix;
int bits = 0;

int readBits() {
  unsigned long mks = micros();
  int state = digitalRead(PinInput);
  if (bitsReceived == WaitingForLow) {
    // Waiting for line to get HIGH
    if (state == LOW) {
      bitsReceived = WaitingForPrefix; 
    }
  } else if (bitsReceived == WaitingForPrefix) {
    // Waiting for line to get HIGH
    if (state == HIGH) {
      bitsReceived = SkippingPrefix; 
    }
  } else if (bitsReceived == SkippingPrefix) {
    if (state == LOW) {
      bits = bitsReceived = 0;
      lastState = state;
      lastStateMks = mks;
    }
  } else {
    unsigned long delta = mks - lastStateMks;

    if (state != lastState){
      if (delta > MaxLowMks) {
        bits |= 1 << bitsReceived;
      }
      
      bitsReceived++;
      
      if (bitsReceived == AddressCommandLength) {
        if (bits < MinCommandWithData) {
          bitsReceived = state == HIGH ? WaitingForLow : WaitingForPrefix;
          return bits;
        }
      } else if (bitsReceived == MessageLength) {
        bitsReceived = state == HIGH ? WaitingForLow : WaitingForPrefix;
        return bits;
      }
      
      lastState = state;
      lastStateMks = mks;
    } else if (delta > MaxHighMks) {
      // Line was in same state too long, resetting reading
      bitsReceived = WaitingForPrefix;
    }
  }
  
  return -1;
}

/*
// Manchester code

int halfPeriod = 0;
int readBits() {
  unsigned long mks = micros();
  int state = digitalRead(PinInput);

  if (bitsReceived == WaitingForLow) {
    // Waiting for line to get LOW
    if (state == LOW) {
      bitsReceived = WaitingForPrefix;
    }
  } else if (bitsReceived == WaitingForPrefix) {
    // Waiting for prefix
    if (state == HIGH) {
      bitsReceived = ReadingPrefix;
      periodStart = mks;
    }
  } else if (bitsReceived == ReadingPrefix) {
    // Reading prefix
    if (state == LOW) {
      bits = bitsReceived = 0;
      halfPeriod = mks - periodStart;
      periodStart = mks + halfPeriod + halfPeriod / 4;
      lastState = LOW;
    }
  } else if (mks > periodStart) {
    unsigned long delta = mks - periodStart;
    if (lastState == -1) {
      lastState = digitalRead(PinInput);
    } else if (delta > halfPeriod + 3 * halfPeriod / 4) {
      // Switch did not happen, reset
      bitsReceived = state == HIGH ? WaitingForLow : WaitingForPrefix;
    } else if (state != lastState) {
      if (state == HIGH) {
        bits |= 1 << bitsReceived;
      }
      
      bitsReceived++;
      if (bitsReceived == 9) {
        //if (bitsReceived < MinCommandWithData) {
          bitsReceived = state == HIGH ? WaitingForLow : WaitingForPrefix;
          return bits;
        //}
      } else if (bitsReceived == 13) {
        bitsReceived = state == HIGH ? WaitingForLow : WaitingForPrefix;
        return bits;
      }

      lastState = -1;
      periodStart = mks + halfPeriod + halfPeriod / 4;
    }
  }

  return -1;
}
*/

