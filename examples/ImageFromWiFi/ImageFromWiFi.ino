#include <WiFi.h>

#include <Adafruit_GFX.h>
#include <Fonts/Picopixel.h>
#include <P3RGB64x32MatrixPanel.h>

// constructor with default pin wiring
P3RGB64x32MatrixPanel matrix;

// use this constructor for custom pin wiring instead of the default above
// these pins are an example, you may modify this according to your needs
//P3RGB64x32MatrixPanel matrix(25, 26, 27, 21, 22, 23, 15, 32, 33, 12, 16, 17, 18);

char ssid[] = "****SSID****";
char pass[] = "**PASSWORD**";
WiFiServer server(20032);

void setup() {
  Serial.begin(115200);

  Serial.print("Attempting to connect to Network named: ");
  Serial.println(ssid);                   // print the network name (SSID);

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("");
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  server.begin();                           // start the server

  matrix.begin();                           // setup the LED matrix

  matrix.setTextColor(matrix.color444(0, 0, 8));  // print IP address
  matrix.println("IP address");
  matrix.setTextColor(matrix.color444(12, 12, 12));
  matrix.setFont(&Picopixel);
  matrix.println(ip);
}

void loop()
{
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {
    Serial.println("new client");
    unsigned int idx = 0;
    while (client.connected()) {
      if (client.available()) {
        int len = client.read((byte*)matrix.matrixbuff + idx , 64*32*2 - idx);
        idx = (idx + len) & (64 * 32 * 2 - 1);
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}

