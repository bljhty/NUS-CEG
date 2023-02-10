// Color calibration test

// boolean to know if the balance has been set
boolean balanceSet = false;

//floats to hold colour arrays
//set black and white array to be all 0 if need to do white and black calibrations
//input the average of the black and white balance into the array when doing color calibrations
float colorArray[] = {0, 0, 0};
//float blackArray[] = {0, 0, 0};
//float whiteArray[] = {0, 0, 0};
float blackArray[] = {637.5, 417, 447.5};
float whiteArray[] = {952, 856.5, 818};

//place holder for average
int avgRead;

void setup() {
  //setup the outputs for the colour sensor
  pinMode(A0, INPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);

  //begin serial communication
  Serial.begin(9600);

}

void loop() {
  //comment out checkBalance and balanceSet = false when checking color
  //comment out check color when checking white and black balance
  checkBalance();
  checkColour();
  printColour();
  balanceSet = false;
}

void checkBalance() {
  //check if the balance has been set, if not, set it
  if (balanceSet == false) {
    setBalance();
  }
}

/*
   RED LED   : A3 HIGH, A2 LOW
   BLUE LED  : A3 LOW,  A2 HIGH
   GREEN LED : A3 HIGH, A2 HIGH
*/
void onLed(int num) {
  digitalWrite(A3, (num % 2) ? HIGH : LOW);
  digitalWrite(A2, (num - 1) ? HIGH : LOW);
}

void offLed() {
  digitalWrite(A3, LOW);
  digitalWrite(A2, LOW);
  delay(500);
}

void getReading(int times) {
  int total = 0;
  //take the reading however many times was requested and add them up
  for (int i = 0; i < times; i+= 1) {
    total += analogRead(A0);
    delay(100);
  }
  //calculate the average and set it
  avgRead = total / times;
}

void setBalance() {
  //set white balance
  Serial.println("put white");
  delay(5000); //delay for five seconds, this gives us time to get a white sample in front of our sensor
  //scan the white sample.
  //go through each light, get a reading, set the base reading for each colour red, blue, and green to the white array
  for (int i = 0; i < 3; i += 1) {
    onLed(i + 1);
    delay(500);
    getReading(5); //number is the number of scans to take for average
    whiteArray[i] = avgRead;
    offLed();
  }

  //done scanning white, now it will pulse blue to tell you that it is time for the black paper.
  //set black balance
  Serial.println("put black");
  delay(5000);              //wait for five seconds so we can position our black sample
  //go ahead and scan, sets the colour values for red, blue, and green when exposed to black
  for (int i = 0; i < 3; i += 1) {
    onLed(i + 1);
    delay(500);
    getReading(5); //number is the number of scans to take for average
    blackArray[i] = avgRead;
    offLed();
  }

  //set boolean value so we know that balance is set
  balanceSet = true;
  //delay another 5 seconds to allow the human to catch up to what is going on
  delay(5000);
}

void checkColour() {
  Serial.println("put color");
  delay(5000);
  float greyDiff;
  for (int i = 0; i < 3; i += 1) {
    onLed(i + 1);
    delay(500);
    getReading(5); //number is the number of scans to take for average
    colorArray[i] = avgRead;
    greyDiff = whiteArray[i] - blackArray[i];
    colorArray[i] = (colorArray[i] - blackArray[i]) / (greyDiff) * 255;
    offLed();
  }
}

//prints the colour in the arrays
void printColour() {
  for (int i = 0; i < 3; i+= 1) {
  Serial.print("white balance = ");
  Serial.println(int(whiteArray[i]));
  }
  for (int i = 0; i < 3; i+= 1) {
  Serial.print("black balance = ");
  Serial.println(int(blackArray[i]));
  }
  Serial.print("R = ");
  Serial.println(int(colorArray[0]));
  Serial.print("B = ");
  Serial.println(int(colorArray[1]));
  Serial.print("G = ");
  Serial.println(int(colorArray[2]));
  delay(2000);
}
