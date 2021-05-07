 /////////////for hot spot
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
int Analog = A0;

/////////////for hotspot
MDNSResponder mdns;
// Replace with your network credentials
const char* ssid = "ESPServer_RAJ";
const char* password = "RAJ@12345";
String webString=""; 
ESP8266WebServer server(80);
String webPage = "";
String web="";
int pin1 = D1;
int pin2= D2;
int TEMP_level=0;
void setup()
{
  //pinMode(A0,INPUT);
  webPage +="<h2>ESP8266 Web Server new</h2><p>TEMP METER <a href=\"TEMP\"><button> TEMPERATURE (oC)</button></a></p>";
  webPage += "<p>MOTOR1-STATUS <a href=\"LED1ON\"><button>ON</button></a>&nbsp;<a href=\"LED1OFF\"><button>OFF</button></a></p>";
  webPage += "<p>MOTOR1-STATUS <a href=\"LED2ON\"><button>ON</button></a>&nbsp;<a href=\"LED2OFF\"><button>OFF</button></a></p>";

  // preparing GPIOs
  pinMode(pin1, OUTPUT);
  digitalWrite(pin1, LOW);
  delay(1000);
  pinMode(pin2, OUTPUT);
  digitalWrite(pin2, LOW);
  delay(1000);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  if (mdns.begin("esp8266", WiFi.localIP())) 
  {
    Serial.println("MDNS responder started");
  }
  
  server.on("/", []()
  {
    server.send(200, "text/html", webPage);
  });

  /**************************************************************************************/
  
  server.on("/TEMP", []()
    { 
      // if you add this subdirectory to your webserver call, you get text below :)
     get_TEMP();
     webString="TEMPERATURE: "+String((float)TEMP_level)+"oC";
     server.send(200, "text/plain", webString);            // send to someones browser when asked
    });


  server.on("/LED1ON", []()
  {
    server.send(200, "text/html", webPage);
    digitalWrite(pin1, HIGH);
    delay(1000);
  });
  server.on("/LED1OFF", []()
  {
    server.send(200, "text/html", webPage);
    digitalWrite(pin1, LOW);
    delay(1000); 
  });
  server.on("/LED2ON", []()
  {
    server.send(200, "text/html", webPage);
    digitalWrite(pin2, HIGH);
    delay(1000);
  });
  server.on("/LED2OFF", []()
  {
    server.send(200, "text/html", webPage);
    digitalWrite(pin2, LOW);
    delay(1000); 
  });
  server.begin();
  Serial.println("Congats Sir, Your HTTP server started");
}

void loop() 
{
server.handleClient();
get_TEMP();
delay(500);
}

void get_TEMP()
{
  int TEMP_level1= analogRead(Analog);
  TEMP_level=TEMP_level1/2;  
 }
