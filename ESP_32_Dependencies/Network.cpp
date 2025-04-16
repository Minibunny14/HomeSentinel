#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "your-ssid";
const char* password = "your-password";
const char* serverName = "http://your-server-ip:port/motion";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected!");
}

void loop() {
  // Simulate motion detection (replace this with your actual sensor logic)
  bool motionDetected = true; 

  if (motionDetected) {
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      http.begin(serverName);
      http.addHeader("Content-Type", "application/json");

      String json = "{\"motion\": true}";
      int httpResponseCode = http.POST(json);

      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);

      http.end();
    }
  }

  delay(10000); // wait 10 seconds before next check
}
