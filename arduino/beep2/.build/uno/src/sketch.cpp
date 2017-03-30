#include <Arduino.h>
#include <Stepper.h>
void setup();
void defineTestAsset();
void menu();
void displayStats();
void bipodTestCycle80RPM(int handleRotations);
void bipodTest80RPM(int numCycles);
void bipodTestwithStop(int numTillBreak);
void checkForInput();
double oneRotationClockWise();
double oneRotationCounterClockWise();
double oneQuaterRotationClockWise();
double oneQuaterRotationCounterClockWise();
void loop();
#line 1 "src/sketch.ino"
/*
 Stepper Motor Control for Bipod Test

 This program drives a unipolar or bipolar stepper motor.
 The motor is attached to digital pins 8 - 12 of the Arduino.
 The Motor and Test suite can be controlled through the serial ports


 Created 11 Mar, 2017
 by Greg Byrne

 */

//#include <Stepper.h>

/*--------------------------Program Variables to Change -------------------------------------------------*/
const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
const int stepsPerQTRRevolution = 50;
const double adcRefVoltage = 1.1;
// for your motor
int rpm = 80;
int rotationsTillStop = 42;
int testCycles = 600;
const int cyclesTillTorque = 50;
const int inputInterval = 200; // for serial input polling
const int adcSamplesPerRotation = 8; // for current sense poling clockwise 8
const int adcSamplesPerRotationCC = 8; // for current sense poling counter clockwise 8
/*-------------------------- End Program Variables to Change --------------------------------------------------*/

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);

/* ------ pin declarations ---- */
int senA = A0;
int senB = A1;
int driveEN = 12;
/* ------ End pin declarations ---- */
/* ------ Variables declarations ---- */
double peakCurrent = 0;
double avgCurrent = 0;
double turnCurrent = 0;
int cycles = 0;
long rotationsCompleted = 0;
unsigned long system_Second = 0;
unsigned int system_Minute = 0;
unsigned int system_Hour = 0;
unsigned long startTime = 0;
unsigned long previousMillis = 0;
unsigned long previousSampleMillis = 0;
char bipodNumber = '0';
char testDay = '0';
/* ------ End Variables declarations ---- */

void setup() {
  
  // set the speed at 80 rpm:
  pinMode(driveEN, OUTPUT);
  digitalWrite(driveEN, LOW);
  analogReference(INTERNAL);
  myStepper.setSpeed(rpm);
  // initialize the serial port:
  Serial.begin(115200);
  defineTestAsset();
  menu();
}
void defineTestAsset()
{
   Serial.println("Bipod Test 80rpm");
   Serial.println("-------------------");
   Serial.println("Enter Bipod Number (1-4)");
   bipodNumber = Serial.read();
   while(bipodNumber != '1' && bipodNumber != '2'&& bipodNumber != '3'&& bipodNumber != '4')
   {
      bipodNumber = Serial.read();
      delay(250);
   }
  
   Serial.println("-------------------");
   Serial.print("Bipod # ");
   Serial.print(bipodNumber);
   Serial.print(" Selected!!\n");
   Serial.println("-------------------");
   Serial.println("What Day of Testing is this Bipod about to start?");
   while(testDay != '1' && testDay != '2'&& testDay != '3'&& testDay != '4' && testDay != '5')
   {
      testDay = Serial.read();
      delay(250);
   }
   Serial.println("-------------------");
   Serial.print("Test Day: ");
   Serial.print(testDay);
   Serial.print(" Entered for Bipod #");
   Serial.print(bipodNumber);
   Serial.print(" !!\n");
}
void menu()
{
   Serial.print("Bipod Test with Bipod # ");
   Serial.print(bipodNumber);
   Serial.print(" Test Day: ");
   Serial.print(testDay);
   Serial.print("\n");
   Serial.println("-------------------");
   Serial.println("Enter 1 ----> Start Bipod Test");
   Serial.println("Enter 2 ----> Pause Test");
   Serial.println("Enter 3 ----> Display Test Stats");
   Serial.println("Enter 4 ----> Rotate Clockwise 1 rotation");
   Serial.println("Enter 5 ----> Rotate Counter Clockwise 1 rotation");
   Serial.println("Enter 6 ----> Start Bipod Test with stops for Torque Measurments");
   Serial.println("Enter 7 ----> Rotate Clockwise 1/4 rotation");
   Serial.println("Enter 8 ----> Rotate Counter Clockwise 1/4 rotation");
   Serial.println("-------------------");
}

void displayStats()
{
  //rotations = qtrRotations / 4;
  system_Second = (millis() - startTime)/1000;
  system_Minute = system_Second/60;
  system_Hour = system_Minute/60;
  
  Serial.println("Test Statistics");
  Serial.print("Bipod # ");
  Serial.print(bipodNumber);
  Serial.print(" Test Day: ");
  Serial.print(testDay);
  Serial.print("\n");
  Serial.println("-------------------");
  Serial.print("Number of Cycles:");
  Serial.print(cycles);
  Serial.print("\n");
  Serial.print("Number of Handle Rotations:");
  Serial.print(rotationsCompleted);
  Serial.print("\n");
  Serial.print("Peak Current:");
  Serial.print(peakCurrent,4);
  Serial.print(" Amps\n");
  Serial.print("Test Timer:");
  if(system_Hour%24 < 10)
  {
    Serial.print("0");
    Serial.print(system_Hour%24);
  }
  else
  {
    Serial.print(system_Hour%24);
  }
  Serial.print(":");
  if(system_Minute%60 < 10)
  {
    Serial.print("0");
    Serial.print(system_Minute%60);
  }
  else
  {
    Serial.print(system_Minute%60);
  }
  Serial.print(":");
  if(system_Second%60 < 10)
  {
    Serial.print("0");
    Serial.print(system_Second%60);
  }
  else
  {
    Serial.print(system_Second%60);
  }
  Serial.print("\n");
  Serial.println("-------------------");
}

void bipodTestCycle80RPM(int handleRotations)
{
    unsigned long currentSampleMillis = millis();
    myStepper.setSpeed(rpm);
    Serial.print("Start handle rotation clockwise for ");
    Serial.print(handleRotations);
    Serial.print(" turns\n");
    digitalWrite(driveEN, HIGH);
    avgCurrent = 0;
    turnCurrent = 0;
    for(int i = 0; i < handleRotations; i++)
    {
      turnCurrent = oneRotationClockWise();
     // Serial.print("Current Draw:");
     // Serial.print(turnCurrent,4);
     // Serial.print(" Amps \n");
      if(turnCurrent > peakCurrent)
      {
        peakCurrent = turnCurrent;
      }
      avgCurrent += turnCurrent;
      rotationsCompleted++;
      checkForInput();
    }
    Serial.println("Pause for 5 Seconds");
    digitalWrite(driveEN, LOW);
    avgCurrent = avgCurrent / handleRotations;
    Serial.println("-------------------");
    Serial.print("Average Current Draw:");
    Serial.print(avgCurrent,4);
    Serial.print(" Amps for last ");
    Serial.print(handleRotations);
    Serial.print(" turns \n");
    Serial.println("-------------------");
    for(int i = 0; i < 20; i++)
    {
      delay(250);
      checkForInput();
    }
    
    Serial.print("Start handle rotation counter clockwise for ");
    Serial.print(handleRotations);
    Serial.print(" turns \n");
    digitalWrite(driveEN, HIGH);
    avgCurrent = 0;
    turnCurrent = 0;
    for(int i = 0; i < handleRotations; i++)
    {
      turnCurrent = oneRotationCounterClockWise();
      //Serial.print("Current Draw:");
     // Serial.print(turnCurrent,4);
     // Serial.print(" Amps \n");
      if(turnCurrent > peakCurrent)
      {
        peakCurrent = turnCurrent;
      }
      avgCurrent += turnCurrent;
      rotationsCompleted++;
      checkForInput();
    }
    Serial.println("Pause for 15 Seconds");
    digitalWrite(driveEN, LOW);
  
    cycles++;
    avgCurrent = avgCurrent / handleRotations;
    Serial.println("-------------------");
    Serial.print("Average Current Draw:");
    Serial.print(avgCurrent,4);
    Serial.print(" Amps for last ");
    Serial.print(handleRotations);
    Serial.print(" turns\n");
    Serial.println("-------------------");
    for(int i = 0; i < 60; i++)
    {
      delay(250);
      checkForInput();
    }
}

void bipodTest80RPM(int numCycles)
{
    Serial.println("-------------------");
    Serial.print("Test Starting for ");
    Serial.print(numCycles);
    Serial.print(" cycles with ");
    Serial.print(rotationsTillStop * 2);
    Serial.print(" handle rotations per cycle\n");
    Serial.println("-------------------");
    startTime = millis();
  for(int i = 0; i < numCycles; i++)
  {
    bipodTestCycle80RPM(rotationsTillStop);
    displayStats();
  }
    Serial.println("-------------------");
    Serial.print("Test Finished for ");
    Serial.print(numCycles);
    Serial.print(" cycles \n");
    displayStats();
}

void bipodTestwithStop(int numTillBreak)
{
    char continueTest = '6';
    Serial.println("-------------------");
    Serial.print("Test Starting for Bipod stopping after every ");
    Serial.print(numTillBreak);
    Serial.print(" cycles with ");
    Serial.print(rotationsTillStop * 2);
    Serial.print(" handle rotations per cycle\n");
    Serial.println("-------------------");
    startTime = millis();
    for(int j = 1; j<= 12; j++)
    {
        for(int i = 0; i < numTillBreak; i++)
        {
          bipodTestCycle80RPM(rotationsTillStop);
          displayStats();
        }
        Serial.println("Torque Measurments Should be taken now");
        Serial.print("Torque Measurment: ");
        Serial.print(j);
        Serial.print(" out of 12 \n");
        continueTest = '2';
        Serial.println("-------------------");
        Serial.println("Enter 1 ----> Start Bipod Test Again");
        Serial.println("Enter 2 ----> Pause Test");
        Serial.println("-------------------");
        while(continueTest == '2')
        {
          if(Serial.available())
          {
            continueTest = Serial.read();
          }
          delay(250);
        }
     } 
     Serial.println("-------------------");
     Serial.print("Test Finished for ");
     Serial.print(numTillBreak*12);
     Serial.print(" cycles \n");
     displayStats();
}


     
 
void checkForInput()
{
  if(Serial.available())
  {
    char pause = Serial.read();
    if(pause == '2')
    {
      Serial.println("-------------------");
      Serial.print("Bipod # ");
      Serial.print(bipodNumber);
      Serial.print(" Test Paused\n");
      Serial.println("-------------------");
      digitalWrite(driveEN, LOW);
      while(pause == '2')
      {
        if(Serial.available())
        {
          pause = Serial.read();
        }
        delay(250);
      }
      Serial.println("-------------------");
      Serial.print("Bipod # ");
      Serial.print(bipodNumber);
      Serial.println(" Test Resuming\n");
      Serial.println("-------------------");
      digitalWrite(driveEN, HIGH);
    }
    if(pause == '3')
    {
      displayStats();
    }
  }
}

double oneRotationClockWise()
{
      double currentA = 0;
      double currentB = 0;
      double totalCurrent = 0;
      int peakSensorA = 0;
      int peakSensorB = 0;
      int sensorValueA = 0;
      int sensorValueB = 0;
      double voltA = 0;
      double voltB = 0;
      int stepAmmount = stepsPerRevolution/adcSamplesPerRotation;
      digitalWrite(driveEN, HIGH);
      for(int i=0; i < adcSamplesPerRotation; i++)
      {
        myStepper.step(stepAmmount);
        sensorValueA = analogRead(senA);
        sensorValueB = analogRead(senB);
        if(sensorValueA > peakSensorA)
        {
          peakSensorA = sensorValueA;
        }
        if(sensorValueB > peakSensorB)
        {
          peakSensorB = sensorValueB;
        }
      }
      digitalWrite(driveEN, LOW);
      
      voltA = (peakSensorA)*(adcRefVoltage/1024.0);
      voltB = (peakSensorB)*(adcRefVoltage/1024.0);
      currentA = voltA/0.5;
      currentB = voltB/0.5;
      if(currentA > currentB)
      {
        totalCurrent = currentA*2;
      }
      else if (currentB > currentA)
      {
        totalCurrent = currentB*2;
      }
      else
      {
        totalCurrent = currentA+currentB;
      }
      /*
      Serial.print("VoltA:");
      Serial.print(voltA,4);
      Serial.print(" VoltB:");
      Serial.print(voltB,4);
      Serial.print("\n");
      Serial.print("CurrentA:");
      Serial.print(currentA,4);
      Serial.print(" CurrentB:");
      Serial.print(currentB,4);
      Serial.print("\n");
      Serial.print("Total Current:");
      Serial.print(totalCurrent,4);
      Serial.print(" Amps \n");
      */
      return totalCurrent;
}

double oneRotationCounterClockWise()
{
      double currentA = 0;
      double currentB = 0;
      double totalCurrent = 0;
      int peakSensorA = 0;
      int peakSensorB = 0;
      int sensorValueA = 0;
      int sensorValueB = 0;
      double voltA = 0;
      double voltB = 0;
      int stepAmmount = -stepsPerRevolution/adcSamplesPerRotationCC;
      digitalWrite(driveEN, HIGH);
      for(int i=0; i < adcSamplesPerRotationCC; i++)
      {
        myStepper.step(stepAmmount);
        sensorValueA = analogRead(senA);
        sensorValueB = analogRead(senB);
        if(sensorValueA > peakSensorA)
        {
          peakSensorA = sensorValueA;
        }
        if(sensorValueB > peakSensorB)
        {
          peakSensorB = sensorValueB;
        }
      }
      digitalWrite(driveEN, LOW);
      
      voltA = (peakSensorA)*(adcRefVoltage/1024.0);
      voltB = (peakSensorB)*(adcRefVoltage/1024.0);
      currentA = voltA/0.5;
      currentB = voltB/0.5;
      if(currentA > currentB)
      {
        totalCurrent = currentA*2;
      }
      else if (currentB > currentA)
      {
        totalCurrent = currentB*2;
      }
      else
      {
        totalCurrent = currentA+currentB;
      }
      /*
      Serial.print("VoltA:");
      Serial.print(voltA,4);
      Serial.print(" VoltB:");
      Serial.print(voltB,4);
      Serial.print("\n");
      Serial.print("CurrentA:");
      Serial.print(currentA,4);
      Serial.print(" CurrentB:");
      Serial.print(currentB,4);
      Serial.print("\n");
      Serial.print("Total Current:");
      Serial.print(totalCurrent,4);
      Serial.print(" Amps \n");
      */
      return totalCurrent;
}

double oneQuaterRotationClockWise()
{
      double currentA = 0;
      double currentB = 0;
      double totalCurrent = 0;
      int peakSensorA = 0;
      int peakSensorB = 0;
      int sensorValueA = 0;
      int sensorValueB = 0;
      double voltA = 0;
      double voltB = 0;
      int stepAmmount = stepsPerRevolution/4/adcSamplesPerRotation;
      digitalWrite(driveEN, HIGH);
      for(int i=0; i < adcSamplesPerRotation; i++)
      {
        myStepper.step(stepAmmount);
        sensorValueA = analogRead(senA);
        sensorValueB = analogRead(senB);
        if(sensorValueA > peakSensorA)
        {
          peakSensorA = sensorValueA;
        }
        if(sensorValueB > peakSensorB)
        {
          peakSensorB = sensorValueB;
        }
      }
      digitalWrite(driveEN, LOW);
      
      voltA = (peakSensorA)*(adcRefVoltage/1024.0);
      voltB = (peakSensorB)*(adcRefVoltage/1024.0);
      currentA = voltA/0.5;
      currentB = voltB/0.5;
      if(currentA > currentB)
      {
        totalCurrent = currentA*2;
      }
      else if (currentB > currentA)
      {
        totalCurrent = currentB*2;
      }
      else
      {
        totalCurrent = currentA+currentB;
      }
      return totalCurrent;
}

double oneQuaterRotationCounterClockWise()
{
      double currentA = 0;
      double currentB = 0;
      double totalCurrent = 0;
      int peakSensorA = 0;
      int peakSensorB = 0;
      int sensorValueA = 0;
      int sensorValueB = 0;
      double voltA = 0;
      double voltB = 0;
      int stepAmmount = -stepsPerRevolution/4/adcSamplesPerRotationCC;
      digitalWrite(driveEN, HIGH);
      for(int i=0; i < adcSamplesPerRotationCC; i++)
      {
        myStepper.step(stepAmmount);
        sensorValueA = analogRead(senA);
        sensorValueB = analogRead(senB);
        if(sensorValueA > peakSensorA)
        {
          peakSensorA = sensorValueA;
        }
        if(sensorValueB > peakSensorB)
        {
          peakSensorB = sensorValueB;
        }
      }
      digitalWrite(driveEN, LOW);
      
      voltA = (peakSensorA)*(adcRefVoltage/1024.0);
      voltB = (peakSensorB)*(adcRefVoltage/1024.0);
      currentA = voltA/0.5;
      currentB = voltB/0.5;
      if(currentA > currentB)
      {
        totalCurrent = currentA*2;
      }
      else if (currentB > currentA)
      {
        totalCurrent = currentB*2;
      }
      else
      {
        totalCurrent = currentA+currentB;
      }
      return totalCurrent;
}

void loop() 
{
  unsigned long currentMillis = millis();
  if(Serial.available() && (currentMillis - previousMillis >= inputInterval))
  {
    previousMillis = currentMillis;
    menu();
    char ch = Serial.read();
    if(ch == '1')
    {
      bipodTest80RPM(testCycles);
    }
    else if(ch == '2')
    {
      Serial.println("-------------------");
      Serial.println("No Test Running so no need to Pause :) !!");
      Serial.println("-------------------");
    }
    else if(ch == '3')
    {
      displayStats();
    }
    else if(ch == '4')
    {
      Serial.println("Clockwise one rotation");
      Serial.print("Total Current:");
      Serial.print(oneRotationClockWise(),4);
      Serial.print(" Amps \n");
    }
    else if(ch == '5')
    {
      Serial.println("Counterclockwise one rotation");
      Serial.print("Total Current:");
      Serial.print(oneRotationCounterClockWise(),4);
      Serial.print(" Amps \n");
    }
    else if(ch == '6')
    {
      bipodTestwithStop(cyclesTillTorque);
    }
    else if(ch == '7')
    {
      Serial.println("Clockwise 1/4 rotation");
      Serial.print("Total Current:");
      Serial.print(oneQuaterRotationClockWise(),4);
      Serial.print(" Amps \n");
    }
    else if(ch == '8')
    {
      Serial.println("Counterclockwise 1/4 rotation");
      Serial.print("Total Current:");
      Serial.print(oneQuaterRotationCounterClockWise(),4);
      Serial.print(" Amps \n");
    }
    else
    {
      displayStats();
    }
  }
}
