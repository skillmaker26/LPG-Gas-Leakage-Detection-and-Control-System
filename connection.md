complete wiring table with Arduino pin numbers, connected components, and their functions for LPG Gas Leakage Detection and Control System .
---

### 🔌 **1. MQ-6 Gas Sensor**

**Purpose**: Detects LPG gas concentration in air.

| MQ-6 Pin | Connected To  | Notes                                 |
| -------- | ------------- | ------------------------------------- |
| VCC      | 5V (Arduino)  | Power supply                          |
| GND      | GND (Arduino) | Ground connection                     |
| AOUT     | A0 (Arduino)  | Analog output used to read gas level  |
| DOUT     | Not used      | Optional, we use analog for precision |

---

### 💡 **2. 16x2 LCD with I2C Module**

**Purpose**: Display gas level and system status.

| LCD Pin (via I2C) | Connected To      | Notes                           |
| ----------------- | ----------------- | ------------------------------- |
| SDA               | A4 (Arduino UNO)  | Data line for I2C communication |
| SCL               | A5 (Arduino UNO)  | Clock line for I2C              |
| VCC               | 5V (Arduino)      | Power supply                    |
| GND               | GND (Arduino)     | Ground                          |
| Potentiometer     | Used on I2C board | Adjust LCD brightness if needed |


---

###  **3. Buzzer**

**Purpose**: Audio alert when gas is detected.

| Buzzer Pin | Connected To    | Notes                      |
| ---------- | --------------- | -------------------------- |
| +          | Pin 8 (Arduino) | Controlled via digital pin |
| -          | GND (Arduino)   | Ground connection          |

---

### **4. Exhaust Fan (via Relay Module)**

**Purpose**: Ventilate leaked gas from the kitchen.

| Fan Module Pin | Connected To     | Notes                                      |
| -------------- | ---------------- | ------------------------------------------ |
| IN             | Pin 10 (Arduino) | Trigger relay to start fan                 |
| VCC            | 5V               | Relay power supply                         |
| GND            | GND              | Common ground                              |
| NO/COM         | Fan Power        | Fan connected to AC/DC via relay switching |

> Use **optocoupler-based relay module** to isolate Arduino from AC circuits.

---

### **5. Servo Motor (for Gas Valve Control)**

**Purpose**: Automatically close the regulator knob.

| Servo Wire      | Connected To | Notes                                |
| --------------- | ------------ | ------------------------------------ |
| Red (VCC)       | 5V           | Power supply                         |
| Brown (GND)     | GND          | Ground                               |
| Orange (Signal) | Pin 9        | PWM signal to control servo position |

>  Servo draws current—use separate power if needed for stable rotation.

---

### **6. HC-05 Bluetooth Module**

**Purpose**: Sends alerts to nearby Android device via Bluetooth.

| HC-05 Pin | Connected To       | Notes                                                    |
| --------- | ------------------ | -------------------------------------------------------- |
| VCC       | 5V                 | Power supply                                             |
| GND       | GND                | Ground                                                   |
| TXD       | Pin 2 (Arduino RX) | Connect via voltage divider to drop 5V → 3.3V safe level |
| RXD       | Pin 3 (Arduino TX) | No divider needed                                        |


---

### **7. ESP8266 Wi-Fi Module (ESP-01)**

**Purpose**: Send alert via IFTTT (email, app notification).

| ESP8266 Pin | Connected To        | Notes                                              |
| ----------- | ------------------- | -------------------------------------------------- |
| VCC         | 3.3V only           | Never connect to 5V. Use AMS1117 or 3.3V regulator |
| GND         | GND                 | Ground                                             |
| TX          | Pin 11 (Arduino RX) | Use logic level shifter or divider (5V → 3.3V)     |
| RX          | Pin 12 (Arduino TX) | Arduino → ESP via divider (resistors 1k + 2k)      |
| CH\_PD      | 3.3V                | Must be HIGH for ESP to run                        |
| RST         | Open / Manual reset | Optional                                           |

> Note- **ESP8266 is sensitive!** Only use 3.3V logic. You can’t directly power ESP from Arduino's 3.3V pin (insufficient current).


---

##  Summary Table of Connections

| Component       | Arduino Pin(s)   | Notes                                |
| --------------- | ---------------- | ------------------------------------ |
| MQ-6 Gas Sensor | A0               | Analog value read                    |
| LCD (I2C)       | A4, A5           | SDA, SCL for I2C                     |
| Buzzer          | 8                | Digital output                       |
| Fan Relay       | 10               | Digital output                       |
| Servo Motor     | 9                | PWM signal                           |
| HC-05 Bluetooth | 2 (RX), 3 (TX)   | Via SoftwareSerial + voltage divider |
| ESP8266 Wi-Fi   | 11 (RX), 12 (TX) | Use voltage regulator and divider    |

---
