#include "DHT.h"
#include "SPI.h"
#include "Ethernet.h"

#define DHTPIN A0 //Pino do DHT
#define DHTTYOE DHT11 //Modelo DHT

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192,168,1,110);//Define o endereco IP

EthernetServer server(80);
DHT dht(DHTPIN, DHTTYOE);

void setup()
{
 Ethernet.begin(mac, ip);
 server.begin();
 dht.begin();
}

void loop()
{
 float temp = dht.readTemperature();//Temperatura
 float umid = dht.readHumidity();//Umidade
 
 EthernetClient client = server.available();
 if(client){
  boolean currentLineIsBlank = true;
  while (client.connected()) {
   if (client.available()) {
    char c = client.read();
    
    if (c == '\n' && currentLineIsBlank) {
     
     // send a standard http response header
     client.println("HTTP/1.1 200 OK");
     client.println("Content-Type: application/json");
     client.println();
     client.print("{");
     client.print("\"temp\":");
     client.print(temp);
     client.print(",");
     client.print("\"umid\":");
     client.print(umid);
     client.println("}");
     break;
    }
    if (c == '\n') {
     // you're starting a new line
     currentLineIsBlank = true;
    } 
    else if (c != '\r') {
     // you've gotten a character on the current line
     currentLineIsBlank = false;
    }
   }
  }
  // give the web browser time to receive the data
  delay(1);
  // close the connection:
  client.stop();
 }
} 