#include "Note.h"

// HARDWARE CONSTANTS
// pin of speaker/pezo
int PIN_PEZO = 8;

// RADIO SETTINGS
boolean radioOn = true;
boolean loopSingleSong = false;

// Notes frequencies
// FYI: the sets are CDEFGAB with numbers 12345678, example: ...A4-B4-C5-D5-E5-F5-G5-A5-B5-C6-D6.....
int const NOTE_C1 =  33;
int const NOTE_CS1 = 35;
int const NOTE_D1 =  37;
int const NOTE_DS1 = 39;
int const NOTE_E1 =  41;
int const NOTE_F1 =  44;
int const NOTE_FS1 = 46;
int const NOTE_G1 =  49;
int const NOTE_GS1 = 52;
int const NOTE_A1 =  55;
int const NOTE_AS1 = 58;
int const NOTE_B1 =  62;
int const NOTE_C2 =  65;
int const NOTE_CS2 = 69;
int const NOTE_D2 =  73;
int const NOTE_DS2 = 78;
int const NOTE_E2 =  82;
int const NOTE_F2 =  87;
int const NOTE_FS2 = 93;
int const NOTE_G2 =  98;
int const NOTE_GS2 = 104;
int const NOTE_A2 =  110;
int const NOTE_AS2 = 117;
int const NOTE_B2 =  123;
int const NOTE_C3 =  131;
int const NOTE_CS3 = 139;
int const NOTE_D3 =  147;
int const NOTE_DS3 = 156;
int const NOTE_E3 =  165;
int const NOTE_F3 =  175;
int const NOTE_FS3 = 185;
int const NOTE_G3 =  196;
int const NOTE_GS3 = 208;
int const NOTE_A3 =  220;
int const NOTE_AS3 = 233;
int const NOTE_B3 =  247;
int const NOTE_C4 =  262;
int const NOTE_CS4 = 277;
int const NOTE_D4 =  294;
int const NOTE_DS4 = 311;
int const NOTE_E4 =  330;
int const NOTE_F4 =  349;
int const NOTE_FS4 = 370;
int const NOTE_G4 =  392;
int const NOTE_GS4 = 415;
int const NOTE_A4 =  440;
int const NOTE_AS4 = 466;
int const NOTE_B4 =  494;
int const NOTE_C5 =  523;
int const NOTE_CS5 = 554;
int const NOTE_D5 =  587;
int const NOTE_DS5 = 622;
int const NOTE_E5 =  659;
int const NOTE_F5 =  698;
int const NOTE_FS5 = 740;
int const NOTE_G5 =  784;
int const NOTE_GS5 = 831;
int const NOTE_A5 =  880;
int const NOTE_AS5 = 932;
int const NOTE_B5 =  988;
int const NOTE_C6 =  1047;
int const NOTE_CS6 = 1109;
int const NOTE_D6 =  1175;
int const NOTE_DS6 = 1245;
int const NOTE_E6 =  1319;
int const NOTE_F6 =  1397;
int const NOTE_FS6 = 1480;
int const NOTE_G6 =  1568;
int const NOTE_GS6 = 1661;
int const NOTE_A6 =  1760;
int const NOTE_AS6 = 1865;
int const NOTE_B6 =  1976;
int const NOTE_C7 =  2093;
int const NOTE_CS7 = 2217;
int const NOTE_D7 =  2349;
int const NOTE_DS7 = 2489;
int const NOTE_E7 =  2637;
int const NOTE_F7 =  2794;
int const NOTE_FS7 = 2960;
int const NOTE_G7 =  3136;
int const NOTE_GS7 = 3322;
int const NOTE_A7 =  3520;
int const NOTE_AS7 = 3729;
int const NOTE_B7 =  3951;
int const NOTE_C8 =  4186;
int const NOTE_CS8 = 4435;
int const NOTE_D8 =  4699;
int const NOTE_DS8 = 4978;

// Marks the end of a song
int const END =  0;
int const REST =  1;

// Max song size
int const max_song_size = 200;

// Keep track of time passed in order to pace song
long previousMillis = 0;
long interval = 0;

// ------------ //
// Stored Songs //
// ------------ //
// Each song is stored with two arrays, with elements of the arrays corresponding to eachother:
// 2. The notes of the song (NOTES)
// 3. The durations for each note in milliseconds (DURATIONS)
int const NOTES = 0;
int const DURATIONS = 1;
int const INTERVAL_BETWEEN_SONGS = 1000;
int current_song = 0;
int thisNote = 0;

// Playlist of songs to be played on the radio
// For organizational purposes, keep each 'bar' of notes on separate lines
int const NUMBER_OF_SONGS_IN_PLAYLIST = 2;
Note* playlist[][max_song_size] = {
  // Mario Theme 
  {
    new Note(NOTE_E5, 150), new Note(NOTE_E5, 150), new Note(REST, 150), new Note(NOTE_E5, 150),  
    new Note(REST, 150), new Note(NOTE_C5, 150), new Note(NOTE_E5, 150), new Note(REST, 150),  
    new Note(NOTE_G5, 150), new Note(REST, 450), 
    new Note(NOTE_G4, 150), new Note(REST, 450), 
    
    new Note(NOTE_C5, 150), new Note(REST, 300), new Note(NOTE_G4, 150), 
    new Note(REST, 300), new Note(NOTE_E4, 150), new Note(REST, 150), 
    new Note(REST, 150), new Note(NOTE_A4, 150), new Note(REST, 150), new Note(NOTE_B4, 150),
    new Note(REST, 150), new Note(NOTE_B4, 150), new Note(NOTE_A4, 150), new Note(REST, 150), 
    
    new Note(NOTE_G4, 200), new Note(NOTE_E5, 200), new Note(NOTE_G5, 200), 
    new Note(NOTE_A5, 150), new Note(REST, 150), new Note(NOTE_F5, 150), new Note(NOTE_G5, 150),  
    new Note(REST, 150), new Note(NOTE_E5, 150), new Note(REST, 150), new Note(NOTE_C5, 150),
    new Note(NOTE_D5, 150), new Note(NOTE_B4, 150), new Note(REST, 300), 
    
    new Note(NOTE_C5, 150), new Note(REST, 300), new Note(NOTE_G4, 150), 
    new Note(REST, 300), new Note(NOTE_E4, 150), new Note(REST, 150), 
    new Note(REST, 150), new Note(NOTE_A4, 150), new Note(REST, 150), new Note(NOTE_B4, 150),
    new Note(REST, 150), new Note(NOTE_B4, 150), new Note(NOTE_A4, 150), new Note(REST, 150), 
    
    new Note(NOTE_G4, 200), new Note(NOTE_E5, 200), new Note(NOTE_G5, 200), 
    new Note(NOTE_A5, 150), new Note(REST, 150), new Note(NOTE_F5, 150), new Note(NOTE_G5, 150),  
    new Note(REST, 150), new Note(NOTE_E5, 150), new Note(REST, 150), new Note(NOTE_C5, 150),
    new Note(NOTE_D5, 150), new Note(NOTE_B4, 150), new Note(REST, 300), 
    
    NULL
  },
  // Zelda - Turtle Rock
  {
    new Note(NOTE_B4, 250), new Note(NOTE_CS5, 250), new Note(NOTE_F5, 250), new Note(NOTE_G5, 250), new Note(NOTE_B4, 250), new Note(NOTE_C5, 250), new Note(NOTE_F5, 250), new Note(NOTE_G5, 250),
    new Note(NOTE_B4, 250), new Note(NOTE_CS5, 250), new Note(NOTE_F5, 250), new Note(NOTE_G5, 250), new Note(NOTE_B4, 250), new Note(NOTE_C5, 250), new Note(NOTE_F5, 250), new Note(NOTE_G5, 250),
    new Note(NOTE_A4, 250), new Note(NOTE_B4, 250), new Note(NOTE_DS5, 250), new Note(NOTE_F5, 250), new Note(NOTE_A4, 250), new Note(NOTE_B4, 250), new Note(NOTE_D5, 250), new Note(NOTE_F5, 250),
    new Note(NOTE_A4, 250), new Note(NOTE_B4, 250), new Note(NOTE_DS5, 250), new Note(NOTE_F5, 250), new Note(NOTE_A4, 250), new Note(NOTE_B4, 250), new Note(NOTE_D5, 250), new Note(NOTE_F5, 250),
    
    new Note(NOTE_B4, 250), new Note(NOTE_CS5, 250), new Note(NOTE_F5, 250), new Note(NOTE_G5, 250), new Note(NOTE_B4, 250), new Note(NOTE_C5, 250), new Note(NOTE_F5, 250), new Note(NOTE_G5, 250),
    new Note(NOTE_B4, 250), new Note(NOTE_CS5, 250), new Note(NOTE_F5, 250), new Note(NOTE_G5, 250), new Note(NOTE_B4, 250), new Note(NOTE_C5, 250), new Note(NOTE_F5, 250), new Note(NOTE_G5, 250),
    new Note(NOTE_C5, 250), new Note(NOTE_D5, 250), new Note(NOTE_FS5, 250), new Note(NOTE_GS5, 250), new Note(NOTE_C5, 250), new Note(NOTE_D5, 250), new Note(NOTE_F5, 250), new Note(NOTE_G5, 250),
    new Note(NOTE_C5, 250), new Note(NOTE_D5, 250), new Note(NOTE_FS5, 250), new Note(NOTE_GS5, 250), new Note(NOTE_C5, 250), new Note(NOTE_D5, 250), new Note(NOTE_F5, 250), new Note(NOTE_G5, 250),
    
    new Note(NOTE_CS5, 250), new Note(NOTE_DS5, 250), new Note(NOTE_FS5, 250), new Note(NOTE_A5, 250), new Note(NOTE_C5, 250), new Note(NOTE_D5, 250), new Note(NOTE_F5, 250), new Note(NOTE_A5, 250),
    new Note(NOTE_CS5, 250), new Note(NOTE_DS5, 250), new Note(NOTE_FS5, 250), new Note(NOTE_A5, 250), new Note(NOTE_C5, 250), new Note(NOTE_D5, 250), new Note(NOTE_F5, 250), new Note(NOTE_A5, 250),
    new Note(NOTE_CS5, 250), new Note(NOTE_DS5, 250), new Note(NOTE_FS5, 250), new Note(NOTE_A5, 250), new Note(NOTE_C5, 250), new Note(NOTE_D5, 250), new Note(NOTE_F5, 250), new Note(NOTE_A5, 250),
    new Note(NOTE_CS5, 250), new Note(NOTE_DS5, 250), new Note(NOTE_FS5, 250), new Note(NOTE_A5, 250), new Note(NOTE_C5, 250), new Note(NOTE_D5, 250), new Note(NOTE_F5, 250), new Note(NOTE_A5, 250),
    
    new Note(NOTE_B4, 250), new Note(NOTE_CS5, 250), new Note(NOTE_F5, 250), new Note(NOTE_G5, 250), new Note(NOTE_B4, 250), new Note(NOTE_C5, 250), new Note(NOTE_F5, 250), new Note(NOTE_G5, 250),
    new Note(NOTE_B4, 250), new Note(NOTE_CS5, 250), new Note(NOTE_F5, 250), new Note(NOTE_G5, 250), new Note(NOTE_B4, 250), new Note(NOTE_C5, 250), new Note(NOTE_F5, 250), new Note(NOTE_G5, 250),
    new Note(NOTE_A4, 250), new Note(NOTE_B4, 250), new Note(NOTE_DS5, 250), new Note(NOTE_F5, 250), new Note(NOTE_A4, 250), new Note(NOTE_B4, 250), new Note(NOTE_D5, 250), new Note(NOTE_F5, 250),
    new Note(NOTE_A4, 250), new Note(NOTE_B4, 250), new Note(NOTE_DS5, 250), new Note(NOTE_F5, 250), new Note(NOTE_A4, 250), new Note(NOTE_B4, 250), new Note(NOTE_D5, 250), new Note(NOTE_F5, 250),
    
    new Note(NOTE_B4, 250), new Note(NOTE_CS5, 250), new Note(NOTE_F5, 250), new Note(NOTE_G5, 250), new Note(NOTE_B4, 250), new Note(NOTE_C5, 250), new Note(NOTE_F5, 250), new Note(NOTE_G5, 250),
    new Note(NOTE_B4, 250), new Note(NOTE_CS5, 250), new Note(NOTE_F5, 250), new Note(NOTE_G5, 250), new Note(NOTE_B4, 250), new Note(NOTE_C5, 250), new Note(NOTE_F5, 250), new Note(NOTE_G5, 250),
    new Note(NOTE_C5, 250), new Note(NOTE_D5, 250), new Note(NOTE_FS5, 250), new Note(NOTE_GS5, 250), new Note(NOTE_C5, 250), new Note(NOTE_D5, 250), new Note(NOTE_F5, 250), new Note(NOTE_G5, 250),
    new Note(NOTE_C5, 250), new Note(NOTE_D5, 250), new Note(NOTE_FS5, 250), new Note(NOTE_GS5, 250), new Note(NOTE_C5, 250), new Note(NOTE_D5, 250), new Note(NOTE_F5, 250), new Note(NOTE_G5, 250),
    
    new Note(NOTE_CS5, 250), new Note(NOTE_DS5, 250), new Note(NOTE_FS5, 250), new Note(NOTE_A5, 250), new Note(NOTE_C5, 250), new Note(NOTE_D5, 250), new Note(NOTE_F5, 250), new Note(NOTE_A5, 250),
    new Note(NOTE_CS5, 250), new Note(NOTE_DS5, 250), new Note(NOTE_FS5, 250), new Note(NOTE_A5, 250), new Note(NOTE_C5, 250), new Note(NOTE_D5, 250), new Note(NOTE_F5, 250), new Note(NOTE_A5, 250),
    new Note(NOTE_CS5, 250), new Note(NOTE_DS5, 250), new Note(NOTE_FS5, 250), new Note(NOTE_A5, 250), new Note(NOTE_C5, 250), new Note(NOTE_D5, 250), new Note(NOTE_F5, 250), new Note(NOTE_A5, 250),
    new Note(NOTE_CS5, 250), new Note(NOTE_DS5, 250), new Note(NOTE_FS5, 250), new Note(NOTE_A5, 250), new Note(NOTE_C5, 250), new Note(NOTE_D5, 250), new Note(NOTE_F5, 250), new Note(NOTE_A5, 250),
  }
};

void setup() {
  Serial.begin(9600); // set up Serial library at 9600 bps
  previousMillis = millis();
  Serial.println("Starting");
}


void loop() {
  // only consider playing music if the radio is 'on'
  if (!radioOn) return;
  
  // Check if the song is complete
  if (!playlist[current_song][thisNote]) {
    if (!loopSingleSong) {
      // advance to the next song if we aren't looping
      current_song = (current_song+1) % NUMBER_OF_SONGS_IN_PLAYLIST;
    }
    
    // Make sure we reset the current note
    thisNote = 0;
    
    // Make sure to provide a silent period between songs
    interval = INTERVAL_BETWEEN_SONGS;
  }
  
  // play the song by iterating over the notes at given intervals:
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;   
  
    // If the current note isn't a 'rest' beat, 
    // then play the note for the alloted duration (milliseconds)
    if (playlist[current_song][thisNote]->getFrequency() != REST) {
      tone(PIN_PEZO, 
           playlist[current_song][thisNote]->getFrequency(),
           playlist[current_song][thisNote]->getDuration());
    }
    
    // Set how long to wait until next note 
    // (namely the length of the currently playing note)
    interval = playlist[current_song][thisNote]->getDuration();
    
    // go to next note
    thisNote++;
  }
}
