#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <splash.h>
#include "PinChangeInterrupt.h" 

//display definitions; display is 70x40 ssd1306
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define SCREEN_START_X 29
#define SCREEN_START_Y 24
#define SCREEN_REAL_WIDTH 70
#define SCREEN_REAL_HEIGHT 40
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


float voltage;
float current;

int voltpin = A1;
int amppin = A0;
float multiplier = 0.222;

int sampleCount = 256;
float interval_current;

void displayValues(){
  
  display.clearDisplay();  
  display.setCursor(SCREEN_START_X, SCREEN_START_Y);
  String line = String(voltage)+"V";
  display.print(line);
  display.setCursor(SCREEN_START_X, SCREEN_START_Y + 24);
  line = String(current)+"A";
  display.print(line);
  display.display();
}

void setup()
{
	Serial.begin(9600);
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  displayValues();
}

void loop()
{
	float temp;
	temp=analogRead(voltpin);
	voltage=((temp/4.105775)/10);
  
  float sensorRead = 0.0;
  float currentTemp = 0.0;
  float sampleTotal = 0.0;

  // buck converter outputs 4.96 volts
  for (int x = 0; x < sampleCount; x++){
    sensorRead = analogRead(A0)*(4.96 / 1024.0);  
    currentTemp = (((sensorRead) - (4.96/2))/multiplier);
    sampleTotal += currentTemp;
    interval_current = max(interval_current, currentTemp);
    delay(2);
  }  
  current = interval_current;
  interval_current = 0;
  displayValues();

}