#define CommandOff        0b000
#define CommandOn         0b001
#define CommandSwitch     0b010
#define CommandRandom     0b011
#define CommandFadeOut    0b100
#define CommandFlicker    0b101
#define CommandTwinkle    0b110
#define CommandBrightness 0b111

void processCommand(byte command, byte data) {
  switch (command) {
    case CommandOff:
      randomMode = RandomModeNone;
      analogWrite(PinLed, currentLight = 0);
      break;
    case CommandOn:
      randomMode = RandomModeNone;
      analogWrite(PinLed, currentLight = 255);
      break;
    case CommandSwitch:
      randomMode = RandomModeNone;
      analogWrite(PinLed, currentLight = (currentLight >= 128 ? 0 : 255));
      break;
    case CommandRandom:
      randomMode = RandomModeNone;
      analogWrite(PinLed, currentLight = random(256));
      break;
    case CommandFadeOut:
      randomMode = RandomModeFadeOut;
      analogWrite(PinLed, currentLight = random(256));
      break;
    case CommandFlicker:
      randomMode = RandomModeFlicker;
      analogWrite(PinLed, currentLight = 0);
      break;
    case CommandTwinkle:
      randomMode = RandomModeTwinkle;
      if ((twinkleUp = random(2))) {
        twinkleTarget = random(255) + 1;
        analogWrite(PinLed, currentLight = 0);
      } else {
        analogWrite(PinLed, currentLight = random(255) + 1);
      }
      break;
    case CommandBrightness:
      randomMode = RandomModeNone;
      analogWrite(PinLed, currentLight = (1 << data));
      break;
  }
}
