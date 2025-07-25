**specific placeholder values** you need to replace in the code

---

### **1. WiFi Credentials**
```cpp
// In setupWiFi() function:
sendATCommand("AT+CWJAP=\"SSID\",\"PASSWORD\""); 
```
**Replace with:**
- `SSID`: Your WiFi network name  
  (e.g., `"HomeWiFi_2.4GHz"`)
- `PASSWORD`: Your WiFi password  
  (e.g., `"SecurePassword123"`)

---

### **2. ThingSpeak API Key**
```cpp
// In sendWiFiAlert() function:
wifiSerial.println("GET /apps/thingtweet/1/statuses/update?api_key=YOUR_KEY&status=GAS_LEAK_DETECTED");
```
**Replace with:**
- Get your key from ThingSpeak:
  1. Sign up at [thingspeak.com](https://thingspeak.com)
  2. Create a new "ThingTweet" app
  3. Copy the **Write API Key**  
  (e.g., `"GET /apps/thingtweet/1/statuses/update?api_key=ABC123XYZ456&status=GAS_LEAK_DETECTED"`)

---
