#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


Adafruit_SSD1306 display(128, 64, &Wire, -1);

static const uint8_t PROGMEM sun[] = {
    0x00, 0x00, 0x01, 0x00, 0x41, 0x04, 0x21, 0x08,
    0x10, 0x10, 0x07, 0xc0, 0x0f, 0xe0, 0x0f, 0xe0,
    0xef, 0xee, 0x0f, 0xe0, 0x0f, 0xe0, 0x07, 0xc0,
    0x10, 0x10, 0x21, 0x08, 0x41, 0x04, 0x01, 0x00
};

static const uint8_t PROGMEM moon[] = {
    0x00, 0xf0, 0x03, 0xf8, 0x07, 0xf0, 0x0f, 0xc0,
    0x0f, 0x80, 0x1f, 0x80, 0x1f, 0x00, 0x1f, 0x00,
    0x1f, 0x00, 0x1f, 0x80, 0x0f, 0x80, 0x0f, 0xc0,
    0x07, 0xf0, 0x03, 0xf8, 0x00, 0xf0, 0x00, 0x00
};

void draw_frame()
{
    display.drawBitmap(0, 0, sun, 16, 16, 1);
    display.drawBitmap(0, 20, moon, 16, 16, 1);
    display.setCursor(24,5);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.println(F("7:00"));
    display.setCursor(24, 24);
    display.println(F("21:00"));

}

void setup()
{
    Serial.begin(9600);

    pinMode(PD5, INPUT);
    pinMode(PD6, INPUT);
    pinMode(PD7, INPUT);
    pinMode(PD4, OUTPUT);


    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3c))
    {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;);
    }

    display.clearDisplay();
    draw_frame();
    display.display();
    delay(2000);
}

void loop()
{
    if (digitalRead(PD5) == HIGH)
    {
        digitalWrite(PD4, HIGH);
    }
    else
    {
        digitalWrite(PD4, LOW);
    }
}
