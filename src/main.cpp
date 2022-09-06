#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>

#include <WiFiManager.h>
#include <Firebase_ESP_Client.h>
#include <NTPClient.h>
#include <DHTesp.h>

#include <addons/TokenHelper.h>

#define DHTPIN  D0
#define IN_1A   D1
#define IN_1B   D2

#define API_KEY "AIzaSyBY0_UuT0FzeKxvyO-pO-9cj9y9gRT3qEY"
// #define USER_EMAIL "admin@admin.com"
// #define USER_PASSWORD "admin123"
#define FIREBASE_PROJECT_ID "espcompose"
#define FIREBASE_CLIENT_EMAIL "firebase-adminsdk-mo7wj@espcompose.iam.gserviceaccount.com"
const char PRIVATE_KEY[] PROGMEM ="-----BEGIN PRIVATE KEY-----\nMIIEvAIBADANBgkqhkiG9w0BAQEFAASCBKYwggSiAgEAAoIBAQCpqoEbgGvbyjYQ\nUNPXzJ99SXqmcuPQ/8SZefi9eagfLc6sWW66cQbJE2ah1I19HrYrab+p5ST4GU83\nmBhJlrz8ezIuyjfTqoaFX5IPS5TgCXjd1DHYWeE1XmRWbdhoaNmKmG0akD1OIFjc\n5vYMobXiP14VuOPJXwzAF40mpoma7nuY/yuzFLAeJO35FOUQlCDpubLH6SW+ZJD3\nYVjg0ZaR750KjXCbatxI072nqs0GBW8yEEGOsdtyGwoW3x9tWUzsFqK85Zl3tEM/\nrt+WFiQdkcxgLNib124/d5wgrfkZohE1BCEdVBfypZlTDtpz8aTHWgqTc7DlgybV\nslR+CjQnAgMBAAECggEAAUPAl5xi8W317jQ0CAlB7RSloeT8Vrv5Uq2KkZr9ClJ0\nlmtKPICILOqEmtMzRMn8UKGGqLUJDAJgSUlMcQv7F6SxcJDYTq8YNplKROvqzaq0\naG7i+jpdR+EOvWgvM0QIzxQoyAhoNJNgJjFghyKTcP7I7D+HDyKyvSHPzwhxwSJf\n0MWaO90UXiHbUS63GkzSZdyMq45iWyX1+2m2rW4Jp9HyzZv64xIPy7HwradXAb7K\n7iSiybxvZOSfYPYReszKRfCHn5/os74iD6i+G0FCdbIAM9yIKj+9YCnkUoysgSG2\nw2xbxlzMgqWZNUC5Vk6tmwQ/0ZxUHtoeRR9tKkeDaQKBgQDnUJXhcJG3nBq2PlG7\nmuo42Y+oU9jTFSDHCNkmESQLpSdxZeEvgtPqfiIkF+jRuAh6D6saZXkyu8LbYYG7\nsHmYocs8sx1SzOQnGQ6+qVPtyt80XgJQWzmv6CWE8q9RbZA9XJyfvK6OCGbQAH/O\n8om2pb+98fEqMlgxqv3h65iKPwKBgQC7xbP29ABcOqaskqfC/8qOWpt7VNqnR1zg\nAhTcAVmmQA1H6W/9d3hbhBXCpotlYXuvux86WybF4yRrt30gNDdx4In9aJ5xTE7F\nh2qJNnVTbn498YkcJnai4QGn2VyxuycbToOzTw1eL1z+2pYSkFVfOYXykS/d3C5M\n0cZgYmZMGQKBgGHXk15FaE2a6S2XFKlfMJ2VYQ+ZT8iRUkj+NCPG8lE+rX1o06Ru\nGUdBzR0pQ+CZkHRY9k33CBnuk9CJ4P9L+VMXdMkbFlc0tXjYP4VkSEz4GAuljCMD\nQ1ZlwqEGqiOuTWm3J8TlYYxQf3SSEX2u15N7bqxgj1sC6o0vv631IpOvAoGAJ+E6\ncqWp+eye/6H6LWkkPQ2zXgNMmSCH8i+e/Ibw4mVSLMbAeUXJcy5fbMLwciy9FzqX\nzE4Vswul7Za/5XoRmIcG56IznH6HcF5tWw0M6BZci41kAG2O3tMJBfwdOsYz7RHE\n2O1eOnW2u43zN6gd6aaO81CwqN8dmkUna8saTOkCgYBsXiv5R2GXNz3GM0W3BqvK\nHX2iTmuHPBk7d1Y870KX8U2srA2aYJcm+ubRUCWkwyM+Nvkcdmcwotiyl66pF4rw\nc3/RCF6LYd2Y/BuaSNQ+DMnRTlXB7Z6ehrXd2iJlCpJgUjNjX4uoBlYfdlHa1ucc\ncvLSpHvB5ugIDvJBfiL4LA==\n-----END PRIVATE KEY-----\n";

#define DEVICE_REGISTRATION_ID_TOKEN "ffel-OdOSyq2QjjHyGJaB3:APA91bFnqZGvC_sIsZ19Yvbgn4W8SKCkrc-MEED9ahAQQr9vjaCtgUu64QmIOTziFnIaw4pp54nVIZrn8a5wVmtMwsNxZ9_qKXKkw0a_qwvcVh8ykav6EeNqD-10JpV17RmaEzeyzVQY"
bool _reconnect_flag = true;
bool _connect_flag = true;
bool _temp_flag = true;
bool _motor_flag = true;
bool _pupuk_flag = true;
unsigned long lastTime = 0;
unsigned long lastTime1 = 0;
unsigned long lastTime2 = 0;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
String months[12]={"Januari", "Februari", "Maret", "April", "Mei", "Juni", "Juli", "Agustus", "September", "Oktober", "November", "Desember"};

DHTesp dht;

//Firebase constructor
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

void sendMessage();
void sendmotorData(String status);
void sendtempData(double value);
void updatemotorData(String status);
void updatetempData(double value);
void sendpupukData();
void spinMotor();
void stopMotor();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  WiFiManager wm;
  WiFi.mode(WIFI_STA);
  wm.autoConnect("ESP8266", "12345678");
  Serial.println("Connected");
  timeClient.begin();
  timeClient.setTimeOffset(7*3600);
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the api key (required) */
  config.api_key = API_KEY;
  config.service_account.data.client_email = FIREBASE_CLIENT_EMAIL;
  config.service_account.data.project_id = FIREBASE_PROJECT_ID;
  config.service_account.data.private_key = PRIVATE_KEY;
  config.token_status_callback = tokenStatusCallback;

  dht.setup(DHTPIN, DHTesp::DHT22);
  pinMode(IN_1A, OUTPUT);
  pinMode(IN_1B, OUTPUT);
  stopMotor();
  fbdo.setBSSLBufferSize(1024 /* Rx buffer size in bytes from 512 - 16384 */, 4096 /* Tx buffer size in bytes from 512 - 16384 */);
  fbdo.setResponseSize(2048);
  /* Assign the user sign in credentials */
  // auth.user.email = USER_EMAIL;
  // auth.user.password = USER_PASSWORD;
  Firebase.begin(&config, &auth);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(WiFi.status() == WL_CONNECTED){
    if(_connect_flag){
      _connect_flag = false;
      _reconnect_flag = true;
      Serial.println("Reconnected");
      while(!Firebase.ready()) {
        delay(1000);
      }
        // sendMessage();
        // sendpupukData();
        // Serial.println("Sending Notification...");
        // sendMessage();
        if(dht.getTemperature() <= 35.0){
          _motor_flag = true;
          updatemotorData("ON");
          spinMotor();
          Serial.println("Motor On");
        }
        else{
          _motor_flag = false;
          updatemotorData("OFF");
          stopMotor();
          Serial.println("Motor Off");
        }
        FirebaseJson payload;
        timeClient.update();
        // all data key-values should be string
        String documentPath = "Sensor/Realtime";
        FirebaseJson content;
        // content.set("fields/notif/booleanValue", false);
        content.set("fields/pupuk/stringValue", "Pupuk Diproses");
        if(!Firebase.Firestore.patchDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), content.raw(),"pupuk")){
          Serial.println(fbdo.errorReason());
        }
        if(!Firebase.Firestore.createDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), content.raw())){
          // Serial.println("Data Not Sended");
          Serial.println(fbdo.errorReason());
  }
    }
    
    timeClient.update();
    int minute = timeClient.getMinutes();
    if(minute == 0 && _temp_flag){
      double temp = dht.getTemperature();
      sendtempData(temp);
      _temp_flag = false;
    }
    if(minute != 0) _temp_flag = true;

    if(millis() - lastTime > 30 * 1000 || lastTime == 0){
      lastTime = millis();
      double temp = dht.getTemperature();
      updatetempData(temp);
    }
    
    if(millis() - lastTime1 > dht.getMinimumSamplingPeriod()){
      lastTime1 = millis();
      float _temp = dht.getTemperature();
      if(_temp <= 35.0 && _motor_flag == true){
        _motor_flag = false;
        spinMotor();
        updatemotorData("ON");
        sendmotorData("ON");
      }
      if(_temp >= 35.5 && _motor_flag == false){
        _motor_flag = true;
        stopMotor();
        updatemotorData("OFF");
        sendmotorData("OFF");
      }
    }

    if(millis() - lastTime2 > 600*1000 && _pupuk_flag == true){
      _pupuk_flag = false;
      lastTime2 = millis();
      sendMessage();
      sendpupukData();
    }

  }
  else{
    if(_reconnect_flag){
      _reconnect_flag = false;
      _connect_flag = true;
      Serial.println("Trying to reconnect...");
    }
    else{
      Serial.println(".");
    }
  }
}

void sendmotorData(String status){
  while(!Firebase.ready()){
    delay(500);
  }
  Serial.println("Firebase Ready!!!");
  timeClient.update();
  unsigned long epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime ((time_t *)&epochTime);
  int monthDay = ptm->tm_mday;
  String monthName = months[ptm->tm_mon];
  int year = ptm->tm_year + 1900;
  String date = String(timeClient.getHours()) + ":" + String(timeClient.getMinutes()) + "  " + String(monthDay) + " " + monthName + " " + String(year);
  String documentPath = "History_Motor/" + String(epochTime);
  FirebaseJson content;
  content.set("fields/date/stringValue", date);
  content.set("fields/motor/stringValue", status);
  if(!Firebase.Firestore.createDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), content.raw())){
    // Serial.println("Data Not Sended");
    Serial.println(fbdo.errorReason());
  }
  Serial.println("Data Sended, Motor : " + status);
}

void sendtempData(double value){
  while(!Firebase.ready()){
    delay(500);
  }
  Serial.println("Firebase Ready!!!");
  timeClient.update();
  time_t epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime ((time_t *)&epochTime);
  int monthDay = ptm->tm_mday;
  String monthName = months[ptm->tm_mon];
  int year = ptm->tm_year + 1900;
  String date = String(timeClient.getHours()) + ":" + String(timeClient.getMinutes()) + "  " + String(monthDay) + " " + monthName + " " + String(year);
  String documentPath = "History_Temp/" + String(epochTime);
  FirebaseJson content;
  content.set("fields/date/stringValue", date);
  content.set("fields/temp/doubleValue", value);
  if(!Firebase.Firestore.createDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), content.raw())){
    // Serial.println("Data Not Sended");
    Serial.println(fbdo.errorReason());
  }
  Serial.println("Data Sended, Temperature : " + String(value) );
}

void updatemotorData(String status){
  while(!Firebase.ready()){
    delay(500);
  }
  Serial.println("Firebase Ready!!!");
  String documentPath = "Sensor/Realtime";
  FirebaseJson content;
  content.set("fields/motor/stringValue", status);
  if(!Firebase.Firestore.patchDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), content.raw(),"motor")){
    Serial.println(fbdo.errorReason());
  }
  if(!Firebase.Firestore.createDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), content.raw())){
    // Serial.println("Data Not Sended");
    Serial.println(fbdo.errorReason());
  }
  Serial.println("Data Updated, Motor : " + status);
}

void updatetempData(double value){
  while(!Firebase.ready()){
    delay(500);
  }
  Serial.println("Firebase Ready!!!");
  String documentPath = "Sensor/Realtime";
  FirebaseJson content;
  content.set("fields/temp/doubleValue", value);
  if(!Firebase.Firestore.patchDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), content.raw(),"temp")){
    Serial.println(fbdo.errorReason());
  }
  if(!Firebase.Firestore.createDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), content.raw())){
    // Serial.println("Data Not Sended");
    Serial.println(fbdo.errorReason());
  }
  Serial.println("Data Updated, Temp : " + String(value));
}

void sendpupukData(){
  while(!Firebase.ready()){
    delay(500);
  }
  delay(1000);
  Serial.println("Firebase Ready!!!");
  timeClient.update();
  time_t epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime ((time_t *)&epochTime);
  int monthDay = ptm->tm_mday;
  String monthName = months[ptm->tm_mon];
  int year = ptm->tm_year + 1900;
  String date = String(timeClient.getHours()) + ":" + String(timeClient.getMinutes()) + "  " + String(monthDay) + " " + monthName + " " + String(year);
  String documentPath = "History_Pupuk/" + String(epochTime);
  FirebaseJson content;
  content.set("fields/date/stringValue", date);
  // content.set("fields/temp/doubleValue", value);
  if(!Firebase.Firestore.createDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), content.raw())){
    // Serial.println("Data Not Sended");
    Serial.println(fbdo.errorReason());
  }
  Serial.println("Data Sended, Pupuk Jadi Pada : " + date );
}

void sendMessage()
{
  // sendpupukData();
  while(!Firebase.ready()){
    delay(500);
  }
  String documentPath = "Sensor/Realtime";
  String mask = "motor";

  // If the document path contains space e.g. "a b c/d e f"
  // It should encode the space as %20 then the path will be "a%20b%20c/d%20e%20f"

  Serial.print("Get a document... ");

  if (Firebase.Firestore.getDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), mask.c_str()))
      Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
  else
      Serial.println(fbdo.errorReason());

  Serial.print("Send Firebase Cloud Messaging... ");

  // Read more details about HTTP v1 API here https://firebase.google.com/docs/reference/fcm/rest/v1/projects.messages
  while(!Firebase.ready()){
    delay(500);
  }
  FCM_HTTPv1_JSON_Message msg;

  // msg.token = DEVICE_REGISTRATION_ID_TOKEN;
  msg.topic = "pupuk";
  msg.notification.body = "Pupuk Kompos Sudah Jadi, Silahkan Untuk Diambil";
  msg.notification.title = "Pupuk Kompos Jadi";

  // For the usage of FirebaseJson, see examples/FirebaseJson/BasicUsage/Create.ino
  FirebaseJson payload;
  timeClient.update();
  // all data key-values should be string
  payload.add("timestamp", String(timeClient.getEpochTime()));
  msg.data = payload.raw();
  // String documentPath = "Sensor/Realtime";
  FirebaseJson content;
  content.set("fields/notif/booleanValue", true);
  content.set("fields/pupuk/stringValue", "Pupuk Jadi");
  if(!Firebase.Firestore.patchDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), content.raw(),"notif,pupuk")){
    Serial.println(fbdo.errorReason());
  }
  if(!Firebase.Firestore.createDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), content.raw())){
    // Serial.println("Data Not Sended");
    Serial.println(fbdo.errorReason());
  }

  if (Firebase.FCM.send(&fbdo, &msg)) // send message to recipient
      Serial.printf("ok\n%s\n\n", Firebase.FCM.payload(&fbdo).c_str());
  else
      Serial.println(fbdo.errorReason());
}

void spinMotor(){
  digitalWrite(IN_1A, LOW);
  digitalWrite(IN_1B, LOW);
}

void stopMotor(){
  digitalWrite(IN_1A, HIGH);
  digitalWrite(IN_1B, HIGH);
}