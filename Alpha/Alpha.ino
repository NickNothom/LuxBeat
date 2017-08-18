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
int piezoBufferSize = 100;

//Settings
int r_hex = 255;
int g_hex = 0;
int b_hex = 255;

//Outputs
double pulseDecay = 0.1;          // How fast the signal falls
double pulseStrength = 0.01;     // How much influence the accumulator has
int    lightOffset = 0;       // Value that the signal will rest at
double pulseValue = 0;          // Value of the pulse
double lightValue = 0;          // Total output (pulse + offset)

// Queue of ADC values
QueueArray <int> piezoBuffer;

int cycle = 0;

void setup()
{
  //Initialize NeoPixel, turn all pixels off
  strip.begin();
  strip.show();

  //USB Serial
  Serial.begin(115200);

  //Enable Output LED
  pinMode(LED_BUILTIN, OUTPUT);

  //Input Setup
  piezoBuffer.setPrinter (Serial);
  for (int i = 0; i < piezoBufferSize; i++){
    readPiezo();
  }
}

void loop()
{
  readPiezo();
  calcPiezoMax();
  lightWave();

  if (cycle % 50 == 0){
    render();
  }
  cycle++;
}

void readPiezo(){
  piezoAmp = analogRead(PIEZO_PIN);

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
  colorWipe(strip.Color(random(255), random(255), random(255)), 5);
}

void render(){
#ifdef DEBUG
 Serial.print(piezoAmp); // Print the voltage.
 Serial.print(",");
 Serial.print(piezoAmpSmoothed());
 Serial.print(",");
 Serial.print(lightValue);
 Serial.println();
#endif

  int ledBrightness = lightValue / 16;
  double r_mult = r_hex / 255;
  double g_mult = g_hex / 255;
  double b_mult = b_hex / 255;

  colorSet(strip.Color((ledBrightness * r_mult), (ledBrightness * g_mult), (ledBrightness * b_mult)));
  strip.show();
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

// Set the color of the entire strip
void colorSet(uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
  }
  //strip.show();
}
