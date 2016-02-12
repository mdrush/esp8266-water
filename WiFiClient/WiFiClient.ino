#include <ESP8266WiFi.h>
#include <AllAboutEE_MCP3021.h>
#include <Wire.h>

const char* ssid     = "your-ssid";
const char* password = "your-password";

const char* host = "data.sparkfun.com";
const char* streamId   = "....................";
const char* privateKey = "....................";

const int retryLimit = 5;

// Using a static IP should save time
IPAddress ip(192, 168, 1, 100);
IPAddress gateway(192, 168, 1, 1);  
IPAddress subnet(255, 255, 255, 0);  

bool sendValues();

AllAboutEE::MCP3021 mcp3021;

void setup() {
  mcp3021.begin(0,2); // sda, scl
  
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid);
  WiFi.config(ip, gateway, subnet);  
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


//int value = 0;

void loop() {
  Serial.print("Raw Value: ");
  Serial.println(mcp3021.read(0));

  Serial.print("Analog Value: ");
  Serial.println(mcp3021.read(0, 3.3));

  delay(500);
}




/* Returns true if the request was sent and a reply was 
  received properly, else returns false */
bool sendValues() {
  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return false;
  }
  
  // We now create a URI for the request
  String url = "/input/";
  url += streamId;
  url += "?private_key=";
  url += privateKey;
  url += "&value=";
  //url += value;
  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(10);
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("closing connection\n");

  return true;
}





