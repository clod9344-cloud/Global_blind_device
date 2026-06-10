/*
  VisionAssist AI - Dual Ultrasonic Sensor System
  ===============================================
  
  Pin Connections:
  - Top Sensor (Front obstacle): TRIG=D9, ECHO=D8
  - Bottom Sensor (Ground/Stairs): TRIG=D6, ECHO=D5
  - Buzzer: Positive=D11, Negative=GND
  
  Alert Logic:
  - Top sensor: Alert when distance < 30cm (obstacle)
  - Bottom sensor: Alert when distance < 100cm (stairs/curbs)
*/

// Pin Definitions
const int trigPinTop = 9;      // Top sensor TRIG
const int echoPinTop = 8;      // Top sensor ECHO
const int trigPinBottom = 6;    // Bottom sensor TRIG
const int echoPinBottom = 5;    // Bottom sensor ECHO
const int buzzerPin = 11;       // Buzzer positive leg

// Threshold Distances (in centimeters)
const float TOP_THRESHOLD = 30.0;      // Alert if obstacle within 30cm
const float BOTTOM_THRESHOLD = 100.0;  // Alert if stair/curb within 100cm

// Timing Variables
unsigned long lastAlertTime = 0;
const unsigned long ALERT_COOLDOWN = 250;  // Milliseconds between alerts

// Sensor Reading Variables
float topDistance = 0;
float bottomDistance = 0;

void setup() {
  // Start serial communication
  Serial.begin(9600);
  
  // Configure top sensor pins
  pinMode(trigPinTop, OUTPUT);
  pinMode(echoPinTop, INPUT);
  
  // Configure bottom sensor pins
  pinMode(trigPinBottom, OUTPUT);
  pinMode(echoPinBottom, INPUT);
  
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
  Serial.println("VISIONASSIST AI DUAL SENSOR SYSTEM");
  Serial.println("=================================");
  Serial.println("TOP SENSOR:    Alert when < 30cm (obstacle)");
  Serial.println("BOTTOM SENSOR: Alert when < 100cm (stairs/curbs)");
  Serial.println("=================================");
  Serial.println("");
}

void loop() {
  // Read both sensors
  readSensors();
  
  // Send data to serial (for debugging)
  sendSerialData();
  
  // Check for alerts and sound buzzer
  checkAndAlert();
  
  // Small delay to prevent overwhelming the system
  delay(50);
}

// Function to read both sensors
void readSensors() {
  // Read top sensor (obstacle detection)
  digitalWrite(trigPinTop, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinTop, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinTop, LOW);
  
  long durationTop = pulseIn(echoPinTop, HIGH, 30000);
  
  if (durationTop == 0) {
    topDistance = -1;  // No reading
  } else {
    topDistance = durationTop / 58.0;
    if (topDistance > 400) topDistance = -1;
  }
  
  // Small delay between sensor readings to prevent interference
  delay(10);
  
  // Read bottom sensor (stairs/curb detection)
  digitalWrite(trigPinBottom, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinBottom, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinBottom, LOW);
  
  long durationBottom = pulseIn(echoPinBottom, HIGH, 30000);
  
  if (durationBottom == 0) {
    bottomDistance = -1;  // No reading
  } else {
    bottomDistance = durationBottom / 58.0;
    if (bottomDistance > 400) bottomDistance = -1;
  }
}

// Function to send data to serial port
void sendSerialData() {
  // Format: T:123.45,B:67.89
  Serial.print("T:");
  Serial.print(topDistance);
  Serial.print(",B:");
  Serial.println(bottomDistance);
}

// Function to check conditions and sound alert
void checkAndAlert() {
  bool bottomAlert = (bottomDistance > 0 && bottomDistance <= BOTTOM_THRESHOLD);
  bool topAlert = (topDistance > 0 && topDistance <= TOP_THRESHOLD);
  
  // BOTTOM SENSOR HAS HIGHEST PRIORITY (stairs are dangerous)
  if (bottomAlert) {
    if (millis() - lastAlertTime >= ALERT_COOLDOWN) {
      lastAlertTime = millis();
      
      // Pattern: Long beep for stair/curb detection
      digitalWrite(buzzerPin, HIGH);
      delay(200);
      digitalWrite(buzzerPin, LOW);
      
      // Print alert to serial
      Serial.print("STAIR_EDGE! Distance: ");
      Serial.print(bottomDistance);
      Serial.println(" cm");
    }
  }
  // Check top sensor only if no bottom alert
  else if (topAlert) {
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
      Serial.print(topDistance);
      Serial.println(" cm");
    }
  }
  else {
    // No alert - ensure buzzer is off
    digitalWrite(buzzerPin, LOW);
  }
}
