#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "";      // Your Wi-Fi SSID
const char* password = "";          // Empty password for open network
const char* serverURL = "";  // Replace with your backend endpoint
const int PIR_SENSOR_OUTPUT_PIN = 13; /* PIR sensor O/P pin */
const int ALARM = 12;
int warm_up;
const int LED = 2;



void setup() {
  //set up pinModes for peripherals
  pinMode(PIR_SENSOR_OUTPUT_PIN, INPUT); 
  pinMode(ALARM,OUTPUT);
  pinMode(LED, OUTPUT);

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

  int server_Response_Code = post_Http("{\"test\":\"connection\"}");

  Serial.println("Response Code: " + server_Response_Code);
  Serial.println("Waiting For Power On Warm Up");
  delay(20000); /* Power On Warm Up Delay */

  alarm_Armed();

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
      alarm_Armed();
    }
    Serial.print("No object in sight\n\n");
    delay(1000);
  } else {
    Serial.print("Object detected\n\n");
    digitalWrite(LED, HIGH);
    warm_up = 1;
    post_Http("{\"ALARM\":\"connection\"}");
  }
}

int post_Http(String payload){
  HTTPClient http;  // <- Correct class
  http.begin(serverURL);
  http.addHeader("Content-Type", "application/json");

  int httpCode = http.POST(payload);

  Serial.print("HTTP Response Code: ");
  Serial.println(httpCode);

  if (httpCode > 0) {
    String response = http.getString();
    Serial.print("Server Response: ");
    Serial.println(response);
  }

  http.end();
  return httpCode;
}

void alarm_Armed(){
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
}
