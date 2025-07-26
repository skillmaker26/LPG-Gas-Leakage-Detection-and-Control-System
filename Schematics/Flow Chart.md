# Flow Chart

```mermaid
graph LR
    subgraph Power
        PSU[5V/9V Power Supply]
    end

    subgraph Arduino
        MCU[Arduino UNO]
    end

    subgraph Sensors
        MQ6[MQ-6 Gas Sensor]
    end

    subgraph Outputs
        SERVO[Servo Motor]
        BUZZER[Buzzer]
        FAN[Exhaust Fan]
        LCD[16x2 I2C LCD]
    end

    subgraph Communication
        WIFI[ESP8266]
        BT[HC-05 Bluetooth]
    end

    PSU -->|Vin| MCU
    MCU -->|5V| MQ6
    MCU -->|5V| LCD
    MCU -->|3.3V| WIFI
    MCU -->|5V| BT
    
    MQ6 -->|A0| A0
    MQ6 -->|D0| D2
    
    MCU -->|D9| SERVO
    MCU -->|D8| BUZZER
    MCU -->|D7| FAN
    
    MCU -->|A4| LCD.SDA
    MCU -->|A5| LCD.SCL
    
    MCU -->|TX1/D10| WIFI.RX
    MCU -->|RX1/D11| WIFI.TX
    
    MCU -->|TX2/D12| BT.RX
    MCU -->|RX2/D13| BT.TX

    style PSU fill:#f9f,stroke:#333
    style MCU fill:#9f9,stroke:#333
    style MQ6 fill:#f96,stroke:#333
