//#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>

const char* ssid = "UAP";
const char* password = "princepessa";
MDNSResponder mdns;

ESP8266WebServer server(8267);


int relayInput = 2; // NodeMCU pin D4. sends the input signal.

void handleRoot() {
  server.send(200, "text/html",
              "<html>" \
                "<head><title>Josh's Garage Door Opener</title></head>" \
                "<body>" \
                  "<h1>Open Josh's garage door</h1>" \
                  "<p><a href=\"activate\">Activate</a></p><body></html>");
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  server.send(404, "text/plain", message);
}

void handleActivate() {
  digitalWrite(relayInput, HIGH);
    delay(1000);
    digitalWrite(relayInput, LOW);
  server.send(200, "text/html",
              "<html>" \
              "<head><title>Activated!</title></head>"
              "<body>" \
              "<h1>It should be open, but might be closed!</h1></body></html>");   
  
}
  


void setup(void) {
  // put your setup code here, to run once:
  pinMode(relayInput, OUTPUT); //initialize pin as OUTPUT

  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP().toString());

  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/activate", handleActivate);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
 
}

void loop(void) {
  // put your main code here, to run repeatedly:
  server.handleClient();
  }
