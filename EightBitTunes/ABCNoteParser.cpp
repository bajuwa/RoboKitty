#include "ABCNoteParser.h"

// ABC Notation
char inputChar;
boolean inBrackets;
float meterValue;
float defaultNoteLength;
long beatsPerMinute;
long defaultNoteDuration;

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

ABCNoteParser::ABCNoteParser() {
  reset();
}

void ABCNoteParser::reset() {
  inputChar = ' ';
  inBrackets = false;
  meterValue = 1.0f;
  defaultNoteLength = 0.125f;
  beatsPerMinute = 120;
  defaultNoteDuration = 0;
}

void ABCNoteParser::getNextNote(Stream* str, int* freq, int* dur) {
  while (true) {
    // Get the next char for next iteration if we haven't already got it
    // (sometimes we have already 'previewed' a char to check if the note is over or not)
    skipCharacters(str, &inputChar, " ");
    // If we reached the end of the str, abort finding the next note
    if (inputChar == EOF) return;
    // If we find double quotes or +, "escape" what is inside them
    if (inputChar == '"') {
      inputChar = str->read();
      skipCharactersUntil(str, &inputChar, "\"");
    }
    if (inputChar == '+') {
      inputChar = str->read();
      skipCharactersUntil(str, &inputChar, "+");
    }
    
    // If we are in a 'header section'
    switch (inputChar) {
      // Ignore all unsupported header lines
      case 'H':
      case 'I':
      case 'J':
      case 'N':
      case 'O':
      case 'P':
      case 'R':
      case 'S':
      case 'T':
      case 'U':
      case 'V':
      case 'W':
      case 'X':
      case 'Y':
      case 'Z':
      case '%':
        skipCharactersUntil(str, &inputChar, "\n");
        break;
        
      case 'K':  // Key, also marks the end of the header
        Serial.println(F("Handling header: K - Key"));
        // Currently only supports default Key C major
        // Marks the end of the official header, so scroll forward til endline
        skipCharactersUntil(str, &inputChar, "\n]");
        
        Serial.println(F("Final Settings: "));
        Serial.print(F("Meter: "));
        Serial.println(meterValue);
        Serial.print(F("Note Length: "));
        Serial.println(defaultNoteLength, 4);
        Serial.print(F("BPM: "));
        Serial.println(beatsPerMinute);
        Serial.print(F("Note Duration: "));
        Serial.println(defaultNoteDuration);
        Serial.println(F("Finished handling header: K - Key"));
        
        break;
      
      case 'M':  // Meter
        Serial.println(F("Handling header: M - Meter"));
        // Remove the expected ':' character
        inputChar = str->read();
        skipCharacters(str, &inputChar, ": ");
        // Get the value for the meter from the fractional form (ex: 4/4 or 6/8)
        meterValue = (float)getIntegerFromStream(str, &inputChar);
        if (inputChar == '/') {
          inputChar = str->read(); // move past the '/'
          meterValue /= (float)getIntegerFromStream(str, &inputChar);
        }
        // Calculate the default note length based on the Meter
        defaultNoteLength = meterValue < 0.75f ? 0.0625f : 0.125f;
        // Calculate the default note duration based off of our (potentially new) note length
        defaultNoteDuration = delayTimeInMilliseconds(defaultNoteLength, beatsPerMinute);
        break;
      
      case 'Q':  // Tempo
        Serial.println(F("Handling header: Q - Tempo"));
        // Remove the expected ':' character
        inputChar = str->read();
        skipCharacters(str, &inputChar, ": ");
        // Get the integer value for the tempo
        defaultNoteDuration = getIntegerFromStream(str, &inputChar);
        // if a '/' is present, it means that they have specified
        // which note to assign the tempo to, and we must make calculations
        // based on this assumption
        if (inputChar == '/') {
          inputChar = str->read();
          double tempoNoteLength = (double)defaultNoteDuration / (double)getIntegerFromStream(str, &inputChar);
          // bypass the filler characters
          while (inputChar != '=') inputChar = str->read();
          inputChar = str->read();
          while (inputChar == ' ') inputChar = str->read();
          // the next int from the str will be our bpm for the given note length
          beatsPerMinute = getIntegerFromStream(str, &inputChar);
          // Calculate the default note duration based off of our (potentially new) bpm
          defaultNoteDuration = delayTimeInMilliseconds(defaultNoteLength, beatsPerMinute);
        } else {
          // If '/' was not present, then they just gave us our tempo
          beatsPerMinute = defaultNoteDuration;
          // Calculate the default note duration based off of our bpm
          defaultNoteDuration = delayTimeInMilliseconds(defaultNoteLength, beatsPerMinute);
        }
        Serial.println(F("Finished handling header: Q - Tempo"));
        break;
      
      case 'L':  // Default Note Length
        Serial.println(F("Handling header: L - Note Length"));
        // Remove the expected ':' character
        inputChar = str->read();
        skipCharacters(str, &inputChar, ": ");
        // Get the value for the length from the fractional form (ex: 4/4)
        defaultNoteLength = (float)getIntegerFromStream(str, &inputChar);
        if (inputChar == '/') {
          inputChar = str->read();
          defaultNoteLength /= (float)getIntegerFromStream(str, &inputChar);
        }
        // Calculate the default note duration based off of our (potentially new) note length
        defaultNoteDuration = delayTimeInMilliseconds(defaultNoteLength, beatsPerMinute);
        break;
        
      case '[':
        inBrackets = true;
        inputChar = str->read();
        break;
        
      case '|':
        // Some combinations of brackets [] actually have aesthetic meanings in ABC Notation
        // Once such combo is [| which we don't want to be seen as chord brackets []
        inBrackets = false;
        inputChar = str->read();
        break;
        
      case ']':
        inBrackets = false;
        inputChar = str->read();
        break;
      
      default:
        // If not a header, treat it as a note
        *freq = getFrequency(str, &inputChar);
        // If we didn't successfully get a note, 
        // reset our char and exit this loop's iteration
        if (*freq == -1) {
          inputChar = ' ';
          continue;
        }
        // Otherwise continue to get the duration next
        *dur = getDuration(str, &inputChar);
        
        // If we are in a set of brackets, that means the music wants to
        // play multiple notes at once, which the arduino piezo does not support
        // So we must escape all remaining notes until the end brack
        if (inBrackets) skipCharactersUntil(str, &inputChar, "]");
        
        // Make sure to return once we have our next valid note
        return;
    }
  }
}

int ABCNoteParser::getIntegerFromStream(Stream* str, char* previewedChar) {
  int num = 0;
  while ('0' <= *previewedChar && *previewedChar <= '9') {
    num = (num*10) + atoi(previewedChar);
    *previewedChar = str->read();
  }
  return num;
}

double ABCNoteParser::delayTimeInMilliseconds(double noteLength, float bpm) {
  return 240000 * noteLength / bpm;
}

int ABCNoteParser::getDuration(Stream* stream, char* input) {
  // Start off with the default duration
  int duration = defaultNoteDuration;
  
  // First see if we can find a multiplier (no preceding chars, just digits)
  int modifier = getIntegerFromStream(stream, input);
  
  if (modifier > 0) {
    // If there was a digit, then it means we multiply our current duration by that number
    duration *= modifier;
  }
  
  while (*input == '/') {
    // If a / is found, it means we divide our duration by the next number
    // If no number is provided, the default is /2
    modifier = getIntegerFromStream(stream, input);
    if (modifier > 0) duration /= modifier;
    else duration /= 2;
    *input = stream->read();
  }
  
  while (*input == '>') {
    // A > (aka a hornpipe) represents a 'dotted' note
    // (therefore adds half its current duration to itself)
    duration += duration/2;
    *input = stream->read();
  }
  
  return duration;
}

int ABCNoteParser::getFrequency(Stream* stream, char* input) {
  // setup some note-specific defaults first
  boolean sharpIndicator = false;
  boolean flatIndicator = false;
  int noteFreqIndex = -1;
  
  // Get the sharp/flat modifier (will recognize but pass over doubles)
  while (*input == '^') {
    sharpIndicator = true;
    *input = stream->read();
  }
  while (*input == '_') {
    flatIndicator = true;
    *input = stream->read();
  }
  
  // Get the note (CDEFGAB)
  if (*input == 'z') {
    // if z, map to 28 (rest)
    noteFreqIndex = 28;
  } else if ('C' <= *input && *input <= 'G') {
    // If CDEFG, map to 7-11
    noteFreqIndex = *input - 'C' + 7;
  } else if ('A' <= *input && *input <= 'B') {
    // If AB, map to 12-13
    noteFreqIndex = *input - 'A' + 12;
  } else if ('c' <= *input && *input <= 'g') {
    // If cdefg, map to 14-18
    noteFreqIndex = *input - 'c' + 14;
  } else if ('a' <= *input && *input <= 'b') {
    // If ab, map to 19-20
    noteFreqIndex = *input - 'a' + 19;
  }
    
  // If, for whatever reason, we still don't have a note, exit with a failure
  if (noteFreqIndex == -1) return -1;
  
  *input = stream->read();
  
  // Get the octave modifier (optional)
  if (*input == '\'') {
    // Apostrophe ' puts the note up an octave
    noteFreqIndex += 7;
    *input = stream->read();
  }    
  if (*input == ',') {
    // Comma , puts the note down an octave
    noteFreqIndex -= 7;
    *input = stream->read();
  }
  
  // Take our note frequency info and get the actual frequency
  if (flatIndicator) {
    // Flats can also become the below notes sharp freq
    sharpIndicator = true;
    noteFreqIndex = max(noteFreqIndex-1,0);
  }
  return frequencies[noteFreqIndex][sharpIndicator ? 1 : 0];
}

void ABCNoteParser::skipCharacters(Stream* stream, char* input, char* skipChars) {
  while (strchr(skipChars, *input)) {
    *input = stream->read();
  }
}

void ABCNoteParser::skipCharactersUntil(Stream* stream, char* input, char* skipUntil) {
  while (!strchr(skipUntil, *input)) {
    *input = stream->read();
  }
}
