#include <serialize.h>
#include <stdarg.h>
#include "packet.h"
#include "constants.h"
#include <math.h>

typedef enum {
  STOP = 0,
  FORWARD = 1,
  BACKWARD = 2,
  LEFT = 3,
  RIGHT = 4
} TDirection;

volatile TDirection dir = STOP;

/*
   Alex's configuration constants
*/

// Number of ticks per revolution from the
// wheel encoder.

#define COUNTS_PER_REV      134

// Wheel circumference in cm.
// We will use this to calculate forward/backward distance traveled
// by taking revs * WHEEL_CIRC

#define WHEEL_CIRC          20

// Motor control pins. You need to adjust these till
// Alex moves in the correct direction
#define LF                  6   // Left forward pin
#define LR                  5   // Left reverse pin
#define RF                  10  // Right forward pin
#define RR                  9  // Right reverse pin

// ALEX's length and breadth in cm
#define ALEX_LENGTH     17.5
#define ALEX_BREADTH    12.5

int counter = 0;

// PI, for calculating turn circumference
#define PI    3.141592654

// Alex's diagonal (computed and stored once)
float alexDiagonal = 0.0;

// Alex's turning circumference, calculated once
float alexCirc = 0.0;


/*
      Alex's State Variables
*/

// Store the ticks from Alex's left and
// right encoders.
volatile unsigned long leftForwardTicks;
volatile unsigned long rightForwardTicks;
volatile unsigned long leftReverseTicks;
volatile unsigned long rightReverseTicks;

// Left and Right encoder ticks for turning
volatile unsigned long leftForwardTicksTurns;
volatile unsigned long leftReverseTicksTurns;
volatile unsigned long rightForwardTicksTurns;
volatile unsigned long rightReverseTicksTurns;


// Store the revolutions on Alex's left
// and right wheels
volatile unsigned long leftRevs;
volatile unsigned long rightRevs;

// Forward and backward distance traveled
volatile unsigned long forwardDist;
volatile unsigned long reverseDist;

// Variable to keep track of whether we have moved a commanded distance
unsigned long deltaDist;
unsigned long newDist;

// Variables to keep track of our turning angle
unsigned long deltaTicks;
unsigned long targetTicks;

/*

   Alex Communication Routines.

*/

TResult readPacket(TPacket *packet)
{
  // Reads in data from the serial port and
  // deserializes it.Returns deserialized
  // data in "packet".

  char buffer[PACKET_SIZE];
  int len;

  len = readSerial(buffer);

  if (len == 0)
    return PACKET_INCOMPLETE;
  else
    return deserialize(buffer, len, packet);

}

void sendStatus()
{
  // Implement code to send back a packet containing key
  // information like leftForwardTicks, rightForwardTicks, leftRevs, rightRevs
  // forwardDist and reverseDist
  // Use the params array to store this information, and set the
  // packetType and command files accordingly, then use sendResponse
  // to send out the packet. See sendMessage on how to use sendResponse.
  //
  TPacket statusPacket;
  statusPacket.packetType = PACKET_TYPE_RESPONSE;
  statusPacket.command = RESP_STATUS;
  statusPacket.params[0] = leftForwardTicks;
  statusPacket.params[1] = rightForwardTicks;
  statusPacket.params[2] = leftReverseTicks;
  statusPacket.params[3] = rightReverseTicks;
  statusPacket.params[4] = leftForwardTicksTurns;
  statusPacket.params[5] = rightForwardTicksTurns;
  statusPacket.params[6] = leftReverseTicksTurns;
  statusPacket.params[7] = rightReverseTicksTurns;
  statusPacket.params[8] = forwardDist;
  statusPacket.params[9] = reverseDist;
  sendResponse(&statusPacket);
}

void sendMessage(const char *message)
{
  // Sends text messages back to the Pi. Useful
  // for debugging.

  TPacket messagePacket;
  messagePacket.packetType = PACKET_TYPE_MESSAGE;
  strncpy(messagePacket.data, message, MAX_STR_LEN);
  sendResponse(&messagePacket);
}

void dbprint(char *format, ...) {
  va_list args;
  char buffer[128];
  va_start(args, format);
  vsprintf(buffer, format, args);
  sendMessage(buffer);
}

void sendBadPacket()
{
  // Tell the Pi that it sent us a packet with a bad
  // magic number.

  TPacket badPacket;
  badPacket.packetType = PACKET_TYPE_ERROR;
  badPacket.command = RESP_BAD_PACKET;
  sendResponse(&badPacket);

}

void sendBadChecksum()
{
  // Tell the Pi that it sent us a packet with a bad
  // checksum.

  TPacket badChecksum;
  badChecksum.packetType = PACKET_TYPE_ERROR;
  badChecksum.command = RESP_BAD_CHECKSUM;
  sendResponse(&badChecksum);
}

void sendBadCommand()
{
  // Tell the Pi that we don't understand its
  // command sent to us.

  TPacket badCommand;
  badCommand.packetType = PACKET_TYPE_ERROR;
  badCommand.command = RESP_BAD_COMMAND;
  sendResponse(&badCommand);

}

void sendBadResponse()
{
  TPacket badResponse;
  badResponse.packetType = PACKET_TYPE_ERROR;
  badResponse.command = RESP_BAD_RESPONSE;
  sendResponse(&badResponse);
}

void sendOK()
{
  TPacket okPacket;
  okPacket.packetType = PACKET_TYPE_RESPONSE;
  okPacket.command = RESP_OK;
  sendResponse(&okPacket);
}

void sendResponse(TPacket *packet)
{
  // Takes a packet, serializes it then sends it out
  // over the serial port.
  char buffer[PACKET_SIZE];
  int len;

  len = serialize(buffer, packet, sizeof(TPacket));
  writeSerial(buffer, len);
}

/*
   Setup and start codes for external interrupts and
   pullup resistors.

*/

// Enable pull up resistors on pins 2 and 3
void enablePullups()
{
  // Use bare-metal to enable the pull-up resistors on pins
  // 2 and 3. These are pins PD2 and PD3 respectively.
  // We set bits 2 and 3 in DDRD to 0 to make them inputs.
  DDRD &= 0b11110011;
  PORTD |= 0b00001100;
}

// Functions to be called by INT0 and INT1 ISRs.
void leftISR() {

  switch (dir) {
    case FORWARD:
      leftForwardTicks++;
      break;

    case BACKWARD:
      leftReverseTicks++;
      break;

    case LEFT:
      leftReverseTicksTurns++;
      break;

    case RIGHT:
      leftForwardTicksTurns++;
      break;
  }

  // calculate distance
  if (dir == FORWARD) {
    forwardDist = (unsigned long) ((float) leftForwardTicks / COUNTS_PER_REV * WHEEL_CIRC);
  }
  if (dir == BACKWARD) {
    reverseDist = (unsigned long) ((float) leftReverseTicks / COUNTS_PER_REV * WHEEL_CIRC);
  }
}


void rightISR()
{
  switch (dir) {
    case FORWARD:
      rightForwardTicks++;
      break;

    case BACKWARD:
      rightReverseTicks++;
      break;

    case LEFT:
      rightForwardTicksTurns++;
      break;

    case RIGHT:
      rightReverseTicksTurns++;
      break;
  }
}

// Set up the external interrupt pins INT0 and INT1
// for falling edge triggered. Use bare-metal.
void setupEINT()
{
  // Use bare-metal to configure pins 2 and 3 to be
  // falling edge triggered. Remember to enable
  // the INT0 and INT1 interrupts.
  EICRA = 0b00001010; // INTo and INT1 trigger on the falling edge
  EIMSK = 0b00000011; // Enable INTo and INT1 interrupt
}

// Implement the external interrupt ISRs below.
// INT0 ISR should call leftISR while INT1 ISR
// should call rightISR.

ISR(INT0_vect)
{
  leftISR();
}

ISR(INT1_vect)
{
  rightISR();
}


// Implement INT0 and INT1 ISRs above.

/*
   Setup and start codes for serial communications

*/

// Set up the serial connection. For now we are using
// Arduino Wiring, you will replace this later
// with bare-metal code.
void setupSerial()
{
  // To replace later with bare-metal.
  Serial.begin(9600);
}

// Start the serial connection. For now we are using
// Arduino wiring and this function is empty. We will
// replace this later with bare-metal code.

void startSerial()
{
  // Empty for now. To be replaced with bare-metal code
  // later on.

}

// Read the serial port. Returns the read character in
// ch if available. Also returns TRUE if ch is valid.
// This will be replaced later with bare-metal code.

int readSerial(char *buffer)
{

  int count = 0;

  while (Serial.available())
    buffer[count++] = Serial.read();

  return count;
}

// Write to the serial port. Replaced later with
// bare-metal code

void writeSerial(const char *buffer, int len)
{
  Serial.write(buffer, len);
}

/*
   Alex's motor drivers.

*/

// Set up Alex's motors. Right now this is empty, but
// later you will replace it with code to set up the PWMs
// to drive the motors.
void setupMotors()
{
  /* Our motor set up is:
        A1IN - Pin 5, PD5, OC0B  LR
        A2IN - Pin 6, PD6, OC0A  LF
        B1IN - Pin 9, PB1, OC1A  RR
        B2IN - Pin 10, PB2, OC1B RF
  */
   DDRB |= 0b00000110; // Set pin 9 and 10 as output
   DDRD |= 0b01100000; // Set pin 5 and 6 as output
}

// Start the PWM for Alex's motors.
// We will implement this later. For now it is
// blank.
void startMotors()
{
   TCNT0 = 0;
   TCCR0A = 0b00000001; // OC0A and OC0B disconnected
   TCCR0B = 0b00000001; 

   TCNT1 = 0;
   TCCR1A = 0b00000001; // OC1A and OC1B disconnected
   TCCR1B = 0b00000001; 
}

// Convert percentages to PWM values
int pwmVal(float speed)
{
  if (speed < 0.0)
    speed = 0;

  if (speed > 100.0)
    speed = 100.0;

  return (int) ((speed / 100.0) * 255.0);
}

// Move Alex forward "dist" cm at speed "speed".
// "speed" is expressed as a percentage. E.g. 50 is
// move forward at half speed.
// Specifying a distance of 0 means Alex will
// continue moving forward indefinitely.
void forward(float dist, float speed)
{
  dir = FORWARD;

  int val = pwmVal(speed);

  // For now we will ignore dist and move
  // forward indefinitely. We will fix this
  // in Week 9.

  if (dist > 0)
  {
    deltaDist = dist;
  } else {
    deltaDist = 9999999;
  }
    
  newDist = forwardDist + deltaDist;

  // LF = Left forward pin, LR = Left reverse pin
  // RF = Right forward pin, RR = Right reverse pin
  // This will be replaced later with bare-metal code.
 
 //Start RF,LF 

  OCR0A = val;
  TCCR0A = 0b00100001; 
  OCR1B = val;
  TCCR1A = 0b10000001; 
}

// Reverse Alex "dist" cm at speed "speed".
// "speed" is expressed as a percentage. E.g. 50 is
// reverse at half speed.
// Specifying a distance of 0 means Alex will
// continue reversing indefinitely.
void reverse(float dist, float speed)
{
  dir = BACKWARD;

  int val = pwmVal(speed);

  // For now we will ignore dist and
  // reverse indefinitely. We will fix this
  // in Week 9.

  if (dist > 0) {
    deltaDist = dist;
  }  else {
    deltaDist = 9999999;
  }
  newDist = reverseDist + deltaDist;

  // LF = Left forward pin, LR = Left reverse pin
  // RF = Right forward pin, RR = Right reverse pin
  // This will be replaced later with bare-metal code.
  
  // Start RR,LR
  OCR1A = val;
  TCCR1A = 0b00100001; 
  OCR0B = val;
  TCCR0A = 0b10000001; 
}

// new function to estimate the number of wheel ticks
// needed to turn an angle
unsigned long computeDeltaTicks(float ang)
{
  // We will assume that angular distance moved = linear distance moved in one wheel
  // revolution. (incorrect but simplifies correction)
  // # of wheel revs to make one full 360 turn is alexCirc / WHEEL_CIRC
  // This is for 360 degrees. For ang degrees it will ve (ang * alexCirc) / (360 * WHEEL_CIRC)
  // to convert to ticks, we multiply by COUNTS_PER_REV.

  unsigned long ticks = (unsigned long) ((ang * alexCirc * COUNTS_PER_REV) / (360.0 * WHEEL_CIRC));

  return ticks;
}

// Turn Alex left "ang" degrees at speed "speed".
// "speed" is expressed as a percentage. E.g. 50 is
// turn left at half speed.
// Specifying an angle of 0 degrees will cause Alex to
// turn left indefinitely.
void left(float ang, float speed)
{
  dir = LEFT;

  int val = pwmVal(speed);

  if (ang == 0)
  {
    deltaTicks = 99999999;
  } else {
    deltaTicks = computeDeltaTicks(ang);
  }

  targetTicks = leftReverseTicksTurns + deltaTicks;

  // For now we will ignore ang. We will fix this in Week 9.
  // We will also replace this code with bare-metal later.
  // To turn left we reverse the left wheel and move
  // the right wheel forward.
  
  // Start RF,LR
  OCR1B = val;
  TCCR1A = 0b10000001; 
  OCR0B = val;
  TCCR0A = 0b10000001; 
}

// Turn Alex right "ang" degrees at speed "speed".
// "speed" is expressed as a percentage. E.g. 50 is
// turn left at half speed.
// Specifying an angle of 0 degrees will cause Alex to
// turn right indefinitely.
void right(float ang, float speed)
{
  dir = RIGHT;

  int val = pwmVal(speed);

  if (ang == 0)
  {
    deltaTicks = 99999999;
  } else {
    deltaTicks = computeDeltaTicks(ang);
  }

  targetTicks = leftForwardTicksTurns + deltaTicks;

  // For now we will ignore ang. We will fix this in Week 9.
  // We will also replace this code with bare-metal later.
  // To turn right we reverse the right wheel and move
  // the left wheel forward.
  
  // Start RR, LF
  OCR1A = val;
  TCCR1A = 0b00100001; 
  OCR0A = val;
  TCCR0A = 0b00100001; 
}

// Stop Alex. To replace with bare-metal code later.
void stop()
{
  dir = STOP;
  TCCR0A = 0b00000001;  // OC0A and OC0B disconnected
  TCCR1A = 0b00000001;  // OC1A and OC1B disconnected
}

/*
   Alex's setup and run codes

*/

// Clears all our counters
void clearCounters()
{
  leftForwardTicks = 0;
  rightForwardTicks = 0;
  leftReverseTicks = 0;
  rightReverseTicks = 0;
  
  leftForwardTicksTurns = 0;
  leftReverseTicksTurns = 0;
  rightForwardTicksTurns = 0;
  rightReverseTicksTurns = 0;
  leftRevs = 0;
  rightRevs = 0;
  forwardDist = 0;
  reverseDist = 0;
}

// Clears one particular counter
void clearOneCounter(int which)
{
  clearCounters();

}
// Intialize Vincet's internal states

void initializeState()
{
  clearCounters();
}

// A red LED has been connected to PB5, (Arduino pin 13), 
// When, ALEX is executing a movement command, the LED is turned off
// Once movement has been completed, LED is turned back on
// A physical response to indicate when a command has been completed by ALEX.
// Eg. When LED is off, Alex is executing a movement command,
//     When LED is on, Alex is ready for the next command
void handleCommand(TPacket *command)
{
  switch (command->command)
  {
    // For movement commands, param[0] = distance, param[1] = speed.

    case COMMAND_FORWARD:
      sendOK();
      PORTB &= 0b11011111;  // switch off LED
      forward((float) command->params[0], (float) command->params[1]);
      PORTB |= 0b00100000;  // switch on LED
      break;
    
    case COMMAND_REVERSE:
      sendOK();
      PORTB &= 0b11011111;  // switch off LED
      reverse((float) command->params[0], (float) command->params[1]);
      PORTB |= 0b00100000;  // switch on LED
      break;

    case COMMAND_TURN_LEFT:
      sendOK();
      PORTB &= 0b11011111;  // switch off LED
      left((float) command->params[0], 80);     // power has been reduced for turns to improve HECTOR SLAM detection
      PORTB |= 0b00100000; // switch on LED
      break;

    case COMMAND_TURN_RIGHT:
      sendOK();
      PORTB &= 0b11011111; // switch off LED
      right((float) command->params[0], 80);    // power has been reduced for turns to improve HECTOR SLAM detection
      PORTB |= 0b00100000; // switch on LED
      break;

    case COMMAND_STOP:
      sendOK();
      stop();
      break;

    case COMMAND_GET_STATS:
      sendStatus();
      break;

    case COMMAND_CLEAR_STATS:
      clearOneCounter(command->params[0]);
      sendOK();
      break;

    default:
      sendBadCommand();
  }
}

void waitForHello()
{
  int exit = 0;

  while (!exit)
  {
    TPacket hello;
    TResult result;

    do
    {
      result = readPacket(&hello);
    } while (result == PACKET_INCOMPLETE);

    if (result == PACKET_OK)
    {
      if (hello.packetType == PACKET_TYPE_HELLO)
      {


        sendOK();
        exit = 1;
      }
      else
        sendBadResponse();
    }
    else if (result == PACKET_BAD)
    {
      sendBadPacket();
    }
    else if (result == PACKET_CHECKSUM_BAD)
      sendBadChecksum();
  } // !exit
}

void setup() {
  // put your setup code here, to run once:
  // compute the diagonal
  alexDiagonal = sqrt((ALEX_LENGTH * ALEX_LENGTH) + (ALEX_BREADTH * ALEX_BREADTH));
  alexCirc = PI * alexDiagonal;

  cli();
  setupEINT();
  setupSerial();
  startSerial();
  setupMotors();
  startMotors();
  enablePullups();
  initializeState();
  DDRB |= 0b00100000;
  PORTB |= 0b00100000;
  
  // RED LED is connected to arduino pin 13, PB5, to monitor ALEX execution of commands
  DDRB |= 0b00100000;   // Set bit 5 in DDRB as output
  PORTB |= 0b00100000;  // Drive bit 5 in DDRB to HIGH to switch on LED
  sei();
}

void handlePacket(TPacket *packet)
{
  switch (packet->packetType)
  {
    case PACKET_TYPE_COMMAND:
      handleCommand(packet);
      break;

    case PACKET_TYPE_RESPONSE:
      break;

    case PACKET_TYPE_ERROR:
      break;

    case PACKET_TYPE_MESSAGE:
      break;

    case PACKET_TYPE_HELLO:
      break;
  }
}

void loop() {

  // put your main code here, to run repeatedly:
  TPacket recvPacket; // This holds commands from the Pi

  TResult result = readPacket(&recvPacket);

  if (result == PACKET_OK)
    handlePacket(&recvPacket);
  else if (result == PACKET_BAD)
  {
    sendBadPacket();
  }
  else if (result == PACKET_CHECKSUM_BAD)
  {
    sendBadChecksum();
  }

  if (deltaDist > 0)
  {
    if (dir == FORWARD)
    {
      if (forwardDist >= newDist)
      {
        deltaDist = 0;
        newDist = 0;
        stop();
      }
    }
    else if (dir == BACKWARD)
    {
      if (reverseDist >= newDist)
      {
        deltaDist = 0;
        newDist = 0;
        stop();
      }
    }
    else if (dir == STOP)
    {
      deltaDist = 0;
      newDist = 0;
      stop();
    }
  }

  if (deltaTicks > 0)
  {
    if (dir == LEFT)
    {
      if (leftReverseTicksTurns >= targetTicks)
      {
        deltaTicks = 0;
        targetTicks = 0;
        stop();
      }
    } 
    if (dir == RIGHT) 
    {
      if (leftForwardTicksTurns >= targetTicks)
      {
        deltaTicks = 0;
        targetTicks = 0;
        stop();
      }
    } 
    if (dir == STOP) {
      deltaTicks = 0;
      targetTicks = 0;
      stop();
    }
  }
}
