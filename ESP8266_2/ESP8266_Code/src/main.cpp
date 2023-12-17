

//====================
// man hình LCD

// #include <LiquidCrystal_I2C.h>
// #include <Arduino.h>
// #include <Wire.h>
// #include <ESP8266_ISR_Servo.h>
// int lcdColum = 16;
// int lcdRow = 2;
// LiquidCrystal_I2C lcd(0x27, 16, 2);

// void setup()
// {
//   Serial.begin(115200);
//   lcd.init();                 //Bắt đầu màn hình
//   lcd.backlight();            // Bật đèn nền

// }
// void loop()
// {
//    lcd.setCursor(0,0);
//    lcd.print("Heloo,I'm Quoc Viet");
//   Serial.println("Helloooo");
//   delay(1000);
//   lcd.clear();

// }

// servo=======================

// #include <Servo.h>

// Servo servo;

// void setup() {

//     servo.attach(5); //D1
//     servo.write(0);

//     delay(2000);

// }

// void loop() {

//     servo.write(180);

//     delay(1000);

//     servo.write(0);

//     delay(1000);

// }

// đọc bàn phím
#include "ESP8266WiFi.h"
#include <Arduino.h>
#include <Keypad.h>
#include <Wire.h>
#include <ESP8266_ISR_Servo.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include "PubSubClient.h"
#include "WiFiClientSecure.h"

// ==============
const char *ssid = "thong";
const char *password = "1234567890";
const char *mqtt_server = "3f3c3cae23984b4cb5a8834ee9d5d0b6.s1.eu.hivemq.cloud";
const char *mqtt_username = "hello";
const char *mqtt_password = "Thong182003";
int mqttPort = 8883;

int lcdColum = 16;
int lcdRow = 2;
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo myServo;
const byte rows = 4;    // số hàng
const byte columns = 3; // số cột

int holdDelay = 700; // Thời gian trễ để xem là nhấn 1 nút nhằm tránh nhiễu
int n = 3;           //
int state = 0;       // nếu state =0 ko nhấn,state =1 nhấn thời gian nhỏ , state = 2 nhấn giữ lâu
char key = 0;
char STR[4] = {'9', '9', '9', '9'}; // Cài đặt mật khẩu tùy ý
char str[4] = {' ', ' ', ' ', ' '};
int i, j, count = 0;

// Định nghĩa các giá trị trả về
char keys[rows][columns] =
    {
        {'1', '2', '3'},
        {'4', '5', '6'},
        {'7', '8', '9'},
        {'*', '0', '#'}};

byte rowPins[rows] = {2, 14, 12, 13}; // nối chân esp8266
byte columnPins[columns] = {15, 3, 1};

// cài đặt thư viện keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, columnPins, rows, columns);

WiFiClientSecure espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
unsigned long previousMillis = 0;
const long interval = 50000;

#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;
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

int tinhieumocua = 0;
int tinhieudoimatkhau = 0;
int tinhieubaochay = 0;
int cuadangmo = 0;
void callback(char *topic, byte *payload, unsigned int length)
{
  if (strcmp(topic, "door-living-room") == 0) // Check topic "LedBedRoom"m
  {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++)
    {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    if ((char)payload[0] == '0')
    {
      tinhieumocua = 2;
    }
    else
    {
      tinhieumocua = 1;
    }
  }
  else if (strcmp(topic, "newpassword") == 0) // Check topic "LedBedRoom"m
  {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++)
    {
      STR[i] = (char)payload[i];
      Serial.print(STR[i]); // Print the updated character
    }
    tinhieudoimatkhau = 1;
  }
  else if (strcmp(topic, "led-kitchen") == 0) // Check topic "LedBedRoom"m
    {
      Serial.print("Message arrived [");
      Serial.print(topic);
      Serial.print("] ");
      for (int i = 0; i < length; i++)
      {
        Serial.print((char)payload[i]);
      }
      Serial.println();
      if ((char)payload[0] == '2')
      {
        tinhieubaochay =1 ;
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

      client.subscribe("door-living-room");
      client.subscribe("led-kitchen");
      client.subscribe("newpassword");
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
{
  Serial.begin(115200); // bật serial, baudrate 9600
  myServo.attach(0);    // Khai báo chân điều khiển động cơ
  myServo.write(0);
  lcd.init(); // Khai báo sử dụng LCD
  lcd.begin(16, 2);
  setup_wifi();
  espClient.setInsecure();
  client.setServer(mqtt_server, mqttPort);
  client.setCallback(callback);
  lcd.backlight();
  lcd.clear();
  lcd.print(" Enter Password");
}
void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
  char temp = keypad.getKey();

  if ((int)keypad.getState() == PRESSED)
  {
    if (temp != 0)
    {
      key = temp;
    }
  }
  if ((int)keypad.getState() == HOLD)
  {
    state++;
    state = constrain(state, 1, n - 1);
    delay(holdDelay);
  }

  if ((int)keypad.getState() == RELEASED)
  {
    key += state;
    state = 0;
    // Xuất lên Máy tính để xem kết quả
    // Nhập mật khẩu
    if (i == 0)
    {
      str[0] = key;
      lcd.setCursor(6, 1);
      lcd.print(str[0]);
      delay(500); // Ký tự hiển thị trên màn hình LCD trong 1s
      lcd.setCursor(6, 1);
      lcd.print("*"); // Ký tự được che bởi dấu *
    }
    if (i == 1)
    {
      str[1] = key;
      lcd.setCursor(7, 1);
      lcd.print(str[1]);
      delay(500);
      lcd.setCursor(7, 1);
      lcd.print("*");
    }
    if (i == 2)
    {
      str[2] = key;
      lcd.setCursor(8, 1);
      lcd.print(str[2]);
      delay(500);
      lcd.setCursor(8, 1);
      lcd.print("*");
    }
    if (i == 3)
    {
      str[3] = key;
      lcd.setCursor(9, 1);
      lcd.print(str[3]);
      delay(500);
      lcd.setCursor(9, 1);
      lcd.print("*");
      count = 1;
    }
    i = i + 1;
  }
  if (tinhieudoimatkhau == 1)
  {
    lcd.clear();
    lcd.print("Change Password Complete!");
    tinhieudoimatkhau = 0;
  }
  if (count == 1)
  {
    if (str[0] == STR[0] && str[1] == STR[1] && str[2] == STR[2] &&
        str[3] == STR[3])
    {
      lcd.clear();
      lcd.print("    Correct!");
      delay(1000);
      for (int i = 0; i <= 180; i++)
      {
        myServo.write(i); // Mở cửa
        delay(15);
        cuadangmo = 1;
      }
      lcd.clear();
      lcd.print("      Opened!");
      i = 0;
      count = 0;
    }
    else
    {
      lcd.clear();
      lcd.print("   Incorrect!");
      delay(1000);
      lcd.clear();
      lcd.print("   Try Again!");
      delay(1000);
      lcd.clear();
      lcd.print(" Enter Password");
      i = 0;
      count = 0;
    }
  }

  switch (key)
  {
  case '#':
    lcd.clear();
    for (int i = 180; i >= 0; i--)
    {
      myServo.write(i); // Đóng cửa
      delay(15);
      cuadangmo = 0;
    }
    lcd.print("     Closed!");
    delay(1000);
    lcd.clear();
    lcd.print(" Enter Password");
    i = -1;
    break;
  }
  delay(50);
  if (tinhieumocua == 2 && cuadangmo == 1)
  {
    for (int i = 180; i >= 0; i--)
    {
      myServo.write(i); // Đóng cửa
      delay(15);
      cuadangmo = 0;
    }
    tinhieumocua = 0;
  }

  if (tinhieumocua == 1 && cuadangmo == 0)
  {
    for (int i = 0; i <= 180; i++)
    {
      myServo.write(i); // Mở cửa
      delay(15);
      cuadangmo = 1;
    }
    tinhieumocua = 0;
  }
  if(tinhieubaochay == 1 && cuadangmo == 0)
  {
        for (int i = 0; i <= 180; i++)
    {
      myServo.write(i); // Mở cửa
      delay(15);
      cuadangmo = 1;
    }
    tinhieumocua = 0;
  }
}

// LCD nối chân D1 và D2
//  keypad nối D3 - Tx theo thứ tự
