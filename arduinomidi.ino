#include <MIDI.h>

// Define the MIDI input pin, MIDI out channel, and the two relay output pins

#define RELAY_PIN_1 4
#define RELAY_PIN_2 5

unsigned long disable_time = 0;
bool touching = false;

// Create MIDI, relay, and MIDI out objects
MIDI_CREATE_DEFAULT_INSTANCE();
const int relay_pins[] = { RELAY_PIN_1, RELAY_PIN_2 };

void controlChange(byte channel, byte number, byte value) {
  if (number == 13 || number == 10) {
    // Check if the relay pins are already on
    bool allOn = true;
    touching = true;
    for (int i = 0; i < 2; i++) {
      if (digitalRead(relay_pins[i]) != HIGH) {
        allOn = false;
        break;
      }
    }

    if (!allOn) {
      for (int i = 0; i < 2; i++) {
        digitalWrite(relay_pins[i], HIGH);
      }
    }
  }

  if ((number == 92 || number == 0) && value == 0) {
    touching = false;
    int perc = analogRead(A0);  // Read the analog input value
    int delayTime = 0;

    if (perc == 1027) {
      delayTime = 10000;  // Set delay time to 10 seconds
    } else {
      delayTime = map(perc, 0, 1023, 0, 10000);  // Scale the input value to the delay time range
    }
    disable_time = millis() + delayTime;
  }
}

void setup() {
  // Initialize the MIDI, relay, and MIDI out objects
  Serial.begin(9600);
  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.setHandleControlChange(controlChange);

  for (int i = 0; i < 2; i++) {
    pinMode(relay_pins[i], OUTPUT);
    digitalWrite(relay_pins[i], LOW);
  }

  // Initialize disable_time
  disable_time = millis();
}

void loop() {
  // Check for MIDI messages
  MIDI.read();

  if (millis() >= disable_time && touching == false) {
    for (int i = 0; i < 2; i++) {
      digitalWrite(relay_pins[i], LOW);
    }
  }
}
