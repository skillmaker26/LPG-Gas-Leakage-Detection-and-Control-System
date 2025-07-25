/* 
 * LPG Gas Leakage Detection and Control System
 * Author: Your Name
 * Date: Current Date
 * 
 * Features:
 * - MQ-6 Gas Sensor Detection
 * - Automatic Valve Control via Servo
 * - Alert Buzzer
 * - LCD Display
 * - WiFi & Bluetooth Notifications
 * - Kitchen Fan Control
 */

// Include necessary libraries
#include <Wire.h>               // For I2C communication
#include <LiquidCrystal_I2C.h>  // For LCD control
#include <Servo.h>              // For servo motor control
#include <SoftwareSerial.h>     // For additional serial ports

// Pin Definitions
#define MQ6_ANALOG A0          // MQ6 analog output pin
#define MQ6_DIGITAL 2          // MQ6 digital output pin
#define BUZZER_PIN 8           // Buzzer control pin
#define SERVO_PIN 9            // Servo control pin
#define FAN_RELAY_PIN 7        // Relay for kitchen fan

// Threshold Values
#define GAS_THRESHOLD 300      // Adjust based on sensor calibration (300-600 typical)
#define SERVO_CLOSED_POS 0     // Servo position for closed valve
#define SERVO_OPEN_POS 90      // Servo position for open valve

// Notification Intervals
#define NOTIFICATION_INTERVAL 60000  // 1 minute between alerts (in ms)

// Initialize Software Serial for communication modules
SoftwareSerial wifiSerial(10, 11);      // RX, TX for ESP8266
SoftwareSerial bluetoothSerial(12, 13);  // RX, TX for HC-05

// Initialize components
LiquidCrystal_I2C lcd(0x27, 16, 2);  // I2C address 0x27, 16x2 display
Servo regulatorValve;                 // Servo object for valve control

// Global Variables
bool gasDetected = false;             // Tracks gas detection state
unsigned long lastNotificationTime = 0; // Last alert timestamp

void setup() {
  // Initialize serial monitors
  Serial.begin(9600);           // Main serial for debugging
  wifiSerial.begin(115200);      // ESP8266 typically uses 115200 baud
  bluetoothSerial.begin(9600);   // HC-05 default baud rate
  
  // Configure pin modes
  pinMode(MQ6_DIGITAL, INPUT);   // MQ6 digital output
  pinMode(BUZZER_PIN, OUTPUT);   // Buzzer control
  pinMode(FAN_RELAY_PIN, OUTPUT);// Fan relay control
  
  // Attach and initialize servo
  regulatorValve.attach(SERVO_PIN);
  regulatorValve.write(SERVO_OPEN_POS);  // Start with valve open
  
  // Initialize LCD
  lcd.init();                     // Initialize the LCD
  lcd.backlight();                // Turn on backlight
  displayStartupMessage();        // Show startup message
  
  // Initialize WiFi module
  setupWiFi();
  
  // Short delay for system stabilization
  delay(2000);
  lcd.clear();
}

void loop() {
  // 1. Read sensor values
  int gasValue = analogRead(MQ6_ANALOG);      // Read analog value (0-1023)
  int digitalValue = digitalRead(MQ6_DIGITAL); // Read digital threshold
  
  // 2. Display gas level on LCD
  updateGasDisplay(gasValue);
  
  // 3. Check for gas leakage
  if (gasValue > GAS_THRESHOLD || digitalValue == HIGH) {
    handleGasLeakage();
  } else {
    handleSafeCondition();
  }
  
  // 4. Small delay between readings
  delay(500);
}

// Function to handle gas detection
void handleGasLeakage() {
  if (!gasDetected) {
    // First detection - trigger all responses
    gasDetected = true;
    triggerAlarm();
    closeValve();
    activateFan();
    sendNotifications();
    lastNotificationTime = millis();
    
    lcd.setCursor(0, 1);
    lcd.print("LEAKAGE!       ");
  } else {
    // Ongoing leakage - send periodic notifications
    if (millis() - lastNotificationTime > NOTIFICATION_INTERVAL) {
      sendNotifications();
      lastNotificationTime = millis();
    }
  }
}

// Function to handle safe conditions
void handleSafeCondition() {
  if (gasDetected) {
    // Just returned to safe state
    gasDetected = false;
    stopAlarm();
    openValve();
    deactivateFan();
    sendAllClearNotification();
    
    lcd.setCursor(0, 1);
    lcd.print("Safe          ");
  }
}

// Alarm control functions
void triggerAlarm() {
  // Alternating tone pattern for attention
  tone(BUZZER_PIN, 1000, 200);  // 1kHz tone for 200ms
  delay(200);
  tone(BUZZER_PIN, 1500, 200);  // 1.5kHz tone for 200ms
}

void stopAlarm() {
  noTone(BUZZER_PIN);           // Stop any playing tones
  digitalWrite(BUZZER_PIN, LOW); // Ensure buzzer is off
}

// Valve control functions
void closeValve() {
  // Slowly close valve (0-90 degrees)
  for (int pos = SERVO_OPEN_POS; pos >= SERVO_CLOSED_POS; pos--) {
    regulatorValve.write(pos);
    delay(15);  // Smooth movement
  }
}

void openValve() {
  // Slowly open valve (0-90 degrees)
  for (int pos = SERVO_CLOSED_POS; pos <= SERVO_OPEN_POS; pos++) {
    regulatorValve.write(pos);
    delay(15);  // Smooth movement
  }
}

// Fan control
void activateFan() {
  digitalWrite(FAN_RELAY_PIN, HIGH);  // Turn on fan
}

void deactivateFan() {
  digitalWrite(FAN_RELAY_PIN, LOW);   // Turn off fan
}

// Display functions
void displayStartupMessage() {
  lcd.setCursor(0, 0);
  lcd.print("LPG Gas Detector");
  lcd.setCursor(0, 1);
  lcd.print("System Booting...");
}

void updateGasDisplay(int gasValue) {
  lcd.setCursor(0, 0);
  lcd.print("Gas Level: ");
  lcd.print(gasValue);
  lcd.print("    ");  // Clear any leftover characters
}

// WiFi setup
void setupWiFi() {
  delay(1000);  // Wait for module startup
  sendATCommand("AT");                 // Test communication
  sendATCommand("AT+CWMODE=1");        // Set to station mode
  sendATCommand("AT+CWJAP=\"SSID\",\"PASSWORD\""); // Replace with your WiFi
}

// Notification functions
void sendNotifications() {
  sendWiFiAlert();
  sendBluetoothAlert();
}

void sendAllClearNotification() {
  sendWiFiAllClear();
  sendBluetoothAllClear();
}

void sendWiFiAlert() {
  wifiSerial.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80");
  delay(2000);
  wifiSerial.println("AT+CIPSEND=100");
  delay(1000);
  wifiSerial.println("GET /apps/thingtweet/1/statuses/update?api_key=YOUR_KEY&status=GAS_LEAK_DETECTED");
  delay(1000);
  wifiSerial.println("AT+CIPCLOSE");
}

void sendBluetoothAlert() {
  bluetoothSerial.println("ALERT: Gas leak detected!");
}

// Helper function for AT commands
void sendATCommand(const char* command) {
  wifiSerial.println(command);
  delay(500);
  while(wifiSerial.available()) {
    Serial.write(wifiSerial.read());  // Echo responses to main serial
  }
}
