#include <ESP8266WiFi.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

const char* WIFI_SSID = "your-ssid";
const char* WIFI_PASSWORD = "your-password";

const char* URL_PATTERN = "GET /color/";
const byte URL_PATTERN_LENGTH = 11;

const int ARRAY_SIZE = 67;
const int KHZ = 38; // 38KHz carrier frequency
const int DELAY = 100; // 100ms delay between each signal burst

IRsend irsend(4);  // An IR LED is controlled by GPIO pin 4 (D2)
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  while (! Serial);

  irsend.begin();

  // Connect to WiFi network
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.print("HTTP server started at http://");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  while (!client.available()) {
    delay(1);
  }

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  // Get the color code to sent depending on request uri
  char colorCode = getColorCodeForRequest(req);

  // Send the IR code + HTTP response
  sendIRcode(colorCode);
  sendHttpResponse(client);
}

void sendIRcode(char newColor) {
  uint16_t rawData[ARRAY_SIZE];

  if (newColor == 'N') { // ON
    sendRawData(rawData, 8900, 4650, 400, 700, 450, 700, 450, 700, 400, 700, 450, 700, 450, 700, 400, 700, 450, 700, 450, 1800, 450, 1800, 400, 1850, 400, 1800, 450, 700, 450, 1800, 450, 1800, 400, 1800, 450, 1800, 450, 1800, 450, 700, 450, 650, 450, 700, 450, 700, 450, 700, 400, 700, 450, 700, 450, 700, 400, 1850, 400, 1800, 450, 1800, 450, 1800, 450, 1800, 400, 1850, 400);
  } else if (newColor == 'W') { // WHITE
    sendRawData(rawData, 8900, 4650, 400, 700, 450, 700, 450, 700, 400, 750, 400, 700, 450, 700, 450, 700, 400, 700, 450, 1800, 450, 1800, 450, 1800, 400, 1850, 400, 700, 450, 1800, 450, 1800, 450, 1800, 400, 1850, 400, 1850, 400, 1800, 450, 700, 450, 700, 400, 700, 450, 700, 450, 700, 450, 700, 400, 700, 450, 700, 450, 1800, 400, 1850, 400, 1850, 400, 1800, 450, 1800, 450);
  } else if (newColor == 'R') { // RED
    sendRawData(rawData, 8950, 4600, 450, 700, 450, 650, 500, 650, 450, 700, 450, 650, 500, 650, 450, 700, 450, 650, 500, 1750, 500, 1750, 450, 1800, 450, 1800, 450, 650, 500, 1750, 450, 1800, 450, 1800, 450, 650, 500, 650, 500, 1750, 450, 700, 450, 650, 500, 650, 450, 700, 450, 650, 500, 1750, 500, 1750, 450, 700, 450, 1800, 450, 1750, 500, 1750, 500, 1750, 450, 1800, 450);
  } else if (newColor == 'G') { // GREEN
    sendRawData(rawData, 8950, 4600, 450, 700, 450, 700, 450, 650, 450, 700, 450, 700, 450, 650, 500, 650, 450, 700, 450, 1800, 450, 1750, 500, 1750, 450, 1800, 450, 700, 450, 1800, 450, 1750, 500, 1750, 450, 1800, 450, 700, 450, 1750, 500, 650, 450, 700, 450, 700, 450, 650, 500, 650, 450, 700, 450, 1750, 500, 650, 450, 1800, 450, 1800, 450, 1800, 450, 1750, 500, 1750, 500);
  } else if (newColor == 'B') { // BLUE
    sendRawData(rawData, 8950, 4600, 450, 700, 400, 750, 400, 700, 450, 700, 450, 700, 400, 700, 450, 700, 450, 700, 400, 1850, 400, 1800, 450, 1800, 450, 1800, 450, 700, 400, 1800, 450, 1800, 450, 1800, 450, 700, 400, 1850, 400, 1800, 450, 700, 450, 700, 450, 700, 400, 700, 450, 700, 450, 1800, 400, 750, 400, 700, 450, 1800, 450, 1800, 400, 1850, 400, 1800, 450, 1800, 450);
  } else if (newColor == 'O') { // ORANGE
    sendRawData(rawData, 8900, 4600, 450, 700, 450, 700, 400, 700, 450, 700, 450, 700, 400, 700, 450, 700, 450, 700, 400, 1800, 450, 1800, 450, 1800, 450, 1800, 400, 700, 450, 1800, 450, 1800, 450, 1800, 400, 750, 400, 700, 450, 700, 400, 1850, 400, 700, 450, 700, 450, 700, 400, 700, 450, 1800, 450, 1800, 450, 1800, 400, 700, 450, 1800, 450, 1800, 450, 1800, 400, 1850, 400);
  } else if (newColor == 'L') { // LIME
    sendRawData(rawData, 8900, 4650, 450, 650, 450, 700, 450, 700, 450, 700, 400, 700, 450, 700, 450, 700, 400, 700, 450, 1800, 450, 1800, 450, 1800, 450, 1800, 400, 700, 450, 1800, 450, 1800, 450, 1800, 400, 1850, 400, 700, 450, 700, 450, 1800, 450, 700, 400, 700, 450, 700, 450, 700, 400, 700, 450, 1800, 450, 1800, 450, 650, 450, 1850, 400, 1800, 450, 1800, 450, 1800, 450);
  } else if (newColor == 'V') { // VIOLET
    sendRawData(rawData, 8950, 4650, 400, 700, 450, 700, 450, 700, 400, 700, 450, 700, 450, 650, 500, 700, 400, 700, 450, 1800, 450, 1800, 450, 1800, 400, 1850, 400, 700, 450, 1800, 450, 1800, 400, 1850, 400, 700, 450, 1800, 450, 700, 400, 1850, 400, 700, 450, 700, 450, 700, 400, 700, 450, 1800, 450, 650, 500, 1800, 400, 700, 450, 1800, 450, 1800, 450, 1800, 400, 1850, 400);
  } else if (newColor == 'H') { // PEACH
    sendRawData(rawData, 8900, 4600, 500, 650, 450, 700, 450, 650, 500, 650, 450, 700, 450, 700, 450, 700, 400, 700, 450, 1800, 450, 1750, 500, 1800, 400, 1850, 400, 700, 400, 1850, 400, 1850, 400, 1850, 400, 750, 350, 750, 400, 1850, 400, 1850, 400, 750, 400, 700, 450, 700, 400, 700, 450, 1800, 450, 1800, 450, 700, 400, 700, 450, 1800, 450, 1800, 450, 1800, 400, 1850, 400);
  } else if (newColor == 'E') { // EMERALD
    sendRawData(rawData, 8850, 4600, 500, 650, 450, 700, 450, 700, 450, 700, 400, 700, 450, 700, 450, 700, 400, 700, 450, 1800, 450, 1800, 450, 1800, 400, 1850, 400, 700, 450, 1800, 450, 1800, 450, 1800, 400, 1800, 450, 700, 450, 1800, 450, 1750, 500, 650, 450, 700, 450, 700, 450, 700, 400, 700, 450, 1800, 450, 700, 400, 700, 450, 1800, 450, 1800, 450, 1800, 450, 1750, 450);
  } else if (newColor == 'U') { // PURPLE
    sendRawData(rawData, 8950, 4650, 400, 700, 450, 700, 450, 700, 400, 700, 450, 700, 450, 700, 400, 700, 450, 700, 450, 1800, 450, 1800, 400, 1850, 400, 1850, 400, 700, 450, 1800, 450, 1800, 450, 1800, 400, 700, 450, 1800, 450, 1800, 450, 1800, 450, 700, 400, 700, 450, 700, 450, 700, 400, 1850, 400, 700, 450, 700, 450, 700, 400, 1850, 400, 1800, 450, 1800, 450, 1800, 450);
  } else if (newColor == 'D') { // DANDELION
    sendRawData(rawData, 8900, 4650, 400, 700, 400, 750, 400, 750, 400, 750, 350, 750, 400, 750, 400, 750, 350, 750, 400, 1850, 400, 1850, 400, 1800, 400, 1850, 400, 750, 400, 1850, 400, 1850, 400, 1800, 400, 750, 400, 750, 400, 750, 400, 700, 450, 1800, 400, 750, 400, 700, 450, 700, 400, 1850, 400, 1850, 400, 1850, 400, 1800, 450, 700, 400, 1850, 400, 1850, 400, 1850, 400);
  } else if (newColor == 'T') { // TURQUOISE
    sendRawData(rawData, 8900, 4600, 450, 650, 500, 700, 400, 700, 450, 700, 450, 700, 400, 700, 450, 700, 450, 700, 400, 1800, 450, 1800, 450, 1800, 450, 1800, 450, 700, 400, 1800, 450, 1800, 450, 1800, 450, 1800, 450, 700, 400, 700, 450, 700, 450, 1800, 400, 700, 450, 700, 450, 700, 450, 650, 450, 1800, 450, 1800, 450, 1800, 450, 650, 450, 1800, 450, 1800, 450, 1800, 450);
  } else if (newColor == 'M') { // MAGENTA
    sendRawData(rawData, 8900, 4650, 400, 750, 400, 700, 400, 750, 400, 750, 400, 700, 450, 700, 400, 750, 400, 700, 450, 1800, 450, 1800, 400, 1850, 400, 1850, 400, 700, 450, 1800, 450, 1800, 450, 1800, 400, 750, 400, 1800, 450, 700, 450, 700, 400, 1850, 400, 700, 450, 700, 400, 750, 400, 1800, 450, 700, 450, 1800, 450, 1800, 400, 700, 450, 1800, 450, 1800, 450, 1800, 400);
  } else if (newColor == 'Y') { // YELLOW
    sendRawData(rawData, 8900, 4650, 400, 700, 450, 700, 450, 700, 400, 700, 450, 700, 450, 700, 400, 700, 450, 700, 450, 1800, 450, 1800, 400, 1850, 400, 1800, 450, 700, 450, 1800, 400, 1800, 450, 1850, 400, 700, 450, 700, 400, 1850, 400, 700, 450, 1800, 450, 700, 450, 700, 400, 700, 450, 1800, 450, 1800, 450, 700, 400, 1800, 450, 700, 450, 1800, 450, 1750, 450, 1850, 400);
  } else if (newColor == 'C') { // CYAN
    sendRawData(rawData, 8900, 4650, 400, 700, 450, 700, 450, 700, 400, 700, 450, 700, 450, 700, 400, 700, 450, 700, 450, 1800, 450, 1800, 400, 1850, 400, 1800, 450, 700, 450, 1800, 450, 1800, 400, 1850, 400, 1800, 450, 700, 450, 1750, 500, 700, 400, 1800, 450, 700, 450, 650, 450, 750, 400, 700, 450, 1800, 450, 700, 400, 1850, 400, 700, 450, 1800, 450, 1800, 400, 1850, 400);
  } else if (newColor == 'P') { // PINK
    sendRawData(rawData, 8950, 4600, 450, 700, 400, 700, 450, 700, 450, 700, 450, 700, 400, 700, 450, 700, 450, 700, 400, 1800, 450, 1800, 450, 1800, 450, 1800, 450, 700, 400, 1800, 450, 1800, 450, 1800, 450, 700, 400, 1850, 400, 1850, 400, 700, 450, 1800, 450, 650, 450, 700, 450, 700, 450, 1800, 450, 650, 450, 700, 450, 1800, 450, 700, 400, 1850, 400, 1800, 450, 1800, 450);
  } else if (newColor == '1') { // FLASH
    sendRawData(rawData, 8900, 4600, 450, 700, 450, 700, 400, 700, 450, 700, 450, 700, 400, 700, 450, 700, 450, 700, 400, 1850, 400, 1800, 450, 1800, 450, 1800, 450, 700, 400, 1850, 400, 1800, 450, 1800, 450, 1800, 450, 1800, 400, 700, 450, 1800, 450, 700, 450, 700, 400, 700, 450, 700, 450, 700, 400, 700, 450, 1800, 450, 700, 450, 1800, 400, 1850, 400, 1800, 450, 1800, 450);
  } else if (newColor == '2') { // STROBE
    sendRawData(rawData, 8900, 4650, 450, 700, 400, 700, 450, 700, 450, 700, 400, 750, 400, 700, 450, 700, 400, 750, 400, 1800, 450, 1800, 450, 1800, 450, 1800, 400, 700, 450, 1800, 450, 1800, 450, 1800, 450, 1750, 450, 1850, 400, 1800, 450, 1800, 450, 700, 450, 700, 400, 700, 450, 700, 450, 700, 400, 700, 450, 700, 450, 700, 400, 1850, 400, 1800, 450, 1800, 450, 1800, 450);
  } else if (newColor == '3') { // FADE
    sendRawData(rawData, 8900, 4650, 450, 700, 400, 700, 450, 700, 450, 700, 400, 700, 450, 700, 450, 700, 400, 700, 450, 1800, 450, 1800, 450, 1800, 450, 1800, 400, 700, 450, 1800, 450, 1800, 450, 1800, 450, 1800, 400, 1850, 400, 700, 450, 700, 450, 1800, 400, 700, 450, 700, 450, 650, 500, 700, 400, 700, 450, 1800, 450, 1750, 500, 650, 450, 1800, 450, 1800, 450, 1800, 450);
  } else if (newColor == '4') { // SMOOTH
    sendRawData(rawData, 8950, 4600, 450, 700, 450, 700, 400, 700, 450, 700, 450, 650, 450, 700, 450, 700, 450, 700, 400, 1850, 400, 1800, 450, 1800, 450, 1800, 450, 700, 400, 1850, 400, 1800, 450, 1800, 450, 1800, 450, 1800, 400, 1850, 400, 700, 450, 1800, 450, 700, 400, 750, 400, 700, 450, 700, 450, 700, 400, 700, 450, 1800, 450, 700, 400, 1850, 400, 1800, 450, 1800, 450);
  } else if (newColor == '+') { // DIM UP
    sendRawData(rawData, 8950, 4600, 500, 650, 450, 650, 500, 650, 500, 650, 450, 650, 500, 650, 500, 650, 450, 650, 500, 1750, 500, 1750, 500, 1750, 500, 1750, 450, 650, 500, 1750, 500, 1750, 500, 1750, 500, 600, 500, 650, 500, 650, 450, 700, 450, 650, 500, 650, 450, 700, 450, 650, 500, 1750, 450, 1800, 450, 1800, 450, 1800, 450, 1750, 500, 1750, 500, 1750, 500, 1750, 500);
  } else if (newColor == '-') { // DIM DOWN
    sendRawData(rawData, 8900, 4650, 400, 700, 450, 700, 450, 700, 400, 700, 450, 700, 450, 700, 400, 700, 450, 700, 450, 1800, 400, 1850, 400, 1800, 450, 1800, 450, 700, 450, 1800, 400, 1850, 400, 1800, 450, 1800, 450, 700, 450, 700, 400, 700, 450, 700, 450, 700, 400, 700, 450, 700, 450, 700, 400, 1850, 400, 1800, 450, 1800, 450, 1800, 450, 1800, 400, 1850, 400, 1850, 400);
  } else { // TURN OFF
    sendRawData(rawData, 8900, 4650, 450, 700, 400, 700, 450, 700, 450, 700, 400, 700, 450, 700, 450, 700, 400, 750, 400, 1800, 450, 1800, 450, 1800, 450, 1800, 400, 750, 400, 1800, 450, 1800, 450, 1800, 450, 700, 400, 1850, 400, 700, 450, 700, 450, 700, 400, 700, 450, 700, 450, 700, 400, 1850, 400, 700, 450, 1800, 450, 1800, 400, 1850, 400, 1800, 450, 1800, 450, 1800, 450);
  }
}

char getColorCodeForRequest(String req) {
  int index = req.indexOf(URL_PATTERN);
  if (index != -1 && req.length() > index + URL_PATTERN_LENGTH) {
    return req[index + URL_PATTERN_LENGTH];
  }
  return 'F';
}

void sendRawData(uint16_t rawData[], ...) {
  va_list args;
  va_start (args, rawData);  // Requires the last fixed parameter (to get the address)
  for (int x = 0; x < ARRAY_SIZE; x++) {
    rawData[x] = va_arg(args, unsigned int); // Requires the type to cast to. Increments ap to the next argument
  }
  va_end(args);

  irsend.sendRaw(rawData, ARRAY_SIZE, KHZ);
  delay(DELAY);
}

void sendHttpResponse(WiFiClient client) {
  client.flush();
  client.print("HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n{\"success\": true}");
  delay(1);
  Serial.println("Client disconnected");
  // The client will actually be disconnected
  // when the function returns and 'client' object is destroyed
}
