/******************************************************************************
Piezo_Vibration_Sensor.ino
Example sketch for SparkFun's Piezo Vibration Sensor
  (https://www.sparkfun.com/products/9197)
Jim Lindblom @ SparkFun Electronics
April 29, 2016

- Connect a 1Mohm resistor across the Piezo sensor's pins.
- Connect one leg of the Piezo to GND
- Connect the other leg of the piezo to A0

Vibrations on the Piezo sensor create voltags, which are sensed by the Arduino's
A0 pin. Check the serial monitor to view the voltage generated.

Development environment specifics:
Arduino 1.6.7
******************************************************************************/
const int PIEZO_PIN = A0; // Piezo output

const int maxTimeout = 10;
int count = 0;
int light = 0;

void setup()
{
  //USB Serial
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  int piezoADC = analogRead(PIEZO_PIN);

  // Read Piezo ADC value in, and convert it to a voltage
  float piezoV = piezoADC / 1023.0 * 5.0;

  if (piezoV > 1 && light == 0) {
      digitalWrite(LED_BUILTIN, HIGH);
      light = 1;
  }

  if (light == 1 && count < maxTimeout) {
    count ++;
  }
  else {
    light = 0;
    count = 0;
    digitalWrite(LED_BUILTIN, LOW);
  }

  Serial.println(piezoV); // Print the voltage.

  /*Serial.print(",");
  Serial.println(average);*/
}
