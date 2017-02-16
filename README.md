# Stranger christmas lights

This is an attempt to recreate christmas lights seen in a Netflix series. The idea is to use Bluetooth LE master controller to connect to a phone and receive text messages.

Each LED light will feature ATtiny45 microcontroller to control the light. Having separace MCU at each LED also opens additional possibilities to do random modes, like flickering or twinkle.

Master microcontroller uses single wire to communicate command to LED. Depending on an address MCU at LED will either perform command or pass it along. This allows for minimal wiring where only four wires connect each LED: VCC, ground, signal in, signal out.
