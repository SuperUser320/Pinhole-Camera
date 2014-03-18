//  Libraries
#include <Servo.h>

//  Objects
Servo shutter;

//  Pin constants
const int shutterPin = 9;
const int isoPin = 10;
const int luxPin = A0;  // 0.6 ohm/lux
const int exposePin = 12;

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
const float meterConst = 8.0;
//  Image variables
float filmIso = 25.0;
float exposureTime;

//  Initialization
void setup() 
{ 
  Serial.begin(57600);
  pinMode(shutterPin, OUTPUT);
  pinMode(isoPin, INPUT);
  pinMode(exposePin, INPUT);
  shutter.attach(shutterPin); 
  shutter.write(shutterClose);
} 

//  Main loop
void loop() 
{ 
  expose = digitalRead(exposePin);
  if(lastExpose && !expose) exposeFilm();
  lastExpose = expose;
  delay(50.0);
} 

///////////////////////////////////////////////////////////

//  Exposure calculation
float calcExposure(float iso) {
  float fstop;
  float time;
  
  fstop = focalLen / pinHole;
  
  time = ((fstop * fstop) * meterConst)/ (filmIso * getLux());
  Serial.print("Exp. time: ");
  Serial.println(time);
  
  return time;
}

float getLux() {
  // 5v 
  // 0.6 ohm/lux
  // 1000 ohm
  
  float voltage = analogRead(luxPin) * (5.0 / 1023.0);
  Serial.print("voltage: ");
  Serial.println(voltage);
  
  float resistance = (471.0 * voltage) / (5.0 - voltage);
  Serial.print("resistance: ");
  Serial.println(resistance);
  
  float lux = resistance / 0.6;
  Serial.print("lux: ");
  Serial.println(lux);
  
  return lux;
}

//  Take picture
void exposeFilm() {
  exposureTime = calcExposure(filmIso);
  shutter.write(shutterOpen);
  Serial.println("Exposing...");
  delay(exposureTime * 1000.0);
  shutter.write(shutterClose);
  Serial.println("Closing exposure...");
}
