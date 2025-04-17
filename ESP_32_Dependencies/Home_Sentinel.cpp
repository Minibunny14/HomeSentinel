#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "OhYeeGee";      // Your Wi-Fi SSID
const char* password = "123456789";          // Empty password for open network
const char* serverURL = "http://192.168.208.96:5050";  // Replace with your backend endpoint
const int PIR_SENSOR_OUTPUT_PIN = 13; /* PIR sensor O/P pin */
const int ALARM = 12;
int warm_up;
const int LED = 2;



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

  pinMode(PIR_SENSOR_OUTPUT_PIN, INPUT);
  pinMode(ALARM,OUTPUT);
  pinMode(LED, OUTPUT);
  Serial.println("Waiting For Power On Warm Up");
  delay(20000); /* Power On Warm Up Delay */
  digitalWrite(LED, LOW);
  noTone(ALARM);

 for (int i = 0; i < 3; i++) {
  tone(ALARM, 1000);
  digitalWrite(LED, HIGH);
  delay(150);
  noTone(ALARM);
  digitalWrite(LED, LOW);
  delay(150);
  digitalWrite(LED, HIGH);
  delay(150);
  noTone(ALARM);
  digitalWrite(LED, LOW);
  delay(150);
}
  Serial.println("Ready!");
}


void loop() {
  int sensor_output;
  sensor_output = digitalRead(PIR_SENSOR_OUTPUT_PIN);
  if (sensor_output == LOW) {
    if (warm_up == 1) {
      Serial.print("Warming Up\n\n");
      warm_up = 0;
      digitalWrite(LED, LOW);
      delay(20000);
      for (int i = 0; i < 3; i++) {
      tone(ALARM, 1000);
      digitalWrite(LED, HIGH);
      delay(150);
      noTone(ALARM);
      digitalWrite(LED, LOW);
      delay(150);
      digitalWrite(LED, HIGH);
      delay(150);
      noTone(ALARM);
      digitalWrite(LED, LOW);
      delay(150);
}
    }
    Serial.print("No object in sight\n\n");
    delay(1000);
  } else {
    Serial.print("Object detected\n\n");
    digitalWrite(LED, HIGH);
    warm_up = 1;
    HTTPClient http;
    http.begin(serverURL);  // Endpoint
    http.addHeader("Content-Type", "application/json");  // Set content type

    String payload = "{\"ALERT\":\"connection\"}";
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
}
