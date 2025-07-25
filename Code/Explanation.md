break down **component-by-component** **how the code interacts with the electronics** in **detail**, including **theory**, **electrical behavior**, and **what exactly happens at each step**.

---

#  **Detailed Project Theory – Electronics + Code**

This LPG leak detection system is **a smart safety project** using sensors, actuators, an alert system, and display – all controlled by an Arduino.

---

##  **BLOCK DIAGRAM OVERVIEW**

```
          +---------------------+
          |     MQ-6 Sensor     |
          |  (Analog Output A0) |
          +----------+----------+
                     |
                     v
              +--------------+
              |   Arduino    |
              +--------------+
            /   |     |     \       Wi-Fi/Bluetooth
           /    |     |      \-----> Alert Systems (optional)
    Buzzer  Fan Relay  LCD   Servo
     Pin 8    Pin 7   I2C     Pin 9
```

---

#  1. **MQ-6 Gas Sensor** – *"The Nose"*

###  Working:

* The MQ-6 detects **LPG, methane, propane**, etc.
* It has a **sensitive layer (SnO2)** that changes resistance in presence of gas.
* It outputs a **variable analog voltage (0–5V)**, converted to a number by the Arduino’s `analogRead()` (0–1023 scale).

### Wiring:

| MQ-6 Pin | Connected To |
| -------- | ------------ |
| VCC      | 5V           |
| GND      | GND          |
| A0       | A0 (Arduino) |

###  In Code:

```cpp
int gasValue = analogRead(MQ6_ANALOG);
```

* Reads a value between 0 (clean air) and \~900+ (high gas).

---

#  2. **Buzzer** – *"The Screamer"*

###  Working:

* A **piezo buzzer** beeps when **HIGH signal** (5V) is applied.
* Can also be made to beep at different frequencies using `tone()`.

### Wiring:

| Buzzer Pin | Arduino |
| ---------- | ------- |
| +          | Pin 8   |
| -          | GND     |

###  In Code:

```cpp
digitalWrite(BUZZER_PIN, HIGH);     // Turns buzzer ON
tone(BUZZER_PIN, 1000, 500);        // Optional beep (1kHz for 500ms)
digitalWrite(BUZZER_PIN, LOW);      // Turns buzzer OFF
```

---

#  3. **Liquid Crystal Display (LCD)** – *"The Speaker"*

###  Working:

* A **16x2 LCD with I2C module** is used.
* Shows **gas value** and **status** (Normal, EVACUATE, etc.)
* I2C makes wiring simple – only **2 wires needed** (SDA, SCL).

###  Wiring:

| I2C Pin | Arduino Pin |
| ------- | ----------- |
| VCC     | 5V          |
| GND     | GND         |
| SDA     | A4          |
| SCL     | A5          |

 *I2C uses internal pull-up resistors, so no need to add extra resistors.*

### In Code:

```cpp
lcd.init();                // Starts LCD
lcd.backlight();           // Turns on light
lcd.setCursor(0,0);        // First row
lcd.print("Gas: ");        // Display message
lcd.setCursor(0,1);        // Second row
lcd.print("EVACUATE!");    // Warning
```

---

#  4. **Servo Motor** – *"The Valve Controller"*

### 🔧 Working:

* A small **5V servo motor** controls the **regulator valve** position.
* `regulatorValve.write(angle);` sets the angle.

  * `0°` → Closed
  * `90°` → Open

###  Wiring:

| Servo Wire  | Arduino |
| ----------- | ------- |
| Red         | 5V      |
| Brown/Black | GND     |
| Orange      | Pin 9   |

 *If servo jitter occurs, use a separate 5V supply and connect GNDs together.*

###  In Code:

```cpp
regulatorValve.write(0);   // Close valve on leak
regulatorValve.write(90);  // Open when normal
```

---

#  5. **Fan Relay** – *"The Ventilation Trigger"*

###  Working:

* A relay turns ON a **high-power fan** to clear the gas.
* Arduino sends 5V to relay IN pin → relay closes circuit → fan gets powered.

### Wiring:

| Relay Pin | Arduino |
| --------- | ------- |
| VCC       | 5V      |
| GND       | GND     |
| IN        | Pin 7   |

*Connect fan to NO (Normally Open) and COM terminals on relay.*

### In Code:

```cpp
digitalWrite(FAN_RELAY_PIN, HIGH);  // Turns fan ON
digitalWrite(FAN_RELAY_PIN, LOW);   // OFF
```

---

# 6. **Serial Monitor** – *"The Debug Terminal"*

```cpp
Serial.begin(9600);
Serial.print("TEST CASE "); // Shown on PC
```

* Useful for debugging.
* You can see gas values, current status, and which case is running (in test mode).

---

# **Gas Leak Simulation**

When `#define FAKE_MODE 1`:

* Instead of real gas values, you use fake values:

  * Normal: 250
  * Small Leak: 450
  * Medium Leak: 650
  * Critical: 900

They cycle every 10 seconds to test response.

---

# **Decision Logic**

```cpp
if (gasValue > 500) {
  lcd.print("EVACUATE!");
  buzzer ON
  fan ON
  valve close
} else {
  lcd.print("Normal");
  buzzer OFF
  fan OFF
  valve open
}
```

You can change the **threshold (500)** based on actual gas testing in your area.

---


###  Bluetooth Alerts

* Connect HC-05 to TX/RX pins.
* Send SMS alert from Arduino (or to Android app).

### Wi-Fi Alerts

* Use ESP8266 or NodeMCU.
* Send IFTTT webhook to Gmail/SMS.

---

# 🔌 **Complete Pin Mapping Table**

| Component     | Arduino Pin | Type            | Purpose                    |
| ------------- | ----------- | --------------- | -------------------------- |
| MQ-6 Sensor   | A0          | Analog In       | Detects gas level          |
| LCD (I2C) SDA | A4          | I2C             | LCD data line              |
| LCD (I2C) SCL | A5          | I2C             | LCD clock line             |
| Buzzer        | Pin 8       | Digital Out     | Sound alert                |
| Servo Motor   | Pin 9       | PWM Out         | Valve control              |
| Fan Relay     | Pin 7       | Digital Out     | Turns ON exhaust fan       |
| HC-05 RX      | Pin 10      | TX from Arduino | Bluetooth alert optional   |
| HC-05 TX      | Pin 11      | RX to Arduino   | Bluetooth receive optional |

---

#  **Final Result: How It Works in Real Life**

1. **Normal Air**:

   * Gas value low (< 500)
   * Fan OFF, Valve Open, Buzzer OFF
   * LCD: “Gas: 250\nNormal”

2. **Gas Detected**:

   * Gas value > 500
   * Buzzer Beeps
   * Fan Relay Turns ON
   * Valve Closes
   * LCD: “Gas: 650\nEVACUATE!”

3. **Auto Reset**:

   * If air clears, system auto-returns to safe state.

---


