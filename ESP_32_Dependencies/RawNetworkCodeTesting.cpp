#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "OhYeeGee";      // Your Wi-Fi SSID
const char* password = "123456789";          // Empty password for open network
const char* serverURL = "http://192.168.208.96:5050";  // Replace with your backend endpoint

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Send test POST request
  HTTPClient http;
  http.begin(serverURL);  // Endpoint
  http.addHeader("Content-Type", "application/json");  // Set content type

  String payload = "{\"test\":\"connection\"}";
  int httpCode = http.POST(payload);  // Send POST

  Serial.print("HTTP Response Code: ");
  Serial.println(httpCode);

  if (httpCode > 0) {
    String response = http.getString();
    Serial.print("Server Response: ");
    Serial.println(response);
  }

  http.end();
}

void loop() {
  // No loop logic – runs once for test
}
