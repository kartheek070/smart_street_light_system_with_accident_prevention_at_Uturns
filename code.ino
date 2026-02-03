#include <SoftwareSerial.h>
#include <Arduino.h>

int ldr = A0;
int brightness;
int ir1 = 7;
int eir1 = 10;
int eir2 = 11;
int ir2 = 4;
int led1 = 6;
int led2 = 5;
int ind1 = 2;
int ind2 = 3;

int ldrValue;
int sensor1State;
int sensor2State;
int exitCondition1;
int exitCondition2;
int ind1State = 0;
int ind2State = 0;

//SoftwareSerial Bluetooth(12, 13); // RX (Bluetooth) to pin 8, TX (Bluetooth) to pin 9

void setup() {
  pinMode(ir1, INPUT);
  pinMode(ir2, INPUT);
  pinMode(eir1, INPUT);
  pinMode(eir2, INPUT);
  pinMode(ldr, INPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(ind1, OUTPUT);
  pinMode(ind2, OUTPUT);

  Serial.begin(9600);    // Start Serial Monitor
  //Bluetooth.begin(9600); // Start Bluetooth serial communication
}

void loop() {
  // Read data from Bluetooth
  if (Serial.available()) {
    char command = Serial.read();
    controlSystem(command); // Call function to handle command
  }

  // Existing sensor-controlled code...
  sensor1State = digitalRead(ir1);
  sensor2State = digitalRead(ir2);
  exitCondition1 = digitalRead(eir1);
  exitCondition2 = digitalRead(eir2);

  ldrValue = analogRead(ldr);
  brightness = map(ldrValue, 0, 1023, 0, 5);
  analogWrite(led1, brightness);
  analogWrite(led2, brightness);

  // Existing conditions for IR sensors
  if (sensor1State == 0) {
    digitalWrite(ind2, HIGH);
    ind2State = 1;
    sensor2State = digitalRead(ir2);

    if (ldrValue >= 1000) {
      brightness = map(ldrValue, 0, 1023, 0, 255);
      analogWrite(led1, brightness);
    }

    while (sensor2State != 0) {
      sensor2State = digitalRead(ir2);
      digitalWrite(ind2, HIGH);
      ind2State = 1;
      brightness = map(ldrValue, 0, 1023, 0, 255);
      analogWrite(led1, brightness);

      if (digitalRead(eir1) == 0) {
        brightness = map(ldrValue, 0, 1023, 0, 5);
        analogWrite(led1, brightness);
        digitalWrite(ind2, LOW);
        ind2State = 0;
        delay(50);
        break;
      }
    }

    digitalWrite(ind2, LOW);
    ind2State = 0;
  }

  if (sensor2State == 0) {
    digitalWrite(ind1, HIGH);
    ind1State = 1;
    sensor1State = digitalRead(ir1);

    if (ldrValue >= 1000) {
      brightness = map(ldrValue, 0, 1023, 0, 255);
      analogWrite(led2, brightness);
    }

    while (sensor1State != 0) {
      sensor1State = digitalRead(ir1);
      digitalWrite(ind1, HIGH);
      ind1State = 1;
      brightness = map(ldrValue, 0, 1023, 0, 255);
      analogWrite(led2, brightness);

      if (digitalRead(eir2) == 0) {
        brightness = map(ldrValue, 0, 1023, 0, 5);
        analogWrite(led2, brightness);
        digitalWrite(ind1, LOW);
        ind1State = 0;
        delay(50);
        break;
      }
    }

    digitalWrite(ind1, LOW);
    ind1State = 0;
  }
}

// Function to control the system via Bluetooth commands
void controlSystem(char command) {
  switch (command) {
    case 'a': // Turn on led1 and ind1
      digitalWrite(led1, HIGH);
      digitalWrite(ind1, HIGH);
      ind1State = 1;
      //Bluetooth.println("LED1 and Indicator 1 ON");
      break;

    case 'b': // Turn off led1 and ind1
      digitalWrite(led1, LOW);
      digitalWrite(ind1, LOW);
      ind1State = 0;
      Serial.println("LED1 and Indicator 1 OFF");
      break;

    case 'c': // Turn on led2 and ind2
      digitalWrite(led2, HIGH);
      digitalWrite(ind2, HIGH);
      ind2State = 1;
      //Bluetooth.println("LED2 and Indicator 2 ON");
      break;

    case 'd': // Turn off led2 and ind2
      digitalWrite(led2, LOW);
      digitalWrite(ind2, LOW);
      ind2State = 0;
          Serial.print("LED2 and Indicator 2 OFF");
      break;

    default:
    Serial.write("Invalid Command");
      break;
  }
}