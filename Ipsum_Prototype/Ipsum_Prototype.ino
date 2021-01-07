#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_NeoPixel.h>

//PWM values corresponding to 0 and 180 degrees for specific servos we used
#define SERVOMIN  80 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  530 // This is the 'maximum' pulse length count (out of 4096
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

//Pins for LED strips
#define pinL 5
#define pinT 6
#define pinR 9
#define pinB 10
int pixelCount = 24;

//Declare each LED ring of each tile seperately
Adafruit_NeoPixel left = Adafruit_NeoPixel(pixelCount, pinL, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel top = Adafruit_NeoPixel(pixelCount, pinT, NEO_RGB + NEO_KHZ800);
Adafruit_NeoPixel right = Adafruit_NeoPixel(pixelCount, pinR, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel bottom = Adafruit_NeoPixel(pixelCount, pinB, NEO_GRB + NEO_KHZ800);

//To check if scenario is active, and to set time intervals for movements and light animations
bool hazardActive = false;
unsigned long hazardTrackTime;
unsigned long hazardLightTime;
unsigned long hazardMoveTime;

//To check if scenario is active, and to set time intervals for movements and light animations
bool energyActive = false;
unsigned long energyLightTime;
float energyLightSinX;
unsigned long energyMoveTime;
float energyMoveSinX;

//To check if scenario is active, and to set time intervals for movements and light animations
bool windoorActive = false;
unsigned long windoorLightTime;
float windoorLightSinX;
unsigned long windoorMoveTime;
float windoorMoveSinX;

//To check if scenario is active, and to set time intervals for movements and light animations
bool welcomeActive = false;
unsigned long welcomeLightTime;
float welcomeLightSinX;
int welcomeLightRange = 50; //Amplitude of SIN wave that determines brightness of tiles

//Stores serial value from PC controller that controls which animation is played.
char mode;

//As the servos were cheap, the PWM values were not identical. This array stores the offset for each servo
int adjustments[8] = {15, 10, 0, 8, 0, 8, 0, -22};


void setup() {
  Serial.begin(9600);

  pwm.begin();
  pwm.setPWMFreq(50); // 50Hz updates

  left.begin();
  top.begin();
  right.begin();
  bottom.begin();
  left.show();
  top.show();
  right.show();
  bottom.show();

  yield(); // Wait for initialization to finish

  //Set all servo positions to 0 (in case physically turned)
  for (int i = 0; i < 7; i++) {
    pwm.setPWM(i, 0, angleToPulse(0, i));
  }

  delay(1000);
}


void loop() {
  if (Serial.available()) { 
    mode = Serial.read();

    //Activate scenario based on command from PC controller
    if (mode == '1') {
      hazardActive = true;
    } else if (mode == '2') {
      windoorActive = true;
    } else if (mode == '3') {
      energyActive = true;
    } else if (mode == '4') {
      welcomeActive = true;
    } else {
      // Nothing
    }
  }

  //Checks which scenario is active, and falles corresponding function
  if (hazardActive == true) {
    hazard();
  } else if (windoorActive == true) {
    windoor();
  } else if (energyActive == true) {
    energy();
  } else if (welcomeActive == true) {
    welcome();
  } else {
    // Nothing
  }
}

//Converts angle value to PWM. Let's keep things easy.
int angleToPulse(int angle, int index) {
  angle += adjustments[index]; //Adjusts offset for specific servo
  int pulse = map(angle, -90, 90, SERVOMIN, SERVOMAX);
  return pulse;
}

//Called after each scenario. Resets all values to make the animations start from begin positions again.
void resetValues() {
  hazardTrackTime = 0;
  hazardLightTime = 0;
  hazardMoveTime = 0;
  energyLightSinX = 0;
  energyLightTime = 0;
  energyMoveSinX = 0;
  energyMoveTime = 0;
  windoorLightSinX = 0;
  windoorLightTime = 0;
  windoorMoveSinX = 0;
  windoorMoveTime = 0;
  welcomeLightTime = 0;
  welcomeLightSinX = 0;

  //Move all tiles back to middle position.
  for (int i = 0; i < 7; i++) {
    pwm.setPWM(i, 0, angleToPulse(0, i));
  }

  //Turn of all LED rings
  for (int i = 0; i < 24; i++) {
    uint32_t fillColor = left.ColorHSV(5200, 0, 0);
    left.setPixelColor(i, fillColor);
    top.setPixelColor(i, fillColor);
    right.setPixelColor(i, fillColor);
    bottom.setPixelColor(i, fillColor);
  }

  left.show();
  top.show();
  right.show();
  bottom.show();
}
