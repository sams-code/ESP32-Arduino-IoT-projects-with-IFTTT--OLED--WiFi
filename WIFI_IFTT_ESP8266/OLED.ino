//Copyright 2019
//IOT-port.com
//fonts at http://oleddisplay.squix.ch/

#include "SSD1306Wire.h"

enum Light {Blink = 1, Center = 2}; 

#define ledPin 5 
//#define wakePin 16    //used for ESP8266
#define Sleeptime 0.5
#define OLED_X_MAX  128
#define OLED_Y_MAX  64

// Initialize the OLED display using Wire library
SSD1306Wire  display(0x3c, 4, 15);  //18=SDK  19=SCK  As per labeling on ESP32 DevKit

volatile int interruptCounter;
int totalInterruptCounter;

char *disp_text[4];
byte disp_text_style[4];
long tick_counter = 0;

void LcdInit() {
  // put your setup code here, to run once:
  pinMode(16, OUTPUT);
  digitalWrite(16, LOW); // set GPIO16 low to reset OLED
  delay(50);
  digitalWrite(16, HIGH); // while OLED is running, must set GPIO16 to high
  Wire.begin(4, 15);

  display.init(); 
  display.flipScreenVertically(); 

  disp_text[0] = "Self Test";
  disp_text[1] = "Self Test";;
  disp_text[2] = "Self Test";
  disp_text[3] = "Self Test";  

  for (int i=0;i<4;i++)
  {
    disp_text_style[i] = 0;
  }

  LcdUpdate();
}

void LcdUpdate(void) {

    display.clear();
  
    // Font Demo, create more fonts at http://oleddisplay.squix.ch/
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_16);
    
    for (int i=0;i<4;i++)
    {
      if (((disp_text_style[i] & Blink) == 0) || (tick_counter & 0x1))
      {
        if (disp_text[i]!= NULL) display.drawString(0, i*15, disp_text[i]); //line spacing: 15 pixels
      }
      //Serial.print("disp_text_style ");  
      //Serial.println(disp_text_style[i]);  
      //Serial.print("tick_counter "); 
      //Serial.println(tick_counter); 
    } 
    
    display.display();  
}

void drawRectEmpty(int x1,int y1,int x2, int y2) {
  
    display.drawRect(x1,y1,x2,y2);
    
    // Draw a pixel at given position
    //display.setPixel(i, i);
    // Fill the rectangle
    //display.fillRect(14, 14, 17, 17);
    // Draw a line horizontally
    //display.drawHorizontalLine(0, 40, 20);
    // Draw a line horizontally
    //display.drawVerticalLine(40, 0, 20);

    // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
    // on how to create xbm files
    //display.drawXbm(34, 14, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
}

void LcDMain() {

  disp_text[0] = "www.iot-port.com";
  disp_text[1] = "Arduino";;
  disp_text[2] = "Cool IoT Projects";

  char tStr[10];
  ltoa(tick_counter,tStr, 10);

  //disp_text[3] = "WiFi,IFTTT,OLED";
  disp_text[3] = tStr;
  
  disp_text_style[1] |=  Blink;
  disp_text_style[2] |=  Blink;
           
  //drawRectEmpty(0, 0, OLED_X_MAX, OLED_Y_MAX);
  // write the buffer to the display
  
  tick_counter++;
  LcdUpdate();
}
