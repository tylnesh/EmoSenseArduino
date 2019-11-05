#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>



#define TEMPERATURE A3
#define HR A2
#define GSR A1

// Setup a oneWire instance to communicate with any OneWire device
OneWire oneWire(TEMPERATURE);  

// Pass oneWire reference to DallasTemperature library
DallasTemperature sensors(&oneWire);

int LED13 = LED_BUILTIN;
int thresholdGSR = 0;
int sensorValue;
  int temp;

int UpperThreshold = 518;
int LowerThreshold = 490;
int reading = 0;
float BPM = 0.0;
bool IgnoreReading = false;
bool FirstPulseDetected = false;
unsigned long FirstPulseTime = 0;
unsigned long SecondPulseTime = 0;
unsigned long PulseInterval = 0;
const unsigned long delayTime = 10;
const unsigned long delayTime2 = 100;
const unsigned long delayTime3 = 100;
const unsigned long baudRate = 9600;
unsigned long previousMillis = 0;
unsigned long previousMillis2 = 0;
unsigned long previousMillis3 = 0;

double outBPM = 0;
double outGSR = 0;

struct message {
  int GSR = 0;
  int HR = 0;
};

int cmd = 0;

void start() {
  pinMode(LED13, OUTPUT);
  long sum = 0;
  delay(1000);

  for (int i = 0; i < 500; i++) {
    sensorValue = analogRead(GSR);
    sum += sensorValue;
    delay(5);
  }
  thresholdGSR = sum / 500;
  // Serial.print("threshold =");
  // Serial.println(thresholdGSR);
}

void hold() {
  cmd = 0;
  while (true) {

    cmd = Serial.read();
    delay(10);
    if (cmd == 1)
      break;
  }
}


// First event timer
int myTimer1(unsigned long delayTime, unsigned long currentMillis){
  if(currentMillis - previousMillis >= delayTime){previousMillis = currentMillis;return 1;}
  else{return 0;}
}

// Second event timer
int myTimer2(unsigned long delayTime2, unsigned long currentMillis){
  if(currentMillis - previousMillis2 >= delayTime2){previousMillis2 = currentMillis;return 1;}
  else{return 0;}
}
int myTimer3(unsigned long delayTime2, unsigned long currentMillis){
  if(currentMillis - previousMillis3 >= delayTime3){previousMillis3 = currentMillis;return 1;}
  else{return 0;}
}


void setup(){
  Serial.begin(9600);
  sensors.begin(); 
  pinMode(9, OUTPUT);
  digitalWrite(9, LOW);

  cmd = 0;
    //while (true) {

      //cmd = Serial.read();
      //delay(10);
      //if (cmd == 1)
      //  break;
    //}
    pinMode(LED13, OUTPUT);
    long sum = 0;
    delay(1000);

    for (int i = 0; i < 500; i++) {
      sensorValue = analogRead(GSR);
      sum += sensorValue;
      delay(5);
    }
    thresholdGSR = sum / 500;

}

void loop(){


    sensorValue = analogRead(GSR);
  // Get current time
  unsigned long currentMillis = millis();

  // First event
  if(myTimer1(delayTime, currentMillis) == 1){

    reading = analogRead(HR);

    // Heart beat leading edge detected.
    if(reading > UpperThreshold && IgnoreReading == false){
      if(FirstPulseDetected == false){
        FirstPulseTime = millis();
        FirstPulseDetected = true;
      }
      else{
        SecondPulseTime = millis();
        PulseInterval = SecondPulseTime - FirstPulseTime;
        FirstPulseTime = SecondPulseTime;
      }
      IgnoreReading = true;
      digitalWrite(9, HIGH);
    }

    // Heart beat trailing edge detected.
    if(reading < LowerThreshold && IgnoreReading == true){
      IgnoreReading = false;
      digitalWrite(9, LOW);
    }

    // Calculate Beats Per Minute.
    BPM = (1.0/PulseInterval) * 60.0 * 1000;
  }

  // Second event
  if(myTimer2(delayTime2, currentMillis) == 1){
   
    outBPM = BPM;
    //Serial.flush();
  }
  temp = thresholdGSR - sensorValue;
    if (abs(temp) > 50) {
      sensorValue = analogRead(GSR);
      temp = thresholdGSR - sensorValue;
      if (abs(temp) > 50) {
        //Serial.print("GSR: ");
        //Serial.print((abs(temp) - 50));
        outGSR = (abs(temp) - 50);
        //Serial.print("\n");
        delay(100);

        cmd = Serial.read();
        Serial.flush();
        if (cmd == 1)
          hold();
        else if (cmd == 2)
          loop();
      }
    }
if(myTimer3(delayTime3, currentMillis) == 1){
    {
      sensors.requestTemperatures();
    //  Serial.print(" ");
    
    String output = String(outGSR,8) + ";" + String(outBPM,4) + ";" + String(sensors.getTempCByIndex(0),5) ;
    Serial.print(output);
    
    /*Serial.print(outGSR);
    Serial.print(";");
    Serial.print(outBPM);
    Serial.print(";");
    Serial.print(sensors.getTempCByIndex(0));
    Serial.println();
    */
    //Serial.flush();
  }
    //delay(100);
  }

}
