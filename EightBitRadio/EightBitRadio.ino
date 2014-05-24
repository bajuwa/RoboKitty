// HARDWARE CONSTANTS
// pin of speaker/pezo
int PIN_PEZO = 8;

// RADIO SETTINGS
boolean radioOn = true;
boolean loopSongs = false;

//these are the notes and corresponding frequencies
// FYI: the sets are CDEFGAB with numbers 12345678, example: ...A4-B4-C5-D5-E5-F5-G5-A5-B5-C6-D6.....
int NOTE_B0 =  31;
int NOTE_C1 =  33;
int NOTE_CS1 = 35;
int NOTE_D1 =  37;
int NOTE_DS1 = 39;
int NOTE_E1 =  41;
int NOTE_F1 =  44;
int NOTE_FS1 = 46;
int NOTE_G1 =  49;
int NOTE_GS1 = 52;
int NOTE_A1 =  55;
int NOTE_AS1 = 58;
int NOTE_B1 =  62;
int NOTE_C2 =  65;
int NOTE_CS2 = 69;
int NOTE_D2 =  73;
int NOTE_DS2 = 78;
int NOTE_E2 =  82;
int NOTE_F2 =  87;
int NOTE_FS2 = 93;
int NOTE_G2 =  98;
int NOTE_GS2 = 104;
int NOTE_A2 =  110;
int NOTE_AS2 = 117;
int NOTE_B2 =  123;
int NOTE_C3 =  131;
int NOTE_CS3 = 139;
int NOTE_D3 =  147;
int NOTE_DS3 = 156;
int NOTE_E3 =  165;
int NOTE_F3 =  175;
int NOTE_FS3 = 185;
int NOTE_G3 =  196;
int NOTE_GS3 = 208;
int NOTE_A3 =  220;
int NOTE_AS3 = 233;
int NOTE_B3 =  247;
int NOTE_C4 =  262;
int NOTE_CS4 = 277;
int NOTE_D4 =  294;
int NOTE_DS4 = 311;
int NOTE_E4 =  330;
int NOTE_F4 =  349;
int NOTE_FS4 = 370;
int NOTE_G4 =  392;
int NOTE_GS4 = 415;
int NOTE_A4 =  440;
int NOTE_AS4 = 466;
int NOTE_B4 =  494;
int NOTE_C5 =  523;
int NOTE_CS5 = 554;
int NOTE_D5 =  587;
int NOTE_DS5 = 622;
int NOTE_E5 =  659;
int NOTE_F5 =  698;
int NOTE_FS5 = 740;
int NOTE_G5 =  784;
int NOTE_GS5 = 831;
int NOTE_A5 =  880;
int NOTE_AS5 = 932;
int NOTE_B5 =  988;
int NOTE_C6 =  1047;
int NOTE_CS6 = 1109;
int NOTE_D6 =  1175;
int NOTE_DS6 = 1245;
int NOTE_E6 =  1319;
int NOTE_F6 =  1397;
int NOTE_FS6 = 1480;
int NOTE_G6 =  1568;
int NOTE_GS6 = 1661;
int NOTE_A6 =  1760;
int NOTE_AS6 = 1865;
int NOTE_B6 =  1976;
int NOTE_C7 =  2093;
int NOTE_CS7 = 2217;
int NOTE_D7 =  2349;
int NOTE_DS7 = 2489;
int NOTE_E7 =  2637;
int NOTE_F7 =  2794;
int NOTE_FS7 = 2960;
int NOTE_G7 =  3136;
int NOTE_GS7 = 3322;
int NOTE_A7 =  3520;
int NOTE_AS7 = 3729;
int NOTE_B7 =  3951;
int NOTE_C8 =  4186;
int NOTE_CS8 = 4435;
int NOTE_D8 =  4699;
int NOTE_DS8 = 4978;

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
int playlist[][2][max_song_size] = {
  // Mario Theme 
  {
    {
      NOTE_E5, NOTE_E5, REST, NOTE_E5, 
      REST, NOTE_C5, NOTE_E5, REST, 
      NOTE_G5, REST, 
      NOTE_G4, REST, 
      
      NOTE_C5, REST, NOTE_G4, 
      REST, NOTE_E4, REST, 
      REST, NOTE_A4, REST, NOTE_B4,
      REST, NOTE_B4, NOTE_A4, REST, 
      
      NOTE_G4, NOTE_E5, NOTE_G5,
      NOTE_A5, REST, NOTE_F5, NOTE_G5,
      REST, NOTE_E5, REST, NOTE_C5,
      NOTE_D5, NOTE_B4, REST,
      
      NOTE_C5, REST, NOTE_G4, 
      REST, NOTE_E4, REST, 
      REST, NOTE_A4, REST, NOTE_B4,
      REST, NOTE_B4, NOTE_A4, REST, 
      
      NOTE_G4, NOTE_E5, NOTE_G5,
      NOTE_A5, REST, NOTE_F5, NOTE_G5,
      REST, NOTE_E5, REST, NOTE_C5,
      NOTE_D5, NOTE_B4, REST,
      
      END
    },
    {
      150, 150, 150, 150, 
      150, 150, 150, 150, 
      150, 450, 
      150, 450, 
      
      150, 300, 150, 
      300, 150, 150, 
      150, 150, 150, 150, 
      150, 150, 150, 150, 
      
      200, 200, 200, 
      150, 150, 150, 150, 
      150, 150, 150, 150, 
      150, 150, 300, 
      
      150, 150, 150, 150, 
      300, 150, 150, 
      150, 150, 150, 150, 
      150, 150, 150, 150, 
      
      200, 200, 200, 
      150, 150, 150, 150, 
      150, 150, 150, 150, 
      150, 150, 300, 
      
      END
    }
  },
  // Zelda - Turtle Rock
  {
    {
      NOTE_B4, NOTE_CS5, NOTE_F5, NOTE_G5, NOTE_B4, NOTE_C5, NOTE_F5, NOTE_G5,
      NOTE_B4, NOTE_CS5, NOTE_F5, NOTE_G5, NOTE_B4, NOTE_C5, NOTE_F5, NOTE_G5,
      NOTE_A4, NOTE_B4, NOTE_DS5, NOTE_F5, NOTE_A4, NOTE_B4, NOTE_D5, NOTE_F5,
      NOTE_A4, NOTE_B4, NOTE_DS5, NOTE_F5, NOTE_A4, NOTE_B4, NOTE_D5, NOTE_F5,
      
      NOTE_B4, NOTE_CS5, NOTE_F5, NOTE_G5, NOTE_B4, NOTE_C5, NOTE_F5, NOTE_G5,
      NOTE_B4, NOTE_CS5, NOTE_F5, NOTE_G5, NOTE_B4, NOTE_C5, NOTE_F5, NOTE_G5,
      NOTE_C5, NOTE_D5, NOTE_FS5, NOTE_GS5, NOTE_C5, NOTE_D5, NOTE_F5, NOTE_G5,
      NOTE_C5, NOTE_D5, NOTE_FS5, NOTE_GS5, NOTE_C5, NOTE_D5, NOTE_F5, NOTE_G5,
      
      NOTE_CS5, NOTE_DS5, NOTE_FS5, NOTE_A5, NOTE_C5, NOTE_D5, NOTE_F5, NOTE_A5,
      NOTE_CS5, NOTE_DS5, NOTE_FS5, NOTE_A5, NOTE_C5, NOTE_D5, NOTE_F5, NOTE_A5,
      NOTE_CS5, NOTE_DS5, NOTE_FS5, NOTE_A5, NOTE_C5, NOTE_D5, NOTE_F5, NOTE_A5,
      NOTE_CS5, NOTE_DS5, NOTE_FS5, NOTE_A5, NOTE_C5, NOTE_D5, NOTE_F5, NOTE_A5,
      
      NOTE_B4, NOTE_CS5, NOTE_F5, NOTE_G5, NOTE_B4, NOTE_C5, NOTE_F5, NOTE_G5,
      NOTE_B4, NOTE_CS5, NOTE_F5, NOTE_G5, NOTE_B4, NOTE_C5, NOTE_F5, NOTE_G5,
      NOTE_A4, NOTE_B4, NOTE_DS5, NOTE_F5, NOTE_A4, NOTE_B4, NOTE_D5, NOTE_F5,
      NOTE_A4, NOTE_B4, NOTE_DS5, NOTE_F5, NOTE_A4, NOTE_B4, NOTE_D5, NOTE_F5,
      
      NOTE_B4, NOTE_CS5, NOTE_F5, NOTE_G5, NOTE_B4, NOTE_C5, NOTE_F5, NOTE_G5,
      NOTE_B4, NOTE_CS5, NOTE_F5, NOTE_G5, NOTE_B4, NOTE_C5, NOTE_F5, NOTE_G5,
      NOTE_C5, NOTE_D5, NOTE_FS5, NOTE_GS5, NOTE_C5, NOTE_D5, NOTE_F5, NOTE_G5,
      NOTE_C5, NOTE_D5, NOTE_FS5, NOTE_GS5, NOTE_C5, NOTE_D5, NOTE_F5, NOTE_G5,
      
      NOTE_CS5, NOTE_DS5, NOTE_FS5, NOTE_A5, NOTE_C5, NOTE_D5, NOTE_F5, NOTE_A5,
      NOTE_CS5, NOTE_DS5, NOTE_FS5, NOTE_A5, NOTE_C5, NOTE_D5, NOTE_F5, NOTE_A5,
      NOTE_CS5, NOTE_DS5, NOTE_FS5, NOTE_A5, NOTE_C5, NOTE_D5, NOTE_F5, NOTE_A5,
      NOTE_CS5, NOTE_DS5, NOTE_FS5, NOTE_A5, NOTE_C5, NOTE_D5, NOTE_F5, NOTE_A5,
      
      END
    },
    {
      250, 250, 250, 250, 250, 250, 250, 250, 
      250, 250, 250, 250, 250, 250, 250, 250, 
      250, 250, 250, 250, 250, 250, 250, 250, 
      250, 250, 250, 250, 250, 250, 250, 250, 
      
      250, 250, 250, 250, 250, 250, 250, 250, 
      250, 250, 250, 250, 250, 250, 250, 250, 
      250, 250, 250, 250, 250, 250, 250, 250, 
      250, 250, 250, 250, 250, 250, 250, 250, 
      
      250, 250, 250, 250, 250, 250, 250, 250, 
      250, 250, 250, 250, 250, 250, 250, 250, 
      250, 250, 250, 250, 250, 250, 250, 250, 
      250, 250, 250, 250, 250, 250, 250, 250, 
      
      250, 250, 250, 250, 250, 250, 250, 250, 
      250, 250, 250, 250, 250, 250, 250, 250, 
      250, 250, 250, 250, 250, 250, 250, 250, 
      250, 250, 250, 250, 250, 250, 250, 250, 
      
      250, 250, 250, 250, 250, 250, 250, 250, 
      250, 250, 250, 250, 250, 250, 250, 250, 
      250, 250, 250, 250, 250, 250, 250, 250, 
      250, 250, 250, 250, 250, 250, 250, 250, 
      
      250, 250, 250, 250, 250, 250, 250, 250, 
      250, 250, 250, 250, 250, 250, 250, 250, 
      250, 250, 250, 250, 250, 250, 250, 250, 
      250, 250, 250, 250, 250, 250, 250, 250, 
      
      END
    }
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
  if (playlist[current_song][NOTES][thisNote] == END) {
    //temporary for my sanity
    //return;
    
    if (!loopSongs) {
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
    if (playlist[current_song][NOTES][thisNote] != REST) {
      tone(PIN_PEZO, 
           playlist[current_song][NOTES][thisNote],
           playlist[current_song][DURATIONS][thisNote]);
    }
    
    // Set how long to wait until next note 
    // (namely the length of the currently playing note)
    interval = playlist[current_song][DURATIONS][thisNote];
    
    // go to next note
    thisNote++;
  }
}
