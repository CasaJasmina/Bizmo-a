#include <Servo.h>

Servo myShakeServo;
Servo myNodServo;

int shakeServoPos = 0;
int nodServoPos = 0;

int nodNumberTimes = 0;

int shakeServoSpeed = 15;
int nodServoSpeed = 20;

int servoPauseBeforeMove = 150;

unsigned long previousMillis = 0; // last time update
long interval = 3000; // interval at which to do something (milliseconds)

int volCompare = 1.2;

int volCompareLevel1 = ((1 * volCompare) + 1);
int volCompareLevel2 = ((3 * volCompare) + 1);
int volCompareLevel3 = ((5 * volCompare) + 1);

const int sampleWindow = 100; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;

const int sampleWindow2 = 100; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample2;
unsigned long intervalTime;

int motionPin = A3;    // select the input pin for the potentiometer
int motionValue = 0;  // variable to store the value coming from the sensor
int motionTreeshold = 250;

void setup()
{
  Serial.begin(9600);
  myShakeServo.attach(A1);
  myShakeServo.write(90);

  myNodServo.attach(A2);
  myNodServo.write(90);

  pinMode(motionPin, INPUT);
}


void shakeMotion(int shakeAngle)
{
  /// single shake motion
  while (shakeServoPos > shakeAngle) {
    shakeServoPos--;
    myShakeServo.write(shakeServoPos);
    delay(shakeServoSpeed);
  }

  while (shakeServoPos < shakeAngle) {
    shakeServoPos++;
    myShakeServo.write(shakeServoPos);
    delay(shakeServoSpeed);
  }
}


void nodMotion(int nodAngle)
{
  /// single nod motion
  while (nodServoPos > nodAngle) {
    nodServoPos--;
    myNodServo.write(nodServoPos);
    delay(nodServoSpeed);
  }

  while (nodServoPos < nodAngle) {
    nodServoPos++;
    myNodServo.write(nodServoPos);
    delay(nodServoSpeed);
  }

}

void combiMotion(int shakeAngle, int nodAngle)
{
  /// combined motion - to do both shake and nod
  while (shakeServoPos > shakeAngle) {
    shakeServoPos--;
    myShakeServo.write(shakeServoPos);
    delay(shakeServoSpeed);

    if (nodServoPos > nodAngle) {
      nodServoPos--;
      myNodServo.write(nodServoPos);
      delay(nodServoSpeed);
    } else if (nodServoPos < nodAngle) {
      nodServoPos++;
      myNodServo.write(nodServoPos);
      delay(nodServoSpeed);
    }
  }

  while (shakeServoPos < shakeAngle) {
    shakeServoPos++;
    myShakeServo.write(shakeServoPos);
    delay(shakeServoSpeed);

    if (nodServoPos > nodAngle) {
      nodServoPos--;
      myNodServo.write(nodServoPos);
      delay(nodServoSpeed);
    } else if (nodServoPos < nodAngle) {
      nodServoPos++;
      myNodServo.write(nodServoPos);
      delay(nodServoSpeed);
    }
  }

  delay(servoPauseBeforeMove);

  intervalTime = millis();

}


void nodDance(int nodTopAngle, int nodCentreAngle, int nodBottomAngle, int nodNumber, int overrideNodSpeed)
{

  int nodNumberIndex = 1;

  /// reset the nod to the middle
  if (nodServoPos > nodCentreAngle) {
    while (nodServoPos > nodCentreAngle) {
      nodServoPos--;
      myNodServo.write(nodServoPos);
      if (overrideNodSpeed == 0) {
        delay(nodServoSpeed);
      } else {
        delay(overrideNodSpeed);
      }      
    }  
  } else {
    while (nodServoPos < nodCentreAngle) {
      nodServoPos++;
      myNodServo.write(nodServoPos);
      if (overrideNodSpeed == 0) {
        delay(nodServoSpeed);
      } else {
        delay(overrideNodSpeed);
      }
    }
  }


  /// complete a number of nods
  while (nodNumberIndex < nodNumber) {

    /// complete a full down nod
    while (nodServoPos > nodBottomAngle) {
      nodServoPos--;
      myNodServo.write(nodServoPos);
      if (overrideNodSpeed == 0) {
        delay(nodServoSpeed);
      } else {
        delay(overrideNodSpeed);
      }
    }

    /// complete a full up nod
    while (nodServoPos < nodTopAngle) {
      nodServoPos++;
      myNodServo.write(nodServoPos);
      if (overrideNodSpeed == 0) {
        delay(nodServoSpeed);
      } else {
        delay(overrideNodSpeed);
      }

    }

    /// increment the nod index
    nodNumberIndex++;
    
  }

  /// reset the nod to the middle
  if (nodServoPos > nodCentreAngle) {
    while (nodServoPos > nodCentreAngle) {
      nodServoPos--;
      myNodServo.write(nodServoPos);
      if (overrideNodSpeed == 0) {
        delay(nodServoSpeed);
      } else {
        delay(overrideNodSpeed);
      }
    }  
  } else {
    while (nodServoPos < nodCentreAngle) {
      nodServoPos++;
      myNodServo.write(nodServoPos);
      if (overrideNodSpeed == 0) {
        delay(nodServoSpeed);
      } else {
        delay(overrideNodSpeed);
      }
    }
  }

}

void motionSensor(){
  motionValue = analogRead(motionPin);

  if( motionValue < motionTreeshold) {
    nodDance(125, 90, 55, 3, 10);
    
    Serial.println(motionValue);
  }
}


void loop()
{

  motionSensor();

  
  unsigned long startMillis = millis(); // Start of sample window
  unsigned int peakToPeak = 0;   // peak-to-peak level

  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  while (millis() - startMillis < sampleWindow)
  {
    sample = analogRead(A4);
    if (sample < 1024)  // toss out spurious readings
    {
      if (sample > signalMax)
      {
        signalMax = sample;  // save just the max levels
      }
      else if (sample < signalMin)
      {
        signalMin = sample;  // save just the min levels
      }
    }
  }
  peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude



  unsigned long startMillis2 = millis(); // Start of sample window
  unsigned int peakToPeak2 = 0;   // peak-to-peak level

  unsigned int signalMax2 = 0;
  unsigned int signalMin2 = 1024;

  while (millis() - startMillis2 < sampleWindow2)
  {
    sample2 = analogRead(A5);
    if (sample2 < 1024)  // toss out spurious readings
    {
      if (sample2 > signalMax2)
      {
        signalMax2 = sample2;  // save just the max levels
      }
      else if (sample2 < signalMin2)
      {
        signalMin2 = sample2;  // save just the min levels
      }
    }
  }

  
//  Serial.println("Nod dance should be happening");

      
  peakToPeak2 = signalMax2 - signalMin2;  // max - min = peak-peak amplitude


  if (peakToPeak > 120 || peakToPeak2 > 120) {

//    nodDance(125, 90, 55, 2, 30);


    ///// turns to left

    if (peakToPeak > peakToPeak2 * volCompareLevel1 && peakToPeak < peakToPeak2 * volCompareLevel2) {

      Serial.print("mic1 is ");
      Serial.print(volCompareLevel1);
      Serial.print(" to ");
      Serial.print(volCompareLevel2);
      Serial.println(" times higher volume than mic2");


      combiMotion(75, 105);
      

    }

    if (peakToPeak > peakToPeak2 * volCompareLevel2 && peakToPeak < peakToPeak2 * volCompareLevel3) {

      Serial.print("mic1 is ");
      Serial.print(volCompareLevel2);
      Serial.print(" to ");
      Serial.print(volCompareLevel3);
      Serial.println(" times higher volume than mic2");

      combiMotion(60, 115);


    }

    if (peakToPeak > peakToPeak2 * volCompareLevel3) {

      Serial.print("mic1 is more than ");
      Serial.print(volCompareLevel3);
      Serial.println(" times higher volume than mic2");

      combiMotion(45, 125);

    }


    ///// turns to right

    if ( peakToPeak2 > peakToPeak * volCompareLevel1 && peakToPeak2 < peakToPeak * volCompareLevel2 ) {

      Serial.print("mic2 is ");
      Serial.print(volCompareLevel1);
      Serial.print(" to ");
      Serial.print(volCompareLevel2);
      Serial.println(" times higher volume than mic1");

      combiMotion(105, 105);

    }

    if (peakToPeak2 > peakToPeak * volCompareLevel2 && peakToPeak2 < peakToPeak * volCompareLevel3) {

      Serial.print("mic2 is ");
      Serial.print(volCompareLevel2);
      Serial.print(" to ");
      Serial.print(volCompareLevel3);
      Serial.println(" times higher volume than mic1");

      combiMotion(120, 115);

    }

    if (peakToPeak2 > peakToPeak * volCompareLevel3) {

      Serial.print("mic2 is more than ");
      Serial.print(volCompareLevel3);
      Serial.println(" times higher volume than mic1");

      combiMotion(135, 125);

    }

    ///// turn to centre

    if (peakToPeak < peakToPeak2 * volCompareLevel1 && peakToPeak2 < peakToPeak * volCompareLevel1) {


      Serial.print("both mic1 and mic2 have volumes which are no more than ");
      Serial.print(volCompareLevel1);
      Serial.println(" times as loud as the other");

      combiMotion(90, 95);

    }

  }

  if (millis() - intervalTime > 15*1000) {
  
    Serial.println("time to reset");
    
    combiMotion(90, 95);
    
    intervalTime = millis();
  
  }

}
