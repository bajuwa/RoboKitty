// Pins that correspond to the LEDs that will light up 
// when charge is greater than 0%, 10%, ... etc
int const PIN_0 = 0;
int const PIN_10 = 1;
int const PIN_20 = 2;
int const PIN_30 = 3;
int const PIN_40 = 4;
int const PIN_50 = 5;
int const PIN_60 = 6;
int const PIN_70 = 7;
int const PIN_80 = 8;
int const PIN_90 = 9;

// measure time for the 'blink' effect
int blinkInterval = 500;
long currentMillis = 0;
long previousMillis = 0;

void setup() {
  Serial.begin(9600);
  pinMode(PIN_0, OUTPUT);
  pinMode(PIN_10, OUTPUT);
  pinMode(PIN_20, OUTPUT);
  pinMode(PIN_30, OUTPUT);
  pinMode(PIN_40, OUTPUT);
  pinMode(PIN_50, OUTPUT);
  pinMode(PIN_60, OUTPUT);
  pinMode(PIN_70, OUTPUT);
  pinMode(PIN_80, OUTPUT);
  pinMode(PIN_90, OUTPUT);
  currentMillis = millis();
}

void loop() {
  currentMillis = millis();
  // Assuming max possible charge of 5500 millivolts, calculate the remaining percent
  int remainingPercent = readVcc() / 55;
  
  if (remainingPercent > 0) digitalWrite(PIN_0, HIGH);
  else digitalWrite(PIN_0, LOW);
  
  if (remainingPercent > 10) digitalWrite(PIN_10, HIGH);
  else digitalWrite(PIN_10, LOW);
  
  if (remainingPercent > 20) digitalWrite(PIN_20, HIGH);
  else digitalWrite(PIN_20, LOW);
  
  if (remainingPercent > 30) digitalWrite(PIN_30, HIGH);
  else digitalWrite(PIN_30, LOW);
  
  if (remainingPercent > 40) digitalWrite(PIN_40, HIGH);
  else digitalWrite(PIN_40, LOW);
  
  if (remainingPercent > 50) digitalWrite(PIN_50, HIGH);
  else digitalWrite(PIN_50, LOW);
  
  if (remainingPercent > 60) digitalWrite(PIN_60, HIGH);
  else digitalWrite(PIN_60, LOW);
  
  if (remainingPercent > 70) digitalWrite(PIN_70, HIGH);
  else digitalWrite(PIN_70, LOW);
  
  if (remainingPercent > 80) digitalWrite(PIN_80, HIGH);
  else digitalWrite(PIN_80, LOW);
  
  if (remainingPercent > 90) digitalWrite(PIN_90, HIGH);
  else digitalWrite(PIN_90, LOW);
  
  // set the last active led to 'blink'
  if (currentMillis - previousMillis > blinkInterval) {
    digitalWrite(min(remainingPercent/10, 9), LOW);
    if (currentMillis - previousMillis > blinkInterval*2) {
      previousMillis = currentMillis;
    }
  }
}

// Code curtesy of:
// http://provideyourown.com/2012/secret-arduino-voltmeter-measure-battery-voltage/
long readVcc() {
  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
    ADMUX = _BV(MUX5) | _BV(MUX0);
  #elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    ADMUX = _BV(MUX3) | _BV(MUX2);
  #else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #endif  
 
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA,ADSC)); // measuring
 
  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH  
  uint8_t high = ADCH; // unlocks both
 
  long result = (high<<8) | low;
 
  result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  return result; // Vcc in millivolts
}
