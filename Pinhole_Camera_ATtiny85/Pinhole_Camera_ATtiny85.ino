//  Libraries
#include <Servo8Bit.h>

//  Objects
Servo8Bit shutter;

//  Pin constants
const int shutterPin = 0;
const int luxPin = A1;  // 0.6 ohm/lux
const int exposePin = 1;
const int ledPin = 3;

//  Control constants
const int shutterOpen = 140;
const int shutterClose = 85;
//  Control variables
int shutterPos = 0;
boolean expose;
boolean lastExpose;

//  Image parameters
const float focalLen = 50.8; //mm
const float pinHole  = 0.28; //mm
const float waveLen  = .00065; //mm (red)
//  Image variables
float filmIso = 25.0;
float exposureTime;

//  Initialization
void setup() 
{
  pinMode(exposePin, INPUT);
  pinMode(ledPin, OUTPUT);
  //shutter.attach(shutterPin); 
  //shutter.write(shutterClose);
} 

//  Main loop
void loop() 
{ 
  digitalWrite(ledPin, HIGH);
  //expose = digitalRead(exposePin);
  //if(lastExpose && !expose) exposeFilm();
  //lastExpose = expose;
  delay(500.0);
  digitalWrite(ledPin, LOW);
  delay(500.0);
} 

///////////////////////////////////////////////////////////

//  Exposure calculation
float calcExposure(float iso) {
  float fstop;
  float time;
  fstop = focalLen / pinHole;
  time = (fstop * fstop) / (filmIso * getLux());
  return time;
}

float getLux() {
  // 5v 
  // 0.6 ohm/lux
  // 1000 ohm
  float voltage = analogRead(luxPin) * (5.0 / 1023.0);
  float resistance = (471.0 * voltage) / (5.0 - voltage);
  float lux = resistance / 0.6;
  return lux;
}

//  Take picture
void exposeFilm() {
  exposureTime = calcExposure(filmIso);
  shutter.write(shutterOpen);
  delay(exposureTime * 1000.0);
  shutter.write(shutterClose);
}
