#include <MIDI.h>

// Define the MIDI input pin, MIDI out channel, and the two relay output pins

#define RELAY_PIN_1 3
#define RELAY_PIN_2 2

// Create MIDI, relay, and MIDI out objects
MIDI_CREATE_DEFAULT_INSTANCE();
const int relay_pins[] = { RELAY_PIN_1, RELAY_PIN_2 };

void controlChange(byte channel, byte number, byte value) {

  if (number == 13 || number == 10) {
    // Check if the relay pins are already on
    bool allOn = true;
    for (int i = 0; i < 2; i++) {
      if (digitalRead(relay_pins[i]) != HIGH) {
        allOn = false;
        break;
      }
    }

    // If the relay pins are not already on, turn them on and start the timer to reset them
    if (!allOn) {
      for (int i = 0; i < 2; i++) {
        digitalWrite(relay_pins[i], HIGH);
      }
    }
  }

  if ((number == 92 || number == 0) && value == 0) {
     for (int i = 0; i < 2; i++) {
        digitalWrite(relay_pins[i], LOW);
      }
  }
}


void setup() {
  // Initialize the MIDI, relay, and MIDI out objects

  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.setHandleControlChange(controlChange);

  for (int i = 0; i < 2; i++) {
    pinMode(relay_pins[i], OUTPUT);
  }
}

void loop() {
  // Check for MIDI messages
  MIDI.read();
}
