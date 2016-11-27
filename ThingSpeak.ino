#include <DHT.h>
#include <ESP8266WiFi.h>
 

/*-----------------ESP8266 Configuración WiFi---------------*/
String apiKey = "6SZZRSYG7LGA1L85";                   // Sustituir con tu API del canal de thingspeak
const char* ssid = "Punto acceso";                    //Poner el SSID de la WiFi
const char* password = "436a6a2a734166663146396642";  //Sustituir por el password de la WiFi
const char* server = "api.thingspeak.com";

/*------------------------DHT SENSOR------------------------*/
#define DHTPIN D4           //Pin al que se encuentra conectado el pin de datos del DHT
#define DHTTYPE DHT22       //Si usamos el DHT11 lo cambiaremos aquí
DHT dht(DHTPIN, DHTTYPE);   //Inicializa el sensor DHT
/*----------------------------------------------------------*/

//En el setup vamos a conectar a la WiFi

WiFiClient client;
 
void setup() 
{
Serial.begin(115200);
delay(10);
dht.begin();
 
WiFi.begin(ssid, password);
 
Serial.println();
Serial.println();
Serial.print("Connecting to ");
Serial.println(ssid);
 
WiFi.begin(ssid, password);
 
while (WiFi.status() != WL_CONNECTED) 
{
delay(500);
Serial.print(".");
}
Serial.println("");
Serial.println("WiFi connected");
 
}
 
void loop() 
{
 
float h = dht.readHumidity();
float t = dht.readTemperature();
if (isnan(h) || isnan(t)) 
{
Serial.println("Failed to read from DHT sensor!");
return;
}
 
if (client.connect(server,80)) {
String postStr = apiKey;
postStr +="&field1=";
postStr += String(t);
postStr +="&field2=";
postStr += String(h);
postStr += "\r\n\r\n";
 
client.print("POST /update HTTP/1.1\n");
client.print("Host: api.thingspeak.com\n"); 
client.print("Connection: close\n");
client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
client.print("Content-Type: application/x-www-form-urlencoded\n");
client.print("Content-Length: ");
client.print(postStr.length());
client.print("\n\n");
client.print(postStr);
 
Serial.print("Temperatura: ");
Serial.print(t);
Serial.print(" ºC ");
Serial.println("");
Serial.print("Humedad: ");
Serial.print(h);
Serial.print("%");
Serial.println("");
Serial.print("Enviando datos a Thingspeak");
Serial.println("");
}
client.stop();
 
Serial.println("Esperando 20 secs");
// thingspeak needs at least a 15 sec delay between updates
// 20 seconds to be safe
delay(20000);
}