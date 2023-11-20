void celebrate() {
  // Code for playing celebratory tune
  // Pokemon successfully capture sound
  buzzer.tone(880, 400);
  buzzer.tone(698, 400);
  buzzer.tone(523, 800);
  buzzer.tone(932, 200);
  buzzer.noTone();
  buzzer.tone(932, 200);
  buzzer.tone(932, 200);
  buzzer.tone(784, 200);
  buzzer.tone(784, 200);
  buzzer.tone(932, 200);
  buzzer.tone(880, 800);
  buzzer.noTone();
}

void stopMotor() {
  leftMotor.stop();
  rightMotor.stop();
}
void moveForward() {
  leftMotor.run(-MOTORSPEED_LEFT);
  rightMotor.run(MOTORSPEED_RIGHT);
}
void turnRight() {
  leftMotor.run(-MOTORSPEED_LEFT);
  rightMotor.run(-MOTORSPEED_RIGHT);
  delay(TURNING_TIME_MS);
  stopMotor();
}
void turnLeft() {
  leftMotor.run(MOTORSPEED_LEFT);
  rightMotor.run(MOTORSPEED_RIGHT);
  delay(TURNING_TIME_MS);
  stopMotor();
}
void uTurn() {
  leftMotor.run(MOTORSPEED_LEFT);
  rightMotor.run(MOTORSPEED_RIGHT);
  delay(2 * TURNING_TIME_MS);
  stopMotor();
}
void doubleLeftTurn() {
  leftMotor.run(MOTORSPEED_LEFT);
  rightMotor.run(MOTORSPEED_RIGHT);
  delay(TURNING_TIME_MS);
  stopMotor();
  moveForward();
  delay(ONE_SQUARE_TIME_MS);
  stopMotor();
  leftMotor.run(MOTORSPEED_LEFT);
  rightMotor.run(MOTORSPEED_RIGHT);
  delay(TURNING_TIME_MS);
  stopMotor();
}
void doubleRightTurn() {
  turnRight();
  moveForward();
  delay(ONE_SQUARE_TIME_MS);
  stopMotor();
  turnRight();
  stopMotor();
}
void nudgeLeft() {
  // Code for nudging slightly to the left for some short interval
  leftMotor.run(-MOTORSPEED_LEFT);
  rightMotor.run(MOTORSPEED_RIGHT + NUDGE_INTENSITY);
  delay(NUDGE_TIME_MS);
}
void nudgeRight() {
  // Code for nudging slightly to the right for some short interval
  leftMotor.run(-MOTORSPEED_LEFT - NUDGE_INTENSITY);
  rightMotor.run(MOTORSPEED_RIGHT);
  delay(NUDGE_TIME_MS);
}

float read_IR() {
  // get distance from wall using IR sensor
  // turn off IR
  digitalWrite(PIN_A0, LOW);
  digitalWrite(PIN_A1, HIGH);
  delay(2);
  float ambient_IR = analogRead(IR_PIN);
  // turn on IR
  digitalWrite(PIN_A0, LOW);
  digitalWrite(PIN_A1, LOW);
  delay(2);
  float total_IR = analogRead(IR_PIN);

  return ambient_IR - total_IR;
}

float get_distance() {
  // get distance from wall using ultrasonic sensor
  pinMode(ULTRASONIC, OUTPUT);
  // turns off and on ultrasonic sensor
  digitalWrite(ULTRASONIC, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRASONIC, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC, LOW);
  //calculate duration
  pinMode(ULTRASONIC, INPUT);
  float duration = pulseIn(ULTRASONIC, HIGH, TIMEOUT);

  return (duration / 2.0 / 1000000 * SPEED_OF_SOUND * 100);
}

// code for turning on LEDs
void shineRed() {
  // Code for turning on the red LED only
  digitalWrite(PIN_A0, HIGH);
  digitalWrite(PIN_A1, HIGH);
}
void shineGreen() {
  // Code for turning on the green LED only
  digitalWrite(PIN_A0, HIGH);
  digitalWrite(PIN_A1, LOW);
}
void shineBlue() {
  // Code for turning on the blue LED only
  digitalWrite(PIN_A0, LOW);
  digitalWrite(PIN_A1, HIGH);
}
void shineNone() {
  // code for turning off all LEDs
  digitalWrite(PIN_A0, LOW);
  digitalWrite(PIN_A1, LOW);
}

void Colour() {
  // when black line is detected
  detectColour_1(); // voids inaccurate first reading
  detectColour_2();
  int red = (int)(colourArray[0]);
  int green = (int)(colourArray[1]);
  int blue = (int)(colourArray[2]);
  checkColour(red, green, blue); // checks colour + action
}

int getAvgReading(int times) {
  // find the average reading for the requested number of times of scanning LDR
  int reading;
  int total = 0;
  // take the reading as many times as requested and add them up
  for (int i = 0; i < times; i += 1) {
    reading = analogRead(LDR_PIN);
    total += reading;
    delay(LDR_WAIT);
  }
  // calculate the average and return it
  return total / times;
}

void shine_LED(int i, bool onState) {
  // code to control LEDs (turn on and off)
  if (onState == false) {
    shineNone();
  } else {
    if (i == 0) {
      shineRed();
    } else if (i == 1) {
      shineGreen();
    } else if (i == 2) {
      shineBlue();
    }
  }
}

void detectColour_1() {
  // faster version of code for detection of colour (to be voided)
  for (int c = 0; c <= 2; c += 1) {
    //Serial.print(colourStr[c]);
    shine_LED(c, true);
    delay(50);
    // get the average of 5 consecutive readings for the current colour and return an average: THE NO. OF AVG VALS CAN BE CHANGED!
    colourArray[c] = getAvgReading(5);
    // the average reading returned minus the lowest value divided by the maximum possible range, multiplied by 255 will give a value between 0-255,
    // representing the value for the current reflectivity (i.e. the colour LDR is exposed to)
    colourArray[c] = (colourArray[c] - blackArray[c]) / (greyDiff[c]) * 255;
    shine_LED(c, false);
    delay(50);
    //Serial.println(int(colourArray[c])); //show the value for the current colour LED, which corresponds to either the R, G or B of the RGB code
  }
  /*for (int c = 0; c <= 2; c += 1) {
    Serial.print(int(colourArray[c]));
    Serial.print(" ");
  }
  Serial.println("");*/
}

void detectColour_2() {
  // code for detection of colour
  for (int c = 0; c <= 2; c += 1) {
    //Serial.print(colourStr[c]);
    shine_LED(c, true);
    delay(200);
    // get the average of 5 consecutive readings for the current colour and return an average: THE NO. OF AVG VALS CAN BE CHANGED!
    colourArray[c] = getAvgReading(5);
    // the average reading returned minus the lowest value divided by the maximum possible range, multiplied by 255 will give a value between 0-255,
    //representing the value for the current reflectivity (i.e. the colour LDR is exposed to)
    colourArray[c] = (colourArray[c] - blackArray[c]) / (greyDiff[c]) * 255;
    shine_LED(c, false);
    delay(200);
    //Serial.println(int(colourArray[c])); //show the value for the current colour LED, which corresponds to either the R, G or B of the RGB code
  }
  /*for (int c = 0; c <= 2; c += 1) {
    Serial.print(int(colourArray[c]));
    Serial.print(" ");
  }
  Serial.println("");*/
}

void checkColour(int red, int green, int blue) {
  // code for checking colour detected + action
  if (red > 70 && red < 120
      && green > 100 && green < 150
      && blue > 90 && blue < 130) {
    // green colour
    led.setColor(0, 255, 0);  // set mbot LED to Green
    led.show();
    //Serial.println("Green");
    turnRight();
  } else if (red > 230 && red < 270
             && green > 100 && green < 150
             && blue > 70 && blue < 120) {
    // orange colour
    led.setColor(255, 50, 0);  // set mbot LED to orange
    led.show();
    //Serial.println("Orange");
    uTurn();
  } else if (red > 220 && red < 260
             && green > 60 && green < 99
             && blue > 50 && blue < 100) {
    // red colour
    led.setColor(255, 0, 0);  // set mbot LED to Red
    led.show();
    //Serial.println("Red");
    turnLeft();
  } else if (red > 110 && red < 170
             && green > 160 && green < 220
             && blue > 210 && blue < 260) {
    // blue colour
    led.setColor(0, 0, 255);  // set mbot LED to Blue
    led.show();
    //Serial.println("Blue");
    doubleRightTurn();
  } else if (red > 150 && red < 200
             && green > 100 && green < 160
             && blue > 170 && blue < 220) {
    // purple colour
    led.setColor(128, 0, 128);  // set mbot LED to purple
    led.show();
    //Serial.println("Purple");
    doubleLeftTurn();
  } else if (red > 200 && green > 200 &&
              blue > 200) {
    // white colour
    led.setColor(255, 255, 255);  // set mbot LED to white colour
    led.show();       
    stopMotor();
    celebrate();
    status = 0;
  } else {  // show failed colour detection by shining white light
    led.setColor(255, 255, 255);
    led.show();
  }
}

void setBalance() {
  // code for colour callibration
  // set white balance
  Serial.println("Put White Sample For Calibration ...");
  delay(5000); // delay for five seconds for getting sample ready
  // scan the white sample.
  // go through one colour at a time, set the maximum reading for each colour -- red, green and blue to the white array
  for( int i = 0; i <= 2; i += 1) {
    shine_LED(i, true);
    delay(RGB_WAIT);
    whiteArray[i] = getAvgReading(5); //scan 5 times and return the average,
    shine_LED(i, false);
    delay(RGB_WAIT);
  }
  // done scanning white, time for the black sample.
  // set black balance
  Serial.println("Put Black Sample For Calibration ...");
  delay(5000); // delay for five seconds for getting sample ready
  // go through one colour at a time, set the minimum reading for red, green and blue to the black array
  for (int i = 0; i <= 2; i+= 1) {
    shine_LED(i, true);
    delay(RGB_WAIT);
    blackArray[i] = getAvgReading(5);
    shine_LED(i, false);
    delay(RGB_WAIT);
    // the differnce between the maximum and the minimum gives the range
    greyDiff[i] = whiteArray[i] - blackArray[i];
  }
  // delay another 5 seconds for getting ready colour objects
  Serial.println("Colour Sensor Is Ready.");
  delay(5000);
  for (int i = 0; i <= 2; i += 1) {
    Serial.print(whiteArray[i]);
    Serial.print(" ");
  }
  Serial.println("");
  for (int i = 0; i <= 2; i += 1) {
    Serial.print(blackArray[i]);
    Serial.print(" ");
  }
  Serial.println("");
  for (int i = 0; i <= 2; i += 1) {
    Serial.print(greyDiff[i]);
    Serial.print(" ");
  }
  Serial.println("");
}
