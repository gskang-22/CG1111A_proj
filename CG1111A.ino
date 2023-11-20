//TURNING, ONE_SQUARE_TIME, IR_SENSOR
#include "MeMCore.h"
#include <stdbool.h>

// port 4
#define PIN_A0  A0
#define PIN_A1  A1

//port 3
#define IR_PIN  A2 // pin for Infrared Sensor input
#define LDR_PIN A3 // pin for colour sensor input

// for movement of mbot
#define TURNING_TIME_MS 300 //duration(ms) for turning 90 degrees
#define ONE_SQUARE_TIME_MS 775 //duration(ms) for one square
#define NUDGE_TIME_MS 10 //duration(ms) to nudge mBot
#define NUDGE_INTENSITY 3
#define MOTORSPEED_RIGHT 250 //motor speed for right wheel 200
#define MOTORSPEED_LEFT 240 //motor speed for left wheel 165

// for colour sensor
#define RGB_WAIT 200
#define LDR_WAIT 10
float colourArray[] = {0,0,0}; 
float whiteArray[] = {860.00,641.00,807.00}; // calibrated
float blackArray[] = {54.00,43.00,154.00}; // calibrated
float greyDiff[] = {806.00,598.00,653.00}; // calibrated

// for IR sensor
#define IR_CONSTANT 20 // reduce working range of IR sensor to 4 cm

// for ultrasonic sensor
#define ULTRASONIC 12
#define SPEED_OF_SOUND 340
#define TIMEOUT 2000 // max microseconds to wait; choose according to max distance of wall
#define MIN_DISTANCE 4 // min distance of US sensor from wall 
#define MAX_DISTANCE 18 // max distance of US sensor from wall

// Initial PID tuning parameters for ultrasonic sensor
#define KP 25 // proportional constant 25
#define KD 0 // derivitive constant 50
#define SET_POINT 11.3 // distance from US sensor to wall when mbot is at center of path
float error, derivative, distance, change_in_speed;  
int previousError = 0; 

int status = 0; // global status; 0 = do nothing, 1 = mBot runs

MeDCMotor leftMotor(M1); //left motor
MeDCMotor rightMotor(M2); //right motor
MeBuzzer buzzer; 
MeLineFollower lineFinder(PORT_2); // assigning lineFinder to RJ25 port 2
MeRGBLed led(0,30); // Based on hardware connections on mCore; cannot change

void setup()
{
  Serial.begin(9600);
  led.setpin(13);
  pinMode(7, INPUT);
  pinMode(PIN_A0, OUTPUT);
  pinMode(PIN_A1, OUTPUT);
  pinMode(IR_PIN, INPUT);
  pinMode(LDR_PIN, INPUT);
  pinMode(A7, INPUT); // Setup A7 as input for the push button

  digitalWrite(PIN_A0, LOW);
  digitalWrite(PIN_A1, LOW);

  // uncomment for colour calibration
  //setBalance();
}

void loop()
{
  if (analogRead(A7) < 100) { // when button is pressed
    status = 1 - status; // change status (between 0 and 1)
    delay(500);
  }
  if (status == 1) { // if button has been pressed
    // calibrate IR sensor
    //double ambient_voltage = calibrate_IR();
    //Serial.print("ambient voltage: ");
    //Serial.println(ambient_voltage);

    while (status == 1) {
      // Read ultrasonic sensor distance
      distance = get_distance();
      if (distance >= MIN_DISTANCE && distance <= MAX_DISTANCE) {
        error = SET_POINT - distance;
        derivative = error - previousError;
        previousError = error;
        change_in_speed = KP * error + KD * derivative;

        leftMotor.run(-MOTORSPEED_LEFT + change_in_speed);
        rightMotor.run(MOTORSPEED_RIGHT + change_in_speed);
        Serial.print("change_in_speed: ");
        Serial.println(change_in_speed);
      } else {
        // no wall detected by US sensor
        moveForward();
      }

      // Read IR sensing distance (read IR detector, estimate distance)
      /*float IR_voltage = analogRead(IR_PIN);
      Serial.print("IR voltage: ");
      Serial.println(IR_voltage);
      if (ambient_voltage - IR_voltage > IR_CONSTANT) {
        // ie distance between IR sensor and wall < 4cm
        Serial.println("IR activated");
        nudgeRight();
       }*/

      // if black line detected
      int sensorState = lineFinder.readSensors(); // read the line sensor's state
      if (sensorState == S1_IN_S2_IN) { // black line detected
        stopMotor();
        Colour(); // detect colour + action
        led.setColor(0, 0, 0); // turn off mbot LED
        led.show();   
      }
    } 
  }
}
