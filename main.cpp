#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
const char* apiURL = "https://your-api-id.execute-api.region.amazonaws.com/time";

void drawCenteredText(const char* text, uint8_t textSize) {
  display.setTextSize(textSize);
  display.setTextColor(WHITE);
  int charWidth = 6 * textSize;
  int charHeight = 8 * textSize;
  int textLength = strlen(text);
  int textPixelWidth = textLength * charWidth;
  int x = (128 - textPixelWidth) / 2;
  int y = (64 - charHeight) / 2;

  display.clearDisplay();
  display.setCursor(x, y);
  display.println(text);
  display.display();
}

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("❌ SSD1306 not found");
    while (true);
  }

  display.clearDisplay();
  drawCenteredText("Connecting...", 1);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("✅ WiFi connected");
  drawCenteredText("WiFi OK", 1);
  delay(1000);
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(apiURL);
    int httpCode = http.GET();

    if (httpCode == 200) {
      String payload = http.getString();
      Serial.println(payload);

      StaticJsonDocument<256> doc;
      if (deserializeJson(doc, payload) == DeserializationError::Ok) {
        const char* timeStr = doc["time"];
        drawCenteredText(timeStr, 2);
      }
    } else {
      Serial.print("HTTP Error: ");
      Serial.println(httpCode);
    }

    http.end();
  }

  delay(1000);
}