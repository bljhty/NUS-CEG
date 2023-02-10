#include <MeMCore.h>
//for motor
MeDCMotor motor1(M1); //right
MeDCMotor motor2(M2); //left
uint8_t motorSpeed = 150; 

//floats to hold colour arrays {Red, Blue, Green}
// The calibration for white and black array have been done on another code (for white and black balance)
float colorArray[] = {0, 0, 0};
float blackArray[] = {637.5, 417, 447.5};
float whiteArray[] = {952, 856.5, 818};

//place holder for average
int avgRead;

//for buzzer
MeBuzzer buzzer;
char i;
int note, duration;

//for Black Line Finder
MeLineFollower lineFinder(PORT_1);

//for Ultrasonic Sensor
MeUltrasonicSensor ultrasonic (PORT_2);

void setup()
{
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);

  Serial.begin(9600);//for Black Line Finder, Infrared Sensor, Ultrasonic Sensor
}

void loop()
{
  offLed();
  
  //move in straight line
  motor1.run( motorSpeed);//Motor1 (Right) Forward is (+) positive
  motor2.run(-motorSpeed);//Motor2 (Left)  Forward is (-) negative
  
  //IR sensor
  Serial.print("voltage = ");
  int rightDist = analogRead(A1);
  Serial.println(rightDist);

  //Ultrasonic distance sensor
  Serial.print("distance (cm) = ");
  int leftDist = ultrasonic.distanceCm ();
  Serial.println(leftDist);

  if (leftDist < 5.5) //if its too close to the left wall
  {
    //correct to the right
    motor2.run(-motorSpeed);
    motor1.run(motorSpeed - 80);
  } else if (rightDist < 330) //if its too close to right wall
  {
    //correct to the left
    motor1.run(motorSpeed);
    motor2.run(-motorSpeed + 130);
  }
  
  //black line finder
  int sensorState = lineFinder.readSensors();
  switch (sensorState)
  {
    case S1_IN_S2_IN: Serial.println("S1_IN_S2_IN");//both sensors senses black line
      motor1.stop();
      motor2.stop();
      checkColour();
      colorTurn();
      motor1.stop();
      motor2.stop();
      break;

    case S1_IN_S2_OUT: Serial.println("S1_IN_S2_OUT"); break; //only one of the two sensors sense a black line
    case S1_OUT_S2_IN: Serial.println("S1_OUT_S2_IN"); break; //only one of the two sensors sense a black line
    case S1_OUT_S2_OUT: Serial.println("S1_OUT_S2_OUT"); break; //no black line
    default: break;
  }
}

/*
 * RED LED   : A3 HIGH, A2 LOW
 * BLUE LED  : A3 LOW,  A2 HIGH
 * GREEN LED : A3 HIGH, A2 HIGH
 */
void onLed(int num) 
{
  digitalWrite(A3, (num % 2) ? HIGH : LOW);
  digitalWrite(A2, (num - 1) ? HIGH : LOW);
}

void offLed() 
{
  digitalWrite(A3, LOW);
  digitalWrite(A2, LOW);
  delay(500);
}

void getReading(int times) 
{
  int total = 0;
  //take the reading however many times was requested and add them up
  for (int i = 0; i < times; i++) {
    total += analogRead(A0);
    delay(100);
  }
  //calculate the average and set it
  avgRead = total / times;
}

void checkColour() 
{
  float greyDiff;
  for (int i = 0; i < 3; i += 1) {
    onLed(i+1);
    delay(500);
    getReading(5); //number is the number of scans to take for average
    colorArray[i] = avgRead;
    greyDiff = whiteArray[i] - blackArray[i];   
    colorArray[i] = (colorArray[i] - blackArray[i]) / (greyDiff) * 255;
    offLed();
  }
}

void colorTurn() 
{
  //for turning, motor speed for the two motors are not the same so the delay for each turn and adjustment are not the same
  if (colorArray[0] > 240 && colorArray[1] < 200 && colorArray[2] < 200 && colorArray[2] > 135) //(red)
  {
    //turn left
    motor1.run(motorSpeed);
    motor2.run(motorSpeed);
    delay (400);
  } else if (colorArray[0] < colorArray[2] && colorArray[0] > 200 && colorArray[1] < 200 && colorArray[2] > 200) //(green)
  {
    //turn right
    motor1.run(-motorSpeed);
    motor2.run(-motorSpeed);
    delay (425);
  } else if (colorArray[0] > colorArray[2] && colorArray[0] > 200 && colorArray[1] < 200 && colorArray[2] > 200) //(orange)
  {
    //reverse slightly to not hit left wall
    motor2.run(motorSpeed);
    delay(400);
    //turn 180 to the left in the same grid
    motor1.run(motorSpeed);
    motor2.run(motorSpeed);
    delay (600);
  } else if (colorArray[0] > 200 && colorArray[0] < 240 && colorArray[1] < 125 && colorArray[2] < 135) //(purple)
  {
    //turn left
    motor1.run(motorSpeed);
    motor2.run(motorSpeed);
    delay (420);
    motor1.stop();
    motor2.stop();
    //move straight for one grid
    motor1.run(motorSpeed);
    motor2.run(-motorSpeed);
    delay (925);
    motor1.stop();
    motor2.stop();
    //turn left again
    motor1.run(motorSpeed);
    motor2.run(motorSpeed);
    delay (420);
  } else if (colorArray[0] < 200 && colorArray[1] < 200 && colorArray[2] < 200 ) //(light blue)
  {
    //turn right
    motor1.run(-motorSpeed);
    motor2.run(-motorSpeed);
    delay (430);
    motor1.stop();
    motor2.stop();
    //move straight for one grid
    motor1.run(motorSpeed);
    motor2.run(-motorSpeed);
    delay (1080);
    motor1.stop();
    motor2.stop();
    //turn right again
    motor1.run(-motorSpeed);
    motor2.run(-motorSpeed);
    delay (460);
  } else if (colorArray[0] > 255 && colorArray[1] > 256 && colorArray[2] > 200) //(white)
  {
    //play victory music 
    for (int i = 0; i < 3; i += 1)
    {
      note = random (100, 1500); // Freq range of numbers
      duration = random (50, 300); // Duration for each notes
      buzzer.tone (note, duration);
      delay(1000);
    }
  }
}
