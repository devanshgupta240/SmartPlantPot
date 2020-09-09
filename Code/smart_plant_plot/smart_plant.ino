#include "DHT.h"

#include <Servo.h>
#include <Wire.h>
#include "Air_Quality_Sensor.h"
#include <Digital_Light_TSL2561.h>
#include"Arduino.h"
#define DHTPIN 2
#define DHTTYPE DHT11
#define WATER_SENSOR 3
#define buzz 4
#define motor_front 5 //CW is defined as pin #7//
#define motor_back 6//CCW is defined as pin #8//
#define pump_high 7 //CW is defined as pin #7//
#define pump_low 8 //CCW is defined as pin #8//

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 70;    // variable to store the servo position

// defines pins numbers
const int trigPin = 11;
const int echoPin = 10;
// defines variables
long duration;
int distance;
DHT dht(DHTPIN, DHTTYPE);

AirQualitySensor sensor(A1);
int current_quality = -1;

int extra  = 0;
long int sunlight = 0;
int rain = 0;
int days = 0;

void setup() { //Setup runs once//
  Serial.println("DHTxx test!");
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  Wire.begin();
  Serial.begin(9600);
  TSL2561.init();
  pinMode(buzz, OUTPUT); //Buzzer
  pinMode(WATER_SENSOR, INPUT);
  pinMode(motor_front, OUTPUT);
  pinMode(motor_back, OUTPUT);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(pump_high, OUTPUT); //Set CW as an output//
  pinMode(pump_low, OUTPUT); //Set CCW as an output//
  dht.begin();
  sensor.init();

}

void loop() { //Loop runs forever//
   digitalWrite(buzz, LOW);
  // delay(1000);
   //exit(0);

  digitalWrite(motor_front, LOW);
  digitalWrite(motor_back, LOW);

  //In Shade
  for (int i = 0; i < 20 + extra; i++) {
    if (digitalRead(WATER_SENSOR) == 1) {
      Serial.println("1Your Tank is empty, please fill the water in the tank");
      digitalWrite(buzz, HIGH);
      delay(500);
      digitalWrite(buzz, LOW);
    }

    // read the input on analog pin 0:
    int sensorValue = analogRead(A2);
    // print out the value you read:
    Serial.print("2Soil Moisture : ");
    Serial.println(sensorValue);
    delay(1000);
    Serial.print("2Water : ");
    Serial.println(digitalRead(WATER_SENSOR));

    if (sensorValue > 500 && digitalRead(WATER_SENSOR) == 0 ) {
      digitalWrite(pump_low, LOW); //Motor stops//
      Serial.println("3Pump High");
            digitalWrite(pump_high, HIGH); //Motor runs clockwise//
            delay(500);            //for 1 second//
            digitalWrite(pump_high, LOW); //Motor stops//
    }

    int quality = sensor.slope();

    Serial.print("2Air Quality sensor value : ");
    Serial.println(sensor.getValue());

    if (quality == AirQualitySensor::FORCE_SIGNAL) {
      Serial.println("4High pollution! Force signal active.");
      Serial.println("1The air quality is hazardous, immediate actions need to be taken.");
      digitalWrite(buzz, HIGH);
      delay(500);
      digitalWrite(buzz, LOW);
    }
    else if (quality == AirQualitySensor::HIGH_POLLUTION) {
      Serial.println("4High pollution!");
      Serial.println("1The air quality is not fit for your plant, some action may need to be taken.");
    }
    else if (quality == AirQualitySensor::LOW_POLLUTION) {
      Serial.println("4Low pollution!");
      Serial.println("1We have noticed some anomaly in the air quality.");
    }
    else if (quality == AirQualitySensor::FRESH_AIR) {
      Serial.println("4Fresh air.");
    }
    delay(1000);

    float temp_hum_val[2] = {0};
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)


    if (!dht.readTempAndHumidity(temp_hum_val)) {
      Serial.print("2Humidity : ");
      Serial.print(temp_hum_val[0]);
      Serial.println(" %\t");
      Serial.print("2Temperature : ");
      Serial.print(temp_hum_val[1]);
      Serial.println(" *C");
      if (temp_hum_val[0] >= 90) {
        Serial.println("1It may be raining lookout for plant");
      }
      else if (temp_hum_val[0] >= 50 && temp_hum_val[0] <= 70) {
        Serial.println("4Humidity level is optimal for plant photosynthesis");
      }
      else {
        Serial.println("4Humidity level not optimal for plant photosynthesis");
      }
      if (temp_hum_val[0] < 90 && rain == 1 && i < extra  ) {
        break;
      }


      if (temp_hum_val[1] >= 18 && temp_hum_val[1] <= 28) {
        Serial.println("4Temperature level optimal");
      }
      else if (temp_hum_val[1] < 18) {
        Serial.println("4Temperature too low.");
      }
      else {
        Serial.println("4Temperature too high.");
      }
    }
    else {
      Serial.println("3Failed to get temprature and humidity value.");
    }

    delay(1000);
  }
  sunlight = 0;
  extra = 0;
  rain = 0;
for (pos = 70; pos <= 160; pos += 1) { // goes from 180 degrees to 0 degrees
      myservo.write(pos); 
     // Serial.println(pos);// tell servo to go to position in variable 'pos'
      delay(50);
}
//delay(2000);
   for (int i = 0; i < 5; i++) {
    //Serial.println("enter1");
    for (pos = 160; pos >= 150; pos -= 1) { // goes from 180 degrees to 0 degrees
      //Serial.println("enter2");
      myservo.write(pos);
      //Serial.println(pos);// tell servo to go to position in variable 'pos'
      delay(20);
    }
    for (pos = 150; pos <= 160; pos += 1) { // goes from 0 degrees to 180 degrees
     //Serial.println("enter3"); // in steps of 1 degree
      myservo.write(pos); 
      //Serial.println(pos);// tell servo to go to position in variable 'pos'
      delay(20);                       // waits 15ms for the servo to reach the position
    }
  }
 // delay(2000);
  for (pos = 160; pos >= 70; pos -= 1) { // goes from 180 degrees to 0 degrees
      myservo.write(pos);
      //Serial.println(pos);// tell servo to go to position in variable 'pos'
      delay(50);
}
  delay(1000);
  //Going Front
  for (int i = 0; i < 60; i++)
  {
    // Clears the trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    // Calculating the distance
    distance = duration * 0.034 / 2;
    // Prints the distance on the Serial Monitor
    Serial.print("3Distance: ");
    Serial.println(distance);
    if (distance <= 10) {
      digitalWrite(motor_front, LOW);
      Serial.println("1 There is an obstacle in path of your plant. Please remove it.");
      digitalWrite(buzz, HIGH);
      delay(1000);
      digitalWrite(buzz, LOW);
      delay(2000);

      i--;
    }
    else {
      digitalWrite(motor_front, HIGH);
      delay(100);
    }

  }
  digitalWrite(motor_front, LOW);

  //In Sunlight
  for (int i = 0; i < 20; i++) {
    if (digitalRead(WATER_SENSOR) == 1) {
      Serial.println("1Your Tank is empty, please fill the water in the tank");
      digitalWrite(buzz, HIGH);
      delay(500);
      digitalWrite(buzz, LOW);
    }

    // read the input on analog pin 0:
    int sensorValue = analogRead(A2);
    // print out the value you read:
    Serial.print("2Soil Moisture : ");
    Serial.println(sensorValue);
    delay(1000);
    Serial.print("2Water : ");
    Serial.println(digitalRead(WATER_SENSOR));

    if (sensorValue > 500 && digitalRead(WATER_SENSOR) == 0 ) {
      digitalWrite(pump_low, LOW); //Motor stops//
      Serial.println("3Pump High");
            digitalWrite(pump_high, HIGH); //Motor runs clockwise//
            delay(500);            //for 1 second//
            digitalWrite(pump_high, LOW); //Motor stops//
    }

    int quality = sensor.slope();

    Serial.print("2Air Quality sensor value : ");
    Serial.println(sensor.getValue());

    if (quality == AirQualitySensor::FORCE_SIGNAL) {
      Serial.println("4High pollution! Force signal active.");
      Serial.println("1The air quality is hazardous, immediate actions need to be taken.");
      digitalWrite(buzz, HIGH);
      delay(500);
      digitalWrite(buzz, LOW);
    }
    else if (quality == AirQualitySensor::HIGH_POLLUTION) {
      Serial.println("4High pollution!");
      Serial.println("1The air quality is not fit for your plant, some action may need to be taken.");
    }
    else if (quality == AirQualitySensor::LOW_POLLUTION) {
      Serial.println("4Low pollution!");
      Serial.println("1We have noticed some anomaly in the air quality.");
    }
    else if (quality == AirQualitySensor::FRESH_AIR) {
      Serial.println("4Fresh air.");
    }
    delay(1000);

    float temp_hum_val[2] = {0};
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)


    if (!dht.readTempAndHumidity(temp_hum_val)) {
      Serial.print("2Humidity : ");
      Serial.print(temp_hum_val[0]);
      Serial.println(" %\t");
      Serial.print("2Temperature : ");
      Serial.print(temp_hum_val[1]);
      Serial.println(" *C");
      if (temp_hum_val[0] >= 90) {
        Serial.println("1It may be raining! Lookout for your plant");
        rain = 1;
        extra = 20 - i;
        Serial.print("3Extra hours of Sunlight: ");
        Serial.println(extra);
        break;
      }
      else if (temp_hum_val[0] >= 50 && temp_hum_val[0] <= 70) {
        Serial.println("4Humidity level is optimal for plant photosynthesis");
      }
      else {
        Serial.println("4Humidity level not optimal for plant photosynthesis");
      }

      if (temp_hum_val[1] >= 18 && temp_hum_val[1] <= 28) {
        Serial.println("4Temperature level optimal");
      }
      else if (temp_hum_val[1] < 18) {
        Serial.println("4Temperature too low.");
      }
      else {
        Serial.println("4Temperature too high.");
      }
    }
    else {
      Serial.println("3Failed to get temprature and humidity value.");
    }

    delay(1000);

    Serial.print("2Light Sensor Value : ");
    Serial.println(TSL2561.readVisibleLux());
    sunlight += (long int)TSL2561.readVisibleLux();
    Serial.print("3Sunlight:");
    Serial.println(sunlight);
    if (sunlight >= 100000) {
      days = 0;
      extra = 20 - i;
      Serial.print("3Extra hours of Sunlight: ");
      
      Serial.println(extra);
      break;
    }

    delay(1000);

  }
  if(extra<=1)
    days++;
  if (days == 0) {
    Serial.println("4Sufficient Sunlight received ");
  }
  else {
    if (days > 1) {
      Serial.println("1Your Plant is not receiveing enough sunlight");
    }
    Serial.print("4it has been ");
    Serial.print(days);
    Serial.println("days since plant has not recieved enough sunlight");
  }


  //Going Back
  digitalWrite(motor_back, HIGH);
  delay(8000);
  digitalWrite(motor_back, LOW);
  

}
