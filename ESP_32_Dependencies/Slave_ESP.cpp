// SLAVE ESP32 CODE (Sensor Node - Bedroom Example)
#include <WiFi.h>
#include <esp_now.h>

// Master MAC Address (REPLACE WITH ACTUAL)
uint8_t masterAddr[] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};

// Message structure
typedef struct struct_message {
    char location[15] = "bedroom"; // Unique per node
    bool motion_detected;
} struct_message;

struct_message sensorData;

// Hardware Pins
const int PIR_PIN = 13;
const int LED_PIN = 2;
int warm_up = 0;

void setup() {
    pinMode(PIR_PIN, INPUT);
    pinMode(LED_PIN, OUTPUT);
    
    WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK) return;
    
    // Register master peer
    esp_now_peer_info_t peerInfo;
    memcpy(peerInfo.peer_addr, masterAddr, 6);
    peerInfo.channel = 0; 
    peerInfo.encrypt = false;
    esp_now_add_peer(&peerInfo);
    
    alarmArmSequence();
    delay(20000); // Warmup period
}

void loop() {
    int sensorState = digitalRead(PIR_PIN);
    
    if (sensorState == HIGH) {
        digitalWrite(LED_PIN, HIGH);
        sensorData.motion_detected = true;
        esp_now_send(masterAddr, (uint8_t *) &sensorData, sizeof(sensorData));
        warm_up = 1;
        delay(2000); // Cooldown
    } else {
        digitalWrite(LED_PIN, LOW);
        if(warm_up == 1) {
            delay(20000); // Post-alert warmup
            alarmArmSequence();
            warm_up = 0;
        }
    }
}

void alarmArmSequence() {
    for(int i=0; i<3; i++) {
        digitalWrite(LED_PIN, HIGH);
        delay(150);
        digitalWrite(LED_PIN, LOW);
        delay(150);
    }
}