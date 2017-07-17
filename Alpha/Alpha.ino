/*
LuxBeat Alpha Demo
Uses one Piezoelectric Sensor, and one LED strip.
*/

#include <Adafruit_NeoPixel.h>
#include <QueueArray.h>

#ifdef __AVR__
  #include <avr/power.h>
#endif

#define DEBUG

//Pins
const int LED_PIN = 6;
const int PIEZO_PIN = A0;

//LED Setup
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, LED_PIN, NEO_GRB + NEO_KHZ800);

//Input
int piezoAmp = 0;
int piezoMax = 0;
int piezoBufferSize = 20;

//Outputs
double pulseValue = 0;
double pulseDecay = 2;
double pulseStrength = 0.5;
int    lightOffset = 100;
double lightValue = 0;

// Queue of ADC values
QueueArray <int> piezoBuffer;

void setup()
{
  //Initialize NeoPixel, turn all pixels off
  strip.begin();
  strip.show();

  //USB Serial
  Serial.begin(9600);

  piezoBuffer.setPrinter (Serial);

  //Enable Output LED
  pinMode(LED_BUILTIN, OUTPUT);

  for (int i = 0; i < piezoBufferSize; i++){
    readPiezo();
  }
}

void loop()
{
  readPiezo();
  calcPiezoMax();
  lightWave();
/*
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
  }*/

  render();
}

void readPiezo(){
  piezoAmp = analogRead(PIEZO_PIN);

  // Read Piezo ADC value in, and convert it to a voltage
  //piezoV = piezoADC / 1023.0 * 5.0;

  piezoBuffer.push(piezoAmp);
  if (piezoBuffer.count() > piezoBufferSize) {
    piezoBuffer.pop();
  }
}
void calcPiezoMax(){

  int max = 0;
  for (int i = 0; i < 5; i++) {
    int n = piezoBuffer.peekAt(i);
    if (n > max)
      max = n;
  }
  //Why do I have to do this.
  if (piezoAmp > max)
    max = piezoAmp;

  piezoMax = max;
}

void lightWave(){
  pulseValue += (piezoAmp * pulseStrength);
  pulseValue = pulseValue * ((100 - pulseDecay) * 0.01);
  pulseValue = min(pulseValue, 1024);

  lightValue = pulseValue + lightOffset;
}

void onHit(){

  //colorWipe(strip.Color(random(255), random(255), random(255)), 5);
}

void render(){
#ifdef DEBUG
 Serial.print(piezoAmp); // Print the voltage.
 Serial.print(",");
 Serial.print(piezoMax);
 Serial.print(",");
 Serial.print(lightValue);
 Serial.println();
#endif
  //strip.show();
}


int piezoAmpSmoothed() {
  int avg = 0;
  for (int i = 0; i < piezoBuffer.count(); i++) {
    avg += piezoBuffer.peekAt(i);
  }
  avg += piezoAmp;
  avg = avg / (piezoBuffer.count() + 1);
  return avg;
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
