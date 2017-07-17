/*
LuxBeat Alpha Demo
Uses one Piezoelectric Sensor, and one LED strip.

*/

#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
  #include <avr/power.h>
#endif

#define DEBUG

const int LED_PIN = 6;
const int PIEZO_PIN = A0; // Piezo output

Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, LED_PIN, NEO_GRB + NEO_KHZ800);

const int maxTimeout = 10;
int count = 0;
int light = 0;

int piezoADC = 0;
float piezoV = 0;

void setup()
{
  //Initialize NeoPixel, turn all pixels off
  strip.begin();
  strip.show();

  //USB Serial
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  readPiezo();

  // If there was just a hit
  if (piezoV > 1 && light == 0) {
    onHit();
  }

  if (light == 1 && count < maxTimeout) {
    count ++;
  }
  else {
    light = 0;
    count = 0;
    digitalWrite(LED_BUILTIN, LOW);
  }

  render();
}

void readPiezo(){
  int piezoADC = analogRead(PIEZO_PIN);

  // Read Piezo ADC value in, and convert it to a voltage
  float piezoV = piezoADC / 1023.0 * 5.0;
}

void onHit(){
  digitalWrite(LED_BUILTIN, HIGH);
  light = 1;

  colorWipe(strip.Color(random(255), random(255), random(255)), 5);
}

void render(){
#ifdef DEBUG
 Serial.println(piezoV); // Print the voltage.
#endif
  strip.show();
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

//Set the color of the entire strip
void colorSet(uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
  }
  strip.show();
}
