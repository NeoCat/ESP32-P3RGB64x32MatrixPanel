#include <WiFi.h>

#include <Adafruit_GFX.h>
#include <P3RGB64x32MatrixPanel.h>
#include <Fonts/FreeSansBold9pt7b.h>

// constructor with default pin wiring
P3RGB64x32MatrixPanel matrix;

// use this constructor for custom pin wiring instead of the default above
// these pins are an example, you may modify this according to your needs
//P3RGB64x32MatrixPanel matrix(25, 26, 27, 21, 22, 23, 15, 32, 33, 12, 16, 17, 18);

char ssid[] = "****SSID****";
char pass[] = "**PASSWORD**";

#define TZ (9*60*60) /*JST*/

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

  configTime(TZ, 0, "ntp.nict.jp", "ntp.jst.mfeed.ad.jp"); // enable NTP

  matrix.begin();                           // setup the LED matrix
}

void loop()
{
  if (WiFi.status() != WL_CONNECTED)
    ESP.restart();

  time_t t;
  static time_t last_t;
  struct tm *tm;
  static const char* const wd[7] = {"Sun","Mon","Tue","Wed","Thr","Fri","Sat"};

  t = time(NULL);
  if (last_t == t) return; // draw each second
  last_t = t;
  tm = localtime(&t);
  matrix.fillScreen(0);

  matrix.setTextColor(matrix.color444(15, 15, 15));
  matrix.setFont(&FreeSansBold9pt7b);
  matrix.setCursor(0, 13);
  matrix.printf("%02d:%02d", tm->tm_hour, tm->tm_min);

  matrix.setFont();
  matrix.printf(":%02d", tm->tm_sec);

  matrix.setCursor(2, 16);
  matrix.setTextColor(tm->tm_wday == 0 ? matrix.color444(15, 0, 0) :
                      tm->tm_wday == 6 ? matrix.color444(0, 6, 15) :
                                         matrix.color444(6, 15, 6));
  matrix.printf("%s", wd[tm->tm_wday]);

  matrix.setCursor(2, 24);
  matrix.setTextColor(matrix.color444(12, 12, 4));
  matrix.printf("%04d/%02d/%02d\n", tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday);

  matrix.swapBuffer(); // display the image written to the buffer
}
