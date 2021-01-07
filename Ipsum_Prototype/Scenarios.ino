//Called when hazard scenario is active
void hazard() {
  //Change all LEDs randomly every 70ms to create flickering effect of fire
  if (millis() - hazardLightTime > 70) {
    for (int i = 0; i < 24; i++) {
      uint32_t fillColor = left.ColorHSV(5200, 255, random(50, 110));
      left.setPixelColor(i, fillColor);
      top.setPixelColor(i, fillColor);
      right.setPixelColor(i, fillColor);
      bottom.setPixelColor(i, fillColor);
    }
    left.show();
    top.show();
    right.show();
    bottom.show();
    hazardLightTime = millis();

    hazardTrackTime++;
  }

  //Move every tile randomly in a range of 10 degrees to create disturbing and 'wrong' movement
  if (millis() - hazardMoveTime > 1500) {
    Serial.println(hazardMoveTime);
    pwm.setPWM(0, 0, angleToPulse(random(-5, 5), 0));
    pwm.setPWM(1, 0, angleToPulse(random(-5, 5), 1));
    pwm.setPWM(2, 0, angleToPulse(random(-5, 5), 2));
    pwm.setPWM(3, 0, angleToPulse(random(-5, 5), 3));
    pwm.setPWM(4, 0, angleToPulse(random(-5, 5), 4));
    pwm.setPWM(5, 0, angleToPulse(random(-5, 5), 5));
    pwm.setPWM(6, 0, angleToPulse(random(-5, 5), 6));
    pwm.setPWM(7, 0, angleToPulse(random(-5, 5), 7));
    hazardMoveTime = millis();
  }

  //Determines how long the animation lasts
  if (hazardTrackTime >= 210) { //Dependent on interval of lights (now 70ms)
    resetValues();
    hazardActive = false;
  }
}

//Called when energy scenario is active
void energy() {
  //Use SIN wave to controll brightness
  if (millis() - energyLightTime > 50) {
    int brightness = 110 * sin(0.5 * PI * energyLightSinX - 0.5 * PI);
    if (brightness < 0) {
      brightness = 0;
    }

    //Use brightness above to controll lights
    for (int i = 0; i < 24; i++) {
      uint32_t fillColor = left.ColorHSV(32000, 255, brightness);
      left.setPixelColor(i, fillColor);
      top.setPixelColor(i, fillColor);
      right.setPixelColor(i, fillColor);
      bottom.setPixelColor(i, fillColor);
    }

    left.show();
    top.show();
    right.show();
    bottom.show();
    energyLightSinX += 0.05;
    energyLightTime = millis();
  }

  //Use SIN wave to create sweeping motion for tiles
  if (millis() - energyMoveTime > 50) {
    pwm.setPWM(0, 0, angleToPulse(15 * -sin(0.5 * PI * energyMoveSinX), 0));
    pwm.setPWM(3, 0, angleToPulse(15 * sin(0.5 * PI * energyMoveSinX), 3));
    pwm.setPWM(4, 0, angleToPulse(15 * sin(0.5 * PI * energyMoveSinX), 4));
    pwm.setPWM(7, 0, angleToPulse(15 * sin(0.5 * PI * energyMoveSinX), 7));
    energyMoveSinX += 0.05;
    energyMoveTime = millis();
  }

  //Determines how long the animation lasts
  if (energyLightSinX >= 16) { //Dependent on interval of lights (now 50ms)
    resetValues();
    energyActive = false;
  }
}

//Called when energy scenario is active
void windoor() {
  //Use SIN wave to controll brightness
  if (millis() - windoorLightTime > 50) {
    int brightness = 110 * sin(PI * windoorMoveSinX - 0.5 * PI);
    if (brightness < 0) {
      brightness = 0;
    }

    //Use brightness above to controll lights
    for (int i = 0; i < 24; i++) {
      uint32_t fillColor = left.ColorHSV(21000, 255, brightness);
      left.setPixelColor(i, fillColor);
      right.setPixelColor(i, fillColor);
      bottom.setPixelColor(i, fillColor);
    }

    left.show();
    right.show();
    bottom.show();
    windoorLightSinX += 0.05;
    windoorLightTime = millis();
  }

  //Use SIN wave to create sweeping motion for tiles (only 3, because designed animation does not use top tile)
  if (millis() - windoorMoveTime > 50) {
    pwm.setPWM(0, 0, angleToPulse(10 * -sin(PI * windoorMoveSinX - 0.5 * PI) + 5, 0));
    pwm.setPWM(4, 0, angleToPulse(10 * sin(PI * windoorMoveSinX - 0.5 * PI) + 5, 4));
    pwm.setPWM(7, 0, angleToPulse(10 * sin(PI * windoorMoveSinX - 0.5 * PI) + 5, 7));
    windoorMoveSinX += 0.05;
    windoorMoveTime = millis();
  }

  //Determines how long the animation lasts
  if (windoorLightSinX >= 16) { //Dependent on interval of lights (now 50ms)
    resetValues();
    windoorActive = false;
  }
}


void welcome() {
  if (millis() - welcomeLightTime > 50) {

    //Use asynchronous SIN waves to controll the brightness of each tile
    int brightness = welcomeLightRange * sin(0.5 * PI * welcomeLightSinX) + 60;
    for (int i = 0; i < 24; i++) {
      uint32_t fillColor = left.ColorHSV(8000, 255, brightness);
      left.setPixelColor(i, fillColor);
    }

    brightness = welcomeLightRange * cos(0.5 * PI * welcomeLightSinX - 0.2) + 60;
    for (int i = 0; i < 24; i++) {
      uint32_t fillColor = left.ColorHSV(8000, 255, brightness);
      top.setPixelColor(i, fillColor);
    }

    brightness = welcomeLightRange * sin(0.5 * PI * welcomeLightSinX - 1) + 60;
    for (int i = 0; i < 24; i++) {
      uint32_t fillColor = left.ColorHSV(8000, 255, brightness);
      right.setPixelColor(i, fillColor);
    }

    brightness = welcomeLightRange * cos(0.5 * PI * welcomeLightSinX - 0.5) + 60;
    for (int i = 0; i < 24; i++) {
      uint32_t fillColor = left.ColorHSV(8000, 255, brightness);
      bottom.setPixelColor(i, fillColor);
    }

    left.show();
    top.show();
    right.show();
    bottom.show();

    welcomeLightSinX += 0.05;
    welcomeLightTime = millis();
  }

  //Determines how long the animation lasts
  if (welcomeLightSinX >= 16) { //Dependent on interval of lights (now 50ms)
    resetValues();
    welcomeActive = false;
  }
}
