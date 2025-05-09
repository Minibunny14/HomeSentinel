// MASTER ESP32 CODE (Receiver & HTTP Gateway)
#include <WiFi.h>
#include <esp_now.h>
#include <HTTPClient.h>

// WiFi Credentials
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";
const char* serverURL = "YOUR_ENDPOINT";

// Structure for ESP-NOW communication
typedef struct struct_message {
    char location[15];  // Room identifier (bedroom, kitchen, etc.)
    bool motion_detected;
} struct_message;

// Callback when data is received
void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
    struct_message incoming;
    memcpy(&incoming, incomingData, sizeof(incoming));
    
    Serial.print("Alert from: ");
    Serial.println(incoming.location);
    
    if(incoming.motion_detected) {
        String payload = "{\"location\":\"" + String(incoming.location) + "\",\"status\":\"triggered\"}";
        postToCloud(payload);
    }
}

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) delay(500);
    
    // ESP-NOW Initialization
    WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK) return;
    esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // 1. Maintain WiFi Connection
  if (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED, LOW); // Visual disconnect indicator
    WiFi.reconnect();
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print("Reconnecting to WiFi...");
    }
    digitalWrite(LED, HIGH);
    Serial.println("\nWiFi reconnected!");
  }

  // 2. System Heartbeat Indicator
  static unsigned long lastHeartbeat = 0;
  if (millis() - lastHeartbeat > 2000) { // Blink every 2 seconds
      digitalWrite(LED, !digitalRead(LED));
      lastHeartbeat = millis();
  }

  // 3. Optional: Periodic System Health Check
  static unsigned long lastHealthCheck = 0;
  if (millis() - lastHealthCheck > 60000) { // Every 60 seconds
      Serial.println("System Status:");
      Serial.print("Connected Devices: ");
      // Could add peer count checking here
      Serial.print("WiFi Strength: ");
      Serial.println(WiFi.RSSI());
      postToCloud("{\"system_status\":\"active\"}"); // Cloud heartbeat
      lastHealthCheck = millis();
  }
}

int postToCloud(String payload) {
    HTTPClient http;
    http.begin(serverURL);
    http.addHeader("Content-Type", "application/json");
    int httpCode = http.POST(payload);
    http.end();
    return httpCode;
}