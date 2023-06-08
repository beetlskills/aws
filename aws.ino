#include <WiFi.h>
#include <AWS_IOT.h> // Located in the Arduino Library folder. Contains device certificate, device private key and root CA (AWS IoT)
#include <ArduinoJson.h>

#define FUNC 15 // side tactile button
#define USR0 21 // red LED
#define USR1 22 // white LED

volatile bool bPressed = false;
int pressCount = 0;

// Configure the name and password of the connected wifi and your MQTT Serve host.
const char* ssid = "SSC";
const char* password = "abcd1234z";
// AWS MQTT Details
AWS_IOT aws_iot;
const String thing_id = "microbot"; //YourThingID
char *aws_mqtt_server = "a3lnqpq9rex88v-ats.iot.ap-southeast-2.amazonaws.com"; //"YourAWSThingID.iot.ap-southeast-2.amazonaws.com"
char *aws_mqtt_client_id = "microbot"; // YourMQTTClientID
char *aws_mqtt_thing_topic_pub = "espeye/count"; //Your/MQTT/Topic

void setup() {
  Serial.begin(115200); // monitor (for debugging)

  WiFi.mode(WIFI_STA);  //Set the mode to WiFi station mode.
  WiFi.begin(ssid, password); //Start Wifi connection.
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Wi-Fi connected");

  while (aws_iot.connect(aws_mqtt_server, aws_mqtt_client_id)) // 1
  {
    Serial.printf("*");
    delay(500);
  }
  Serial.println("AWS connected");  
  delay (2000);
  aws_iot.subscribe(aws_mqtt_thing_topic_pub, pub_cb); // loop-back for debugging

  pinMode(USR0, OUTPUT);
  pinMode(USR1, OUTPUT);
  pinMode(FUNC, INPUT_PULLUP); // must be pulled-up
  attachInterrupt(digitalPinToInterrupt(FUNC), FUNC_ISR, RISING); // detect the release of button  
}

void loop() 
{ 
  // white led, when side tactile switch is pressed
  if (bPressed) {
    pressCount++;
    Serial.println(pressCount);
    bPressed = false;  

    // publish MQTT message
    // https://arduinojson.org/v6/assistant
    StaticJsonDocument<32> doc;
    String json;

    doc["count"] = pressCount; 
    serializeJson(doc, json); // serializes a JsonDocument to create a minified JSON document

    char payload[32]; // a C array
    json.toCharArray(payload, 32); // convert Json to C array

    if (aws_iot.publish(aws_mqtt_thing_topic_pub, payload) == 0) {
      Serial.println("Msg published");
      Serial.println(payload);
    }
    else {
      Serial.println("failed to publish");
    }   
  }
  delay(1000);
}

/* interrupt service routine */
void FUNC_ISR() {
  Serial.println("Side tactile button press detected");
  bPressed = true;
}

/* callback for MQTT subscription */
void pub_cb(char *topic, int val, char *payload) {
  Serial.println("Received subscribed message");
  Serial.println(topic);
  
  // blink white LED (for debugging purpose)
  digitalWrite(USR1, HIGH);
  delay(500);
  digitalWrite(USR1, LOW);
  delay(500);
}