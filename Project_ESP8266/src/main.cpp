#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Hash.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
#include "AccelStepper.h"
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>

// /*----------------------Define-------------------------------*/
// WiFiClientSecure wifiClient;

// PubSubClient mqttClient(wifiClient);

// #define DHTPIN 0
// #define DHTTYPE DHT11
// DHT dht(DHTPIN, DHTTYPE);

// const char* ssid = "thong";
// const char* pass = "1234567890";

// char* mqttServer = "3f3c3cae23984b4cb5a8834ee9d5d0b6.s1.eu.hivemq.cloud";
// const char* mqtt_username = "hello";
// const char* mqtt_password = "Thong182003";
// int mqttPort = 8883;

// unsigned long lastMsg = 0;
// #define MSG_BUFFER_SIZE (50)
// char msg[MSG_BUFFER_SIZE];

// static const char *root_ca PROGMEM = R"EOF(
// -----BEGIN CERTIFICATE-----
// MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw
// TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
// cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4
// WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu
// ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY
// MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc
// h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+
// 0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U
// A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW
// T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH
// B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC
// B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv
// KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn
// OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn
// jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw
// qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI
// rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV
// HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq
// hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL
// ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ
// 3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK
// NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5
// ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur
// TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC
// jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc
// oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq
// 4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA
// mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d
// emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=
// -----END CERTIFICATE-----
// )EOF";

// void setupWifi(){
//   delay(10);
//   Serial.print("\n Conetting to ");
//   Serial.println(ssid);
//   WiFi.mode(WIFI_STA);
//   WiFi.begin(ssid, pass);

//   while (WiFi.status() != WL_CONNECTED)
//   {
//     /* code */
//     delay(500);
//     Serial.print(".");
//   }
//   randomSeed(micros());
//   Serial.println("\n Wifi COnnected\n IP address: ");
//   Serial.println(WiFi.localIP());

// }
// void callback(char* topic, byte* payload, unsigned int length) {
//   String incommingMessage = "";
//   for (int i = 0; i < length; i++) incommingMessage+=(char)payload[i];

//   Serial.println("Message arrived ["+String(topic)+"]"+incommingMessage);

//   //--- check the incomming message
//     if( strcmp(topic,"led_state") == 0){
//      if (incommingMessage.equals("1")) Serial.print("hello");   // Turn the LED on
//      else Serial.print("Hi");  // Turn the LED off
//   }

// }
// void setupMQTT(){
//   mqttClient.setServer(mqttServer, mqttPort);
//   mqttClient.setCallback(callback);
// }

// void reconnect() {
//   // Loop until we're reconnected
//   while (!mqttClient.connected()) {
//     Serial.print("Attempting MQTT connection...");
//     String clientId = "ESP8266Client-";   // Create a random client ID
//     clientId += String(random(0xffff), HEX);
//     // Attempt to connect
//     if (mqttClient.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
//       Serial.println("connected");

//       mqttClient.subscribe("led_state");   // subscribe the topics here

//     } else {
//       Serial.print("failed, rc=");
//       Serial.print(mqttClient.state());
//       Serial.println(" try again in 5 seconds");   // Wait 5 seconds before retrying
//       delay(5000);
//     }
//   }
// }

// void setup(){
//   Serial.begin(115200);
//   WiFi.begin(ssid,pass);
//   setupWifi();
//   wifiClient.setInsecure();
//   dht.begin();
//   setupMQTT();
// }

// void loop(){
//   if(!mqttClient.connected())
//     reconnect();
//   mqttClient.loop();
// }

#define DHTPIN1 13 // Bed Room
#define DHTPIN2 4  // Kitchen
#define LED_BED_ROOM 2
#define LED_KITCHEN 15
#define LED_LIVING_ROOM 0
#define IN1 5
#define IN2 4
#define IN3 14
#define IN4 12

#define DHTTYPE DHT11
#define API_KEY "AIzaSyC78ir3QHABZFjyLI7VWnWGbGzJH12VMlg"
#define DATABSE_URL "https://tkllhk231-default-rtdb.europe-west1.firebasedatabase.app/"
/*------------------------------------------*/
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

bool signUp = false;
int tinhieubaochay = 0;
int trangthaidenbaochay = 0;
int mqttsignal = 0;
bool window_close = true;
DHT dht2(DHTPIN2, DHTTYPE);
DHT dht1(DHTPIN1, DHTTYPE);

const int stepsPerRevolution = 2048;

AccelStepper stepper(AccelStepper::HALF4WIRE, IN1, IN2, IN3, IN4);

const char *ssid = "thong";
const char *password = "1234567890";
const char *mqtt_server = "3f3c3cae23984b4cb5a8834ee9d5d0b6.s1.eu.hivemq.cloud";
const char *mqtt_username = "thong2";
const char *mqtt_password = "Thong182003";
int mqttPort = 8883;

/* ----------------------------------------- Struct -------------------------------- */
class Kitchen
{
public:
  float ReadValueTemperature();
  float ReadValueHumidity();
};

float Kitchen::ReadValueTemperature()
{
  float newT = dht1.readTemperature();
  if (isnan(newT))
  {
    return 0;
  }
  else
  {
    return newT;
  }
}

float Kitchen::ReadValueHumidity()
{
  float newH = dht1.readHumidity();
  if (isnan(newH))
  {
    return 0;
  }
  else
  {
    return newH;
  }
}

class BedRoom
{
public:
  float ReadValueTemperature();
  float ReadValueHumidity();
};

float BedRoom::ReadValueTemperature()
{
  float newT = dht2.readTemperature();
  if (isnan(newT))
  {
    return 0;
  }
  else
  {
    return newT;
  }
}

float BedRoom::ReadValueHumidity()
{
  float newH = dht2.readHumidity();
  if (isnan(newH))
  {
    return 0;
  }
  else
  {
    return newH;
  }
}

WiFiClientSecure espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
unsigned long previousMillis = 0;
const long interval = 50000;

#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setUpFirebase()
{
  config.api_key = API_KEY;
  config.database_url = DATABSE_URL;
  /*Sign Up*/
  if (Firebase.signUp(&config, &auth, "", ""))
  {
    Serial.println("Sign up Firebase OK !!!");
    signUp = true;
  }
  else
  {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }
  /*-- Call Back --*/
  config.token_status_callback = tokenStatusCallback;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void setup_wifi()
{

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char *topic, byte *payload, unsigned int length)
{
  if (strcmp(topic, "led-bed-room") == 0) // Check topic "LedBedRoom"m
  {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++)
    {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    if ((char)payload[0] == '1')
    {
      digitalWrite(LED_BED_ROOM, HIGH);
      Serial.println("LED BEDROOM ON");
    }
    else
    {
      digitalWrite(LED_BED_ROOM, LOW);
      Serial.println("LED BEDROOM OFF");
    }
  }
  else if (strcmp(topic, "led-kitchen") == 0) // Check topic "led-kitchen"
  {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++)
    {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    if ((char)payload[0] == '1')
    {
      digitalWrite(LED_KITCHEN, HIGH);
      tinhieubaochay = 0;
      Serial.println("LED KITCHEN ON");
    }
    else if ((char)payload[0] == '0')
    {
      digitalWrite(LED_KITCHEN, LOW);
      tinhieubaochay = 0;
      Serial.println("LED KTCHEN OFF");
    }
    else if ((char)payload[0] == '2')
    {
      tinhieubaochay = 1;
      Serial.println("Báo cháy");
    }
  }
  else if (strcmp(topic, "window-bed-room") == 0)
  {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++)
    {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    if ((char)payload[0] == '1')
    {
      if (window_close == true)
      {
        mqttsignal = 1;
        Serial.println("Window open");
      }
    }
    else
    {
      if (window_close == false)
      {
        mqttsignal = 2;
        Serial.println("Window CLose");
      }
    }
  }
  else if (strcmp(topic, "led-living-room") == 0) // Check topic "fan-bed-room"
  {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++)
    {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    if ((char)payload[0] == '1')
    {
      digitalWrite(LED_LIVING_ROOM, HIGH);
      Serial.println("Led_living_room open");
    }
    else
    {
      digitalWrite(LED_LIVING_ROOM, LOW);
      Serial.println("Led_living_room close");
    }
  }
}

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password))
    {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("temperature bedroom", "temp");
      client.publish("humidity bedroom", "humdity");
      client.publish("temperature kitchen", "temp");
      client.publish("humidity kitchen", "humdity");

      // ... and resubscribe
      client.subscribe("led-bed-room");
      client.subscribe("led-living-room");
      client.subscribe("door-living-room");
      client.subscribe("led-kitchen");
      client.subscribe("fan-bed-room");
      client.subscribe("window-bed-room");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{ // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  pinMode(LED_BED_ROOM, OUTPUT);
  pinMode(LED_KITCHEN, OUTPUT);
  pinMode(LED_LIVING_ROOM, OUTPUT);
  digitalWrite(LED_LIVING_ROOM, LOW);
  digitalWrite(LED_BED_ROOM, LOW);
  digitalWrite(LED_KITCHEN, LOW);
  dht1.begin();
  dht2.begin();
  setup_wifi();
  espClient.setInsecure();
  setUpFirebase();
  stepper.setMaxSpeed(500);
  stepper.setAcceleration(100);
  stepper.moveTo(24);
  client.setServer(mqtt_server, mqttPort);
  client.setCallback(callback);
}
void loop()
{

  if (!client.connected())
  {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (tinhieubaochay == 1)
  {
    // Đèn 2 thay đổi trạng thái 1 lần mỗi giây
    if (now - previousMillis >= 1000)
    {
      previousMillis = now;
      // Thực hiện thay đổi trạng thái đèn 2 ở đây
      // Ví dụ: nếu trạng thái hiện tại là HIGH, chuyển nó thành LOW và ngược lại
      if (trangthaidenbaochay == 0)
      {
        digitalWrite(LED_KITCHEN, LOW);
        trangthaidenbaochay = 1;
        Serial.println("Báo cháy bật");
      }
      else
      {
        digitalWrite(LED_KITCHEN, HIGH);
        Serial.println("Báo cháy tắt");
        trangthaidenbaochay = 0;
      }
    }
  }
  if (Firebase.ready() && signUp && (now - lastMsg >= interval))
  {

    lastMsg = now;
    Kitchen kitchen;
    BedRoom bedroom;

    float TemperatureKitchen = kitchen.ReadValueTemperature();
    Serial.print("Temperature Kitchen in loop() : ");
    Serial.println(TemperatureKitchen);
    if (Firebase.RTDB.setFloat(&fbdo, "kitchen/temparature", TemperatureKitchen))
    {
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else
    {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    sprintf(msg, "%g", TemperatureKitchen);
    Serial.print("Publish message: ");
    Serial.print(msg);
    Serial.println("to topic tempearature kitchen");
    client.publish("temperature kitchen", msg);

    float HumidityKitchen = kitchen.ReadValueHumidity();
    Serial.print("Humidity Kitchen in loop() : ");
    Serial.println(HumidityKitchen);
    if (Firebase.RTDB.setFloat(&fbdo, "kitchen/HumidityKitchen", HumidityKitchen))
    {
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else
    {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    sprintf(msg, "%g", HumidityKitchen);
    Serial.print("Publish message: ");
    Serial.print(msg);
    Serial.println("to topic humidity kitchen");
    client.publish("humidity kitchen", msg);
  }
  if (mqttsignal == 1)
  {
    stepper.moveTo(5 * stepsPerRevolution);
    if (stepper.distanceToGo() == 0)
    {
      window_close = false;
      mqttsignal = 0;
    }
  }
  else if (mqttsignal == 2)
  {
    stepper.moveTo(- (5 * stepsPerRevolution));
    if (stepper.distanceToGo() == 0)
    {
      window_close = true;
      mqttsignal = 0;
    }
  }

  stepper.run();
}