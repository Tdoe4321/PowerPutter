// Include Libraries
#include <Ultrasonic.h>

// Setup pins
#define ultrasonic_trig_pin 3
#define ultrasonic_echo_pin 4

#define motor_pin 10

#define hall_pin 7

// Ultrasonic object
Ultrasonic ultrasonic(ultrasonic_trig_pin, ultrasonic_echo_pin);

// How many tics does the ball need to be in range
int sequential_count = 0;

// Current value of distace to ball
int current_dist = 999;

// Should the motor be turning right now
bool turning = false;

// Value of the hall effect sensor
bool magnet_val = false;
bool last_magnet_val = false;

void setup() {
  Serial.begin(9600);

  pinMode(motor_pin, OUTPUT);

  pinMode(hall_pin, INPUT);  
}

void loop() {
  // Read from sensors this tick
  current_dist = ultrasonic.read();
  magnet_val = digitalRead(hall_pin);

  // See if the ball is in range
  if (current_dist >= 4 && current_dist <= 6){
    sequential_count++;
    
    // If the ball has been in range for X sequential tics
    if (sequential_count >= 500) {
      turning = true; 
    }
  }

  // Reset counter
  else{
    sequential_count = 0;
  }

  // If we should be turning
  if (turning){
    digitalWrite(motor_pin, HIGH);

    // If we currently detect the magnet AND it has moved away from the magnet
    if (!magnet_val && magnet_val != last_magnet_val){
      turning = false;
      digitalWrite(motor_pin, LOW);

      // Stop it from immediatly being able to start again
      delay(1000);
    }
  }

  // Capture last position of magnet for checking if it has moved
  last_magnet_val = magnet_val;

  // Debug info
  Serial.println(current_dist);
}
