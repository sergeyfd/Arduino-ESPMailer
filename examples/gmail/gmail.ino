#include <ESP8266WiFi.h>
#include "ESPMailer.h"

const char* ssid = "ssid";
const char* key = "password";

void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println();

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, key);
  while (WiFi.status() != WL_CONNECTED) {
      Serial.write('.');
      delay(200);
  } 
  Serial.println("Connected to WiFi");
      
  ESPMailerSecure mail;
  mail.setFingerprint(GMAIL_FINGERPRINT);
  mail.Username = "gmail_user";
  mail.Password = "password";  
  mail.Host = "smtp.gmail.com";
  mail.Port = 465;
  mail.Subject = "Test";
  mail.Body = "Hello there!";
  mail.setFrom("test@gmail.com");
  mail.addToAddress("user@gmail.com");
  mail.setDebugLevel(CLIENT_AND_SERVER);

  if (mail.send())
    Serial.println("Mail sent sucessfully!");
  else
    Serial.println("Failed to send mail!");
}

void loop() {
  // put your main code here, to run repeatedly:

}