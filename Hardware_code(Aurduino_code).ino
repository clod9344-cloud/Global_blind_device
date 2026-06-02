/*
  Smart_Assist - Dual Ultrasonic Sensor System
  ===========================================
  - Ground sensor (Pin 7,6): Alerts when distance < 100cm (stair detection)
  - Front sensor (Pin 9,8): Alerts when distance < 30cm (obstacle detection)
  - Buzzer on Pin 11: Different patterns for each alert


*/

// Pin Definitions
const int trigPinFront = 9;    // Front sensor TRIG
const int echoPinFront = 8;    // Front sensor ECHO
const int trigPinGround = 7;   // Ground sensor TRIG  
const int echoPinGround = 6;   // Ground sensor ECHO
const int buzzerPin = 11;      // Buzzer positive leg

// Threshold Distances (in centimeters)
const float FRONT_THRESHOLD = 30.0;    // Alert if obstacle within 30cm
const float GROUND_THRESHOLD = 100.0;  // Alert if stair edge within 100cm

// Timing Variables
unsigned long lastAlertTime = 0;
const unsigned long ALERT_COOLDOWN = 250;  // Milliseconds between alerts

// Sensor Reading Variables
float frontDistance = 0;
float groundDistance = 0;

void setup() {
  // Start serial communication
  Serial.begin(9600);
  
  // Configure front sensor pins
  pinMode(trigPinFront, OUTPUT);
  pinMode(echoPinFront, INPUT);
  
  // Configure ground sensor pins
  pinMode(trigPinGround, OUTPUT);
  pinMode(echoPinGround, INPUT);
  
  // Configure buzzer
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);
  
  // Test buzzer to confirm it's working
  digitalWrite(buzzerPin, HIGH);
  delay(200);
  digitalWrite(buzzerPin, LOW);
  delay(100);
  digitalWrite(buzzerPin, HIGH);
  delay(100);
  digitalWrite(buzzerPin, LOW);
  
  // Print startup message
  Serial.println("=================================");
  Serial.println("STAIRASSIST DUAL SENSOR SYSTEM");
  Serial.println("=================================");
  Serial.println("GROUND SENSOR: Alert when < 100cm");
  Serial.println("FRONT SENSOR:  Alert when < 30cm");
  Serial.println("=================================");
  Serial.println("");
}

void loop() {
  // Read both sensors
  readSensors();
  
  // Send data to serial (for Python/website)
  sendSerialData();
  
  // Check for alerts and sound buzzer
  checkAndAlert();
  
  // Small delay to prevent overwhelming the system
  delay(50);
}

// Function to read both sensors
void readSensors() {
  // Read front sensor
  digitalWrite(trigPinFront, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinFront, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinFront, LOW);
  
  long durationFront = pulseIn(echoPinFront, HIGH, 30000);
  
  if (durationFront == 0) {
    frontDistance = -1;  // No reading
  } else {
    frontDistance = durationFront / 58.0;
    if (frontDistance > 400) frontDistance = -1;
  }
  
  // Small delay between sensor readings to prevent interference
  delay(10);
  
  // Read ground sensor
  digitalWrite(trigPinGround, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinGround, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinGround, LOW);
  
  long durationGround = pulseIn(echoPinGround, HIGH, 30000);
  
  if (durationGround == 0) {
    groundDistance = -1;  // No reading
  } else {
    groundDistance = durationGround / 58.0;
    if (groundDistance > 400) groundDistance = -1;
  }
}

// Function to send data to serial port
void sendSerialData() {
  // Format: G:123.45,F:67.89
  Serial.print("G:");
  Serial.print(groundDistance);
  Serial.print(",F:");
  Serial.println(frontDistance);
}

// Function to check conditions and sound alert
void checkAndAlert() {
  bool groundAlert = (groundDistance > 0 && groundDistance <= GROUND_THRESHOLD);
  bool frontAlert = (frontDistance > 0 && frontDistance <= FRONT_THRESHOLD);
  
  // GROUND SENSOR HAS HIGHEST PRIORITY (stairs are dangerous)
  if (groundAlert) {
    // Check cooldown to prevent constant beeping
    if (millis() - lastAlertTime >= ALERT_COOLDOWN) {
      lastAlertTime = millis();
      
      // Pattern: Long beep for stair detection
      digitalWrite(buzzerPin, HIGH);
      delay(200);
      digitalWrite(buzzerPin, LOW);
      
      // Print alert to serial
      Serial.print("STAIR_EDGE! Distance: ");
      Serial.print(groundDistance);
      Serial.println(" cm");
    }
  }
  // Only check front if no ground alert
  else if (frontAlert) {
    if (millis() - lastAlertTime >= ALERT_COOLDOWN) {
      lastAlertTime = millis();
      
      // Pattern: Three short beeps for obstacle
      for (int i = 0; i < 3; i++) {
        digitalWrite(buzzerPin, HIGH);
        delay(60);
        digitalWrite(buzzerPin, LOW);
        delay(60);
      }
      
      // Print alert to serial
      Serial.print("OBSTACLE! Distance: ");
      Serial.print(frontDistance);
      Serial.println(" cm");
    }
  }
  else {
    // No alert - ensure buzzer is off
    digitalWrite(buzzerPin, LOW);
  }
}
