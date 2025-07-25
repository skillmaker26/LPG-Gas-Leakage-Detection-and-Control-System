### **Working Principle of the LPG Gas Leakage Detection and Control System**

---

## **1. Gas Sensing Mechanism**
The core detection process begins with the MQ-6 semiconductor gas sensor, which operates on the principle of chemiresistance. The sensor contains a tin dioxide (SnO₂) sensing element that undergoes reversible oxidation-reduction reactions when exposed to hydrocarbon gases like propane and butane. When no gas is present, atmospheric oxygen molecules adsorb onto the SnO₂ surface, capturing free electrons and creating a depletion layer that results in high electrical resistance (typically 10-100 kΩ in clean air).

As LPG molecules interact with the heated sensor surface (maintained at 300-400°C by an internal heater coil), they react with pre-adsorbed oxygen ions. This combustion reaction releases trapped electrons back into the conduction band, dramatically decreasing the sensor's resistance (to 1-10 kΩ when exposed to 500ppm LPG). The sensor's built-in comparator circuit converts this resistance change into both:
- Analog output (0-5V DC proportional to gas concentration)
- Digital output (TTL HIGH when exceeding preset threshold)

---

## **2. Signal Processing and Decision Logic**
The Arduino UNO's 10-bit ADC (Analog-to-Digital Converter) samples the MQ-6's analog output at 100ms intervals, converting the 0-5V signal to a digital value between 0-1023. A moving average filter (5-sample window) eliminates transient noise spikes while preserving actual gas concentration trends.

The system implements three-tiered threat assessment:
1. **Safe Mode** (<300ppm): 
   - LCD displays "Normal: XXppm"
   - Valve remains open (90° position)

2. **Warning Mode** (300-500ppm):
   - Buzzer emits 0.5Hz pulsed beeps
   - LCD shows "Warning: XXppm"

3. **Critical Mode** (>500ppm):
   - Continuous 2kHz alarm siren
   - Servo rotates to 0° (valve closed)
   - Relay energizes exhaust fan
   - Alerts transmitted via WiFi/Bluetooth

The decision algorithm incorporates 50ppm hysteresis to prevent state oscillation near threshold boundaries. For example, the system remains in Warning Mode until concentrations drop below 250ppm, ensuring consistent operation despite minor gas concentration fluctuations.

---

## **3. Actuation and Safety Protocols**
Upon critical leak detection, the system initiates a timed sequence:
   **Immediate Response** (t+0ms):
   - Buzzer activates at 85dB
   - LCD backlight flashes red
   - Valve begins closing (3°/ms)

2. **Secondary Actions** (t+500ms):
   - Exhaust fan activates via 5V relay
   - Cloud notification transmission begins

3. **Persistent Monitoring**:
   - Continues sampling gas levels every 100ms
   - Maintains valve closure until manual reset
   - Fan runs for minimum 5 minutes post-normalization

The servo mechanism employs a worm gear reduction (180:1 ratio) that provides self-locking capability - the valve remains securely closed even if power fails. A Hall effect sensor verifies valve position, while the servo's internal potentiometer provides closed-loop feedback to ensure accurate angular positioning.

---

## **4. Communication Architecture**
The ESP8266 WiFi module establishes TLS-secured MQTT connections to cloud platforms, transmitting JSON-formatted packets containing:
- Device ID and location
- Gas concentration (ppm)
- System status codes
- Timestamp (ISO 8601 format)

Concurrently, the HC-05 Bluetooth module broadcasts alert messages using SPP (Serial Port Profile) to paired Android/iOS devices. The dual-communication approach ensures redundancy - if WiFi connectivity drops, Bluetooth maintains local alert capability. All wireless transmissions include CRC-16 error checking and automatic retry mechanisms for reliable delivery.
