#include <SoftwareSerial.h>
#include <DHT11.h>

#define DHTPIN 8
DHT11 dht(DHTPIN);

SoftwareSerial esp(2, 3); // RX, TX

// 🔹 WiFi
String ssid = "iPhone de Fernando";
String password = "1234567890";

// 🔹 ThingSpeak
String host = "api.thingspeak.com";
String apiKey = "SLL26N7I4UCMUB3A"; // 👈 pega aquí tu key

void setup() {
  Serial.begin(9600);
  esp.begin(9600);

  Serial.println("Iniciando ESP...");

  enviarAT("AT", 2000);
  enviarAT("AT+CWMODE=1", 2000);

  String cmd = "AT+CWJAP=\"" + ssid + "\",\"" + password + "\"";
  enviarAT(cmd, 8000);
}

void loop() {

  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  if (isnan(temp) || isnan(hum)) {
    Serial.println("Error leyendo DHT");
    delay(2000);
    return;
  }

  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.print("  Hum: ");
  Serial.println(hum);

  enviarDatos(temp, hum);

  delay(15000); // ⏱️ OBLIGATORIO para ThingSpeak
}

void enviarDatos(float temp, float hum) {

  String data = "GET /update?api_key=" + apiKey +
                "&field1=" + String(temp) +
                "&field2=" + String(hum) + " HTTP/1.1\r\n" +
                "Host: " + host + "\r\n" +
                "Connection: close\r\n\r\n";

  String cmd = "AT+CIPSTART=\"TCP\",\"" + host + "\",80";
  enviarAT(cmd, 4000);

  cmd = "AT+CIPSEND=" + String(data.length());
  enviarAT(cmd, 2000);

  esp.print(data);

  delay(3000);

  enviarAT("AT+CIPCLOSE", 2000);
}

void enviarAT(String cmd, int tiempo) {
  Serial.println(">> " + cmd);
  esp.println(cmd);

  long int time = millis();
  while ((time + tiempo) > millis()) {
    while (esp.available()) {
      Serial.write(esp.read());
    }
  }
}