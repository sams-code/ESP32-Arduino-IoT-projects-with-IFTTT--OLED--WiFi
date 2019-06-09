//Copyright 2019
//IOT-port.com
//fonts at http://oleddisplay.squix.ch/

#include "SSD1306Wire.h"

#define ledPin 5 
//#define wakePin 16
#define Sleeptime 10

// Initialize the OLED display using Wire library
SSD1306Wire  display(0x3c, 4, 15);  //18=SDK  19=SCK  As per labeling on ESP32 DevKit

void drawLcdInit() {
  // put your setup code here, to run once:
  pinMode(16, OUTPUT);
  digitalWrite(16, LOW); // set GPIO16 low to reset OLED
  delay(50);
  digitalWrite(16, HIGH); // while OLED is running, must set GPIO16 to high
  Wire.begin(4, 15);

  display.init(); 
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);   
}

void drawFontFaceDemo() {
  // clear the display
  display.clear();
    // Font Demo1
    // create more fonts at http://oleddisplay.squix.ch/
    display.setTextAlignment(TEXT_ALIGN_LEFT);

    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 0, "Thank you Jesus");
    display.drawString(0, 13, ", Mary and Joseph");
        
  // write the buffer to the display
  display.display();
}

void setup() {
  Serial.begin(115200);
  while(!Serial) { 
  }
  Serial.println(" ");// print an empty line before and after Button Press    
  Serial.println("Power up");
  Serial.println(" ");  

  drawLcdInit();
  drawFontFaceDemo();

  while(true){
    if (true)
    {
      Serial.print("Sleep: ");
      Serial.print(Sleeptime); 
      Serial.println(" sec"); 
          
      ESP.deepSleep(Sleeptime * 1e6); 
      Serial.println("Wakeup");  
    }
    else
    {
      Serial.print("Wait for: ");
      Serial.print(Sleeptime);
      Serial.println(" sec");
          
      delay(Sleeptime * 1e3);      
      Serial.println("Startover");
    }
  }
}

void loop(){
//if deep sleep is working, this code will never run.
Serial.println("This should never get printed");
delay(1000);
}
