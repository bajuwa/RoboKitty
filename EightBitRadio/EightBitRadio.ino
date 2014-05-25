#include <StandardCplusplus.h>
#include <vector>
#include <system_configuration.h>
#include <unwind-cxx.h>
#include <utility.h>

#include "Note.h"

using namespace std;

// HARDWARE CONSTANTS
// pin of speaker/pezo
int PIN_PEZO = 8;

// RADIO SETTINGS
boolean radioOn = true;
boolean loopSingleSong = false;

// Keep track of time passed in order to pace song
long previousMillis = 0;
long interval = 0;

// Stored Song info
int const INTERVAL_BETWEEN_SONGS = 1000;
int thisNote = 0;

// Playlist of songs to be played on the radio
// For organizational purposes, keep each 'bar' of notes on separate lines
vector<Note*> loadedSong;
//Note* playlist[][max_song_size] = {{}};

void setup() {
  Serial.begin(9600); // set up Serial library at 9600 bps
  previousMillis = millis();
  
  // Take one text file and convert it to a usable song
  // (only keep one song in memory at a time)
  Serial.println("Parsing text file for song");
  //char pseudoFileText[] = "659 150-659 150-0 150-659 150-0 150-523 150-659 150-0 150-784 150-0 450-392 150-0 450-523 150-0 300-392 150-0 300-330 150-0 150-0 150-440 150-0 150-494 150-0 150-494 150-440 150-0 150-392 200-659 200-784 200-880 150-0 150-698 150-784 150-0 150-659 150-0 150-523 150-587 150-494 150-0 300-523 150-0 300-392 150-0 300-330 150-0 150-0 150-440 150-0 150-494 150-0 150-494 150-440 150-0 150-392 200-659 200-784 200-880 150-0 150-698 150-784 150-0 150-659 150-0 150-523 150-587 150-494 150-0 300";
  loadSong("659 150-659 150-0 150-659 150-0 150-523 150-659 150-0 150-784 150-0 450-392 150");
}

void loadSong(char song[]) {
  loadedSong.clear();
  char separators[] = " -";
  char* noteFreq = strtok(song, separators);
  char* noteDur = strtok(NULL, separators);
  unsigned int i = 0;
  while (noteFreq != NULL && noteDur != NULL) {
    Serial.println(i);
    loadedSong.push_back( new Note(atoi(noteFreq), atoi(noteDur)) );
    noteFreq = strtok(NULL, separators);
    noteDur = strtok(NULL, separators);
  }
  loadedSong.push_back( NULL ); 
}


void loop() {
  // only consider playing music if the radio is 'on'
  if (!radioOn) return;
  
  // Check if the song is complete
  if (!loadedSong[thisNote]) {
    if (!loopSingleSong) {
      // advance to the next song if we aren't looping
      // TODO once we have sd card to load from
      
      //temporary for my sanity...
      return;
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
    tone(PIN_PEZO, 
         loadedSong[thisNote]->getFrequency(),
         loadedSong[thisNote]->getDuration());
    
    // Set how long to wait until next note 
    // (namely the length of the currently playing note)
    interval = loadedSong[thisNote]->getDuration();
    
    // go to next note
    thisNote++;
  }
}



