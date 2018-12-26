#include <ESP8266WiFi.h>
#include "ESPMailer.h"

const char* ssid = "ssid";
const char* key = "key";

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
      
  ESPMailer mail;
  mail.Host = "192.168.2.101";
  mail.Subject = "Test";
  mail.Body = "Hello there!";
  mail.setFrom("mail@example.org", "ESPMailer test");
  mail.addToAddress("test@example.org");
  mail.setDebugLevel(CLIENT_AND_SERVER);

  if (mail.send())
    Serial.println("Mail sent sucessfully!");
  else
    Serial.println("Failed to send mail!");
}