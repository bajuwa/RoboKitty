#include <SD.h>
//#include <MemoryFree.h>
#include "TuneManager.h"

// HARDWARE CONSTANTS
// pin of speaker/pezo
int const PIN_PEZO = 8;
int const PIN_CS = 4;
int const PIN_HARDWARE_SS = 10;

// File information
File tuneFile;
File root;
char inputChar = ' ';

// Buffered note array
int readNoteIndex = 0;
int writeNoteIndex = 0;
int tuneFreq[MAX_NOTE_BUFFER];
int tuneDur[MAX_NOTE_BUFFER];

// Timing
unsigned long previousMillis = 0;
unsigned long interval = 0;

// ABC Notation
boolean inBrackets = true;
boolean sharpIndicator = false;
boolean flatIndicator = false;
float meterValue = 1.0f;
float defaultNoteLength = 0.125f;
long defaultNoteDuration = 120;

// Extra information about ABC Notation:
// Middle C is represented as C 
// Note range: C,D,E,F,G,A,B,CDEFGABcdefgabc'd'e'f'g'a'b' (first index)
// ** If the note is 'sharp', then look as the second index
int frequencies[][2] = {
    {131, 139}, //C,
    {147, 156}, //D,
    {165, 165}, //E, (no sharp)
    {175, 185}, //F,
    {196, 208}, //G,
    {220, 233}, //A,
    {247, 247}, //B, (no sharp)
    {262, 277}, //C
    {294, 311}, //D
    {330, 330}, //E (no sharp)
    {349, 370}, //F
    {392, 415}, //G
    {440, 466}, //A
    {494, 494}, //B (no sharp)
    {523, 554}, //c
    {587, 622}, //d
    {659, 659}, //e (no sharp)
    {698, 740}, //f
    {784, 831}, //g
    {880, 932}, //a
    {988, 988}, //b (no sharp)
    {1047, 1109}, //c,
    {1175, 1245}, //d,
    {1319, 1319}, //e, (no sharp)
    {1397, 1480}, //f,
    {1568, 1661}, //g,
    {1760, 1865}, //a,
    {1976, 1976}, //b, (no sharp)
    {0, 0}        //rest
};

TuneManager::TuneManager() {
  Serial.println(F("Initializing TuneManager"));
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output
  // or the SD library functions will not work.
  pinMode(PIN_HARDWARE_SS, OUTPUT);
  
  previousMillis = millis();
  
  if (!SD.begin(PIN_CS)) {
    Serial.println(F("SD Card Initialization Failed"));
  } else {
    Serial.println(F("SD Card Initialized Successfully"));
  }
  
  // Open the root folder so we can rely on 'openNextFile' for our playlist 'shuffle'
  root = SD.open("/");
}

int getIntegerFromFileStream(File f, char* previewedChar) {
  int num = 0;
  while (48 <= *previewedChar && *previewedChar <= 57) {
    // Ascii range 48-57 represents digits 0-9
    num = (num*10) + atoi(previewedChar);
    *previewedChar = f.read();
  }
  //Serial.print(F("Found integer: "));
  //Serial.println(num);
  return num;
}

void getNextNote(File file, int* freq, int* dur) {
  while (true) {
    // Get the next char for next iteration if we haven't already got it
    // (sometimes we have already 'previewed' a char to check if the note is over or not)
    while (inputChar == ' ') inputChar = file.read();
    // If we reached the end of the file, abort finding the next note
    if (inputChar == EOF) return;
    // If we find double quotes, "escape" what is inside them
    if (inputChar == '"') {
      do {
        inputChar = file.read();
      } while (inputChar != '"');
    }
    
    // If we are in a 'header section'
    switch (inputChar) {
      
      case 'K':  // Key, also marks the end of the header
        Serial.println(F("Handling header: K - Key"));
        // Currently only supports default Key C major
        // Marks the end of the official header, so scroll forward til endline
        while (inputChar != '\n' && inputChar != ']') inputChar = file.read();
        Serial.println(F("Finished handling header: K - Key"));
        break;
      
      case 'M':  // Meter
        Serial.println(F("Handling header: M - Meter"));
        // Remove the expected ':' character
        inputChar = file.read();
        if (inputChar != ':') continue; // if the ':' isn't present, there is a formatting error
        inputChar = file.read(); // move past the ':'
        while (inputChar == ' ') inputChar = file.read();
        // Get the value for the meter from the fractional form (ex: 4/4 or 6/8)
        meterValue = (float)getIntegerFromFileStream(file, &inputChar);
        if (inputChar == '/') {
          inputChar = file.read(); // move past the '/'
          meterValue /= (float)getIntegerFromFileStream(file, &inputChar);
        }
        // Calculate the default note length based on the Meter
        defaultNoteLength = meterValue < 0.75f ? 0.0625f : 0.125f;
        // Calculate the default note duration based off of our (potentially new) note length
        // 240000 = 60 (seconds per minute) * 1000 milliseconds per second * 4 beats per whole note
        defaultNoteDuration = 240000 / defaultNoteDuration;
        // at this point, defaultNoteDuration is set to 'whole note', so we need to apply our current default note length
        defaultNoteDuration *= defaultNoteLength;
        Serial.print(F("New meter value: "));
        Serial.println(meterValue);
        Serial.print(F("New default note length: "));
        Serial.println(defaultNoteLength, 4);
        Serial.print(F("New default note duration: "));
        Serial.println(defaultNoteDuration);
        Serial.println(F("Finished handling header: M - Meter"));
        break;
      
      case 'Q':  // Tempo
        Serial.println(F("Handling header: Q - Tempo"));
        // Remove the expected ':' character
        inputChar = file.read();
        if (inputChar != ':') continue; // if the ':' isn't present, there is a formatting error
        inputChar = file.read(); // move past the ':'
        while (inputChar == ' ') inputChar = file.read();
        // Get the integer value for the tempo
        defaultNoteDuration = getIntegerFromFileStream(file, &inputChar);
        // if a '/' is present, it means that they have specified
        // which note to assign the tempo to, and we must make calculations
        // based on this assumption
        if (inputChar == '/') {
          inputChar = file.read();
          // Overall calculation:
          // Note length = [first int] / [second int]
          // Seconds per whole note beat = 4 * (60 / [Tempo])
          // Milliseconds per whole note = [seconds per beat] * 1000;
          // Final default note duration = [note length] * [milliseconds per whole note];
          // [Note Length] * [Milliseconds per note]
          double tempoNoteLength = (double)defaultNoteDuration / (double)getIntegerFromFileStream(file, &inputChar);
          Serial.print(F("Tempo: Note Length: "));
          Serial.println(tempoNoteLength, 4);
          // bypass the filler characters
          while (inputChar != '=') inputChar = file.read();
          inputChar = file.read();
          while (inputChar == ' ') inputChar = file.read();
          // the next int from the file will be our tempo/bpm for the given note length
          defaultNoteDuration = tempoNoteLength * (120000 / getIntegerFromFileStream(file, &inputChar));
          Serial.print(F("Tempo: default note duration: "));
          Serial.println(defaultNoteDuration);
        } else {
          // If '/' was not present, then they just gave us our tempo
          // ( we assume it is for the default note length )
          // 240000 = 60 (seconds per minute) * 1000 milliseconds per second * 4 beats per whole note
          defaultNoteDuration = 240000 / defaultNoteDuration;
          // at this point, defaultNoteDuration is set to 'whole note', so we need to apply our current default note length
          defaultNoteDuration *= (double)defaultNoteLength;
        }
        Serial.print(F("New default note duration: "));
        Serial.println(defaultNoteDuration);
        Serial.println(F("Finished handling header: Q - Tempo"));
        break;
      
      case 'L':  // Default Note Length
        Serial.println(F("Handling header: L - Note Length"));
        // Remove the expected ':' character
        inputChar = file.read();
        if (inputChar != ':') continue;
        inputChar = file.read();
        while (inputChar == ' ') inputChar = file.read();
        // Undo our default tempo
        defaultNoteDuration /= (double)defaultNoteLength;
        // Get the value for the length from the fractional form (ex: 4/4)
        defaultNoteLength = (float)getIntegerFromFileStream(file, &inputChar);
        if (inputChar == '/') {
          inputChar = file.read();
          defaultNoteLength /= (float)getIntegerFromFileStream(file, &inputChar);
        }
        // Reapply the (potentially new) note length to the tempo
        defaultNoteDuration *= (double)defaultNoteLength;
        Serial.print(F("New default note length: "));
        Serial.println(defaultNoteLength, 4);
        Serial.println(F("Finished handling header: L - Note Length"));
        break;
        
      case '[':
        inBrackets = true;
        Serial.println(F("Entering [] Brackets"));
        inputChar = file.read();
        break;
        
      case ']':
        inBrackets = false;
        Serial.println(F("Exiting [] Brackets"));
        inputChar = file.read();
        break;
      
      default:
        // If not, do a series of sequential checks for note information
    
        // reset defaults first though
        sharpIndicator = false;
        flatIndicator = false;
        int noteFreqIndex = -1;
        
        // Get the sharp/flat modifier (will recognize but pass over doubles)
        //Serial.println(F("Looking for sharp/flat modifiers"));
        while (inputChar == '^') {
          sharpIndicator = true;
          inputChar = file.read();
        }
        while (inputChar == '_') {
          flatIndicator = true;
          inputChar = file.read();
        }
        
        // Get the note (CDEFGAB)
        //Serial.println(F("Looking for note"));
        if (inputChar == 122) {
          // if z, map to 28 (rest)
          noteFreqIndex = 28;
        } else if (67 <= inputChar && inputChar <= 71) {
          // If CDEFG, map to 7-11
          noteFreqIndex = inputChar - 67 + 7;
        } else if (65 <= inputChar && inputChar <= 66) {
          // If AB, map to 12-13
          noteFreqIndex = inputChar - 65 + 12;
        } else if (99 <= inputChar && inputChar <= 103) {
          // If cdefg, map to 14-18
          noteFreqIndex = inputChar - 99 + 14;
        } else if (97 <= inputChar && inputChar <= 98) {
          // If ab, map to 19-20
          noteFreqIndex = inputChar - 97 + 19;
        }
        
        // If, for whatever reason, we still don't have a note, exit with a failure
        if (noteFreqIndex == -1) {
          inputChar = ' ';
          continue;
        }
        inputChar = file.read();
        
        // Get the octave modifier (skippable)
        //Serial.println(F("Looking for octave modifiers"));
        if (inputChar == '\'') {
          Serial.println(F("Found a ' - going up an octave"));
          noteFreqIndex += 7;
          inputChar = file.read();
        }    
        if (inputChar == ',') {
          Serial.println(F("Found a , - going down an octave"));
          noteFreqIndex -= 7;
          inputChar = file.read();
        }
        
        // Take our note frequency info and get the actual frequency
        if (flatIndicator) {
          // Flats can also become the below notes sharp freq
          sharpIndicator = true;
          noteFreqIndex = max(noteFreqIndex-1,0);
        }
        *freq = frequencies[noteFreqIndex][sharpIndicator ? 1 : 0];
        //Serial.print(F("Note Freq: "));
        //Serial.print(*freq);
        
        // Get the beat modifier
        *dur = defaultNoteDuration;
        // First see if we can find a multiplier (no preceding chars, just digits)
        int modifier = getIntegerFromFileStream(file, &inputChar);
        
        if (modifier > 0) {
          // If there was a digit, then it means we multiply our current duration by that number
          *dur *= modifier;
        }
        
        while (inputChar == '/') {
          // If a / is found, it means we divide our duration by the next number
          // If no number is provided, the default is /2
          modifier = getIntegerFromFileStream(file, &inputChar);
          if (modifier > 0) *dur /= modifier;
          else *dur /= 2;
          inputChar = file.read();
        }
        
        while (inputChar == '>') {
          // A > (aka a hornpipe) represents a 'dotted' note
          // (therefore adds half its current duration to itself)
          *dur += *dur/2;
          inputChar = file.read();
        }
        
        //Serial.print(F("Note Dur: "));
        //Serial.print(*dur);
        
        // If we are in a set of brackets, that means the music wants to
        // play multiple notes at once, which the arduino piezo does not support
        // So we must escape all remaining notes until the end brack
        if (inBrackets) {
          Serial.println(F("Ignoring rest of notes in stem..."));
          while (inputChar != ']') inputChar = file.read();
        }
        
        // Make sure to return once we have our next valid note
        return;
    }
  }
}

void TuneManager::addNotesToTune(int numOfNotesToAdd) {
  //Serial.print(F("Adding notes to tune: "));
  //Serial.println(numOfNotesToAdd);
  //Serial.print(F("Maximum buffer capacity: "));
  //Serial.println(MAX_NOTE_BUFFER);
  for (int i=0; i<numOfNotesToAdd; i++) {
    // Immediately abort loading more notes if we have run out of space in the buffer
    if ((writeNoteIndex+1)%MAX_NOTE_BUFFER == readNoteIndex) {
      //Serial.print(F("Reached maximum buffer capacity, aborting adding new notes: "));
      //Serial.println(MAX_NOTE_BUFFER);
      return;
    }
    
    // Get the next freq and duration
    if (tuneFile.available()) {
      // Get our next note from the file
      int nextNoteFreq = 0;
      int nextNoteDur = 0;
      // keep asking for the next note until it returns a 'success'
      getNextNote(tuneFile, &nextNoteFreq, &nextNoteDur);
      
      // Check to make sure that we have a note (in case it reached end of file)
      if (nextNoteDur != 0) {
        // Otherwise, add our decoded note to the ongoing tune
        tuneFreq[writeNoteIndex] = nextNoteFreq;
        tuneDur[writeNoteIndex] = nextNoteDur;
        // Increment the index of where we are writing our notes
        writeNoteIndex = (writeNoteIndex+1)%MAX_NOTE_BUFFER;
        continue;
      } 
    } 
    // If we have reached the end of the song, the file will no longer be available
    // Close it and return it to null so that a new tune file can be played
    //Serial.println(F("Reached end of tune, closing file"));
    tuneFile.close();
    break;
  }
}

/*
int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}
*/

void TuneManager::playTunes() {
  // Make sure we have a song to play
  if (!tuneFile) {
    // Proceed to the next song if it exists
    //Serial.println(F("opening next file"));
    tuneFile = root.openNextFile();
    // If we got a new tune, load up some notes from it
    if (tuneFile) {
      Serial.print(F("Loaded next tune file: "));
      Serial.println(tuneFile.name());
      // Reset all data to default for new tune
      inBrackets = true;
      inputChar = ' ';
      sharpIndicator = false;
      flatIndicator = false;
      addNotesToTune(MAX_NOTE_BUFFER);
    }
  }
  // play the song by iterating over the notes at given intervals:
  unsigned long currentMillis = millis();
  if (readNoteIndex != writeNoteIndex && (currentMillis - previousMillis > interval) ) {
    previousMillis = currentMillis;   
  
    // If the current note isn't a 'rest' beat, 
    // then play the note for the alloted duration (milliseconds)
    tone(PIN_PEZO, tuneFreq[readNoteIndex], tuneDur[readNoteIndex]);
    
    // Set how long to wait until next note 
    // (namely the length of the currently playing note plus some rest time to distinguish between repeated notes)
    interval = tuneDur[readNoteIndex];
    
    // Remove the note we just played so it doesn't repeat
    readNoteIndex = (readNoteIndex+1)%MAX_NOTE_BUFFER;
  } else {
     // If we can't play a note yet, might as well buffer some of the upcoming notes
     //Serial.println("Not playing a note, so add to buffer");
     addNotesToTune(MIN_NOTE_BUFFER);
  } 
}

void TuneManager::loadSound(SOUNDS sound) {
  switch (sound) {
    case HAPPY:
      // Mario One-Up 
      //this->loadTune("330 150-392 150-587 150-494 150-523 150-698 150");
      break;
    case SAD:
      // Mario Death
      //this->loadTune("1047 50-1109 50-1175 50-0 450-247 150-698 150-0 150-698 150-698 200-659 200-587 200-523 150-330 150-0 150-330 150-262 150");
      break;
    case PANIC:
      //this->loadTune("1000 200-1300 200-1000 200-1300 200");
      break;
    case BORED:
      //this->loadTune("1000 200-1300 200-1000 200-1300 200");
      break;
  }
}



