#include <MIDI.h> 

//Create an instance of the library with default name, serial port and settings
MIDI_CREATE_DEFAULT_INSTANCE();

struct MySettings : public midi::DefaultSettings {
  static const long BaudRate = 115200;
};

#define MAX_KEYS 8
#define UNSET -1

uint8_t keyPins[MAX_KEYS]  = {39, 41, 43, 45, 47, 49, 51, 53};

// Tone duration based on the amount of travel time the mallet needs to contact the key
const long toneDuration = 35;  
long timerState[MAX_KEYS] = {UNSET, UNSET, UNSET, UNSET, UNSET, UNSET, UNSET, UNSET};


void setup() {
  
  initialisePins();
  
  MIDI.begin(MIDI_CHANNEL_OMNI); 
    
  // OMNI sets it to listen to all channels.. MIDI.begin(2) would set it 
  // to respond to notes on channel 2 only.

  // This command tells the Midi Library which function you want to call when a NOTE ON command
  // is received. In this case it's "MyHandleNoteOn".
  MIDI.setHandleNoteOn(MyHandleNoteOn); 
  
  // NoteOff not called since keys should only be struck momentarily  
}

void loop() { // Main loop
  MIDI.read(); // Continuously check if Midi data has been received.
  updatePinStatus();
}


void initialisePins () {
  for (uint8_t i = 0; i < MAX_KEYS; i++) { 
    pinMode (keyPins[i], OUTPUT);
  }
}

// MyHandleNoteON is the function that will be called by the Midi Library
// when a MIDI NOTE ON message is received.
// It will be passed bytes for Channel, Pitch, and Velocity
void MyHandleNoteOn(byte channel, byte pitch, byte velocity) {

  unsigned long currentMillis = millis();
  // C Major, starting at middle C
  // 60, 62, 64, 65, 67, 69, 71, 72
  if (pitch == 60) {
    digitalWrite(39,HIGH); 
    timerState[0] = currentMillis;
  } 
  
  if (pitch == 62) {
    digitalWrite(41,HIGH); 
    timerState[1] = currentMillis;
  }

  if (pitch == 64) {
    digitalWrite(43,HIGH); 
    timerState[2] = currentMillis;
  } 
  
  if (pitch == 65) {
    digitalWrite(45,HIGH); 
    timerState[3] = currentMillis;
  } 
  
  if (pitch == 67) {
    digitalWrite(47,HIGH);  
    timerState[4] = currentMillis;
  } 
  
  if (pitch == 69) {
    digitalWrite(49,HIGH);  
    timerState[5] = currentMillis;
  }

  if (pitch == 71) {
    digitalWrite(51,HIGH); 
    timerState[6] = currentMillis;
  } 
  
  if (pitch == 72) {
    digitalWrite(53,HIGH); 
    timerState[7] = currentMillis;
  } 
}

// Check all pin statuses to see if enough time has passed since the 
void updatePinStatus() {
  unsigned long currentMillis = millis();

  for (uint8_t i = 0; i < MAX_KEYS; i++) { 
    if (timerState[i] == UNSET) {
      continue;
    }
    
    if (currentMillis - timerState[i] >= toneDuration) {
       digitalWrite(keyPins[i], LOW);
       timerState[i] = UNSET;
    }
  }

}
