#include <Wire.h>
#include <LiquidCrystal_I2C.h> // Include the LCD library

// Define solenoid pins
const int solenoid1Pin = 2;
const int solenoid2Pin = 3;

// Define analog pressure sensor pin
const int pressureSensorPin = A0;

// Set pressure thresholds
const float pressureHigh = 5.0;  // Pressure in kg/cm² for normal operation
const float pressureLow = 3.5;   // Pressure in kg/cm² for brake test

// LCD Setup
LiquidCrystal_I2C lcd(0x27, 16, 2); // Set the LCD address and size

void setup() {
  // Initialize solenoid pins as output
  pinMode(solenoid1Pin, OUTPUT);
  pinMode(solenoid2Pin, OUTPUT);
  
  // Initialize the LCD
  lcd.begin(16, 2);
  lcd.print("Pressure: ");
  
  // Initialize serial communication for debugging
  Serial.begin(9600);
}

void loop() {
  // Read pressure from the sensor
  int sensorValue = analogRead(pressureSensorPin);
  
  // Convert sensor value to pressure (you will need to calibrate this for your sensor)
  float pressure = map(sensorValue, 0, 1023, 0, 100);  // Replace this with the correct conversion
  
  // Display pressure on LCD
  lcd.setCursor(0, 1);
  lcd.print(pressure);
  lcd.print(" kg/cm²  ");
  
  // Fill the tank and maintain pressure
  if (pressure < pressureHigh) {
    digitalWrite(solenoid1Pin, HIGH);  // Open solenoid 1 to fill the tank
    digitalWrite(solenoid2Pin, LOW);   // Ensure solenoid 2 is closed
  } else if (pressure > pressureHigh) {
    digitalWrite(solenoid1Pin, LOW);   // Close solenoid 1 to stop filling
  }
  
  // Brake pressure test
  if (pressure >= pressureHigh) {
    // During brake test, reduce pressure to 3.5 kg/cm²
    digitalWrite(solenoid1Pin, LOW);   // Close solenoid 1
    digitalWrite(solenoid2Pin, HIGH);  // Open solenoid 2 to reduce pressure
    
    if (pressure <= pressureLow) {
      digitalWrite(solenoid2Pin, LOW);  // Close solenoid 2 when 3.5 kg/cm² is reached
    }
  }
  
  delay(1000);  // Wait 1 second before the next loop iteration
}
