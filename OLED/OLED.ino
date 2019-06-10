//Copyright 2019
//IOT-port.com
//fonts at http://oleddisplay.squix.ch/

#include "SSD1306Wire.h"
#include "Timer.h"

#define ledPin 5 
//#define wakePin 16    //used for ESP8266
#define Sleeptime 10
#define OLED_X_MAX  128
#define OLED_Y_MAX  64

// Initialize the OLED display using Wire library
SSD1306Wire  display(0x3c, 4, 15);  //18=SDK  19=SCK  As per labeling on ESP32 DevKit
Timer t;
const int wdtTimeout = 3000;  //time in ms to trigger the watchdog
hw_timer_t *timer = NULL;

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

void drawFontText(char *text[]) {
    // Font Demo
    // create more fonts at http://oleddisplay.squix.ch/
    display.setTextAlignment(TEXT_ALIGN_LEFT);

    display.setFont(ArialMT_Plain_16);
    for (int i=0;i<4;i++)
    {
      if (text[i]!= NULL) display.drawString(0, i*15, text[i]);
    }     
}

void drawRectEmpty(int x1,int y1,int x2, int y2) {
    // Draw a pixel at given position
    //display.setPixel(i, i);

    display.drawRect(x1,y1,x2,y2);

    // Fill the rectangle
    //display.fillRect(14, 14, 17, 17);

    // Draw a line horizontally
    //display.drawHorizontalLine(0, 40, 20);

    // Draw a line horizontally
    //display.drawVerticalLine(40, 0, 20);
}

void drawImageDemo() {
    // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
    // on how to create xbm files
    //display.drawXbm(34, 14, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
}

void setup() {
  Serial.begin(115200);
  while(!Serial) { 
  }
  Serial.println(" ");// print an empty line before and after Button Press    
  Serial.println("Power up");
  Serial.println(" ");  

  /* Use 1st timer of 4 */
  /* 1 tick take 1/(80MHZ/80) = 1us so we set divider 80 and count up */
  timer = timerBegin(0, 80, true);
  /* Attach onTimer function to our timer */
  timerAttachInterrupt(timer, &onTimer, true);
  /* every second 1 tick is 1us  => 1 second is 1000000us, Repeat the alarm (third parameter) */
  timerAlarmWrite(timer, 1000000, true);  
  /* Start an alarm */
  timerAlarmEnable(timer);
  Serial.println("Timer started..");
  
  drawLcdInit();
  char *myString[] = {"www.iot-port.com","Arduino","Cool IoT Projects", "WiFi,IFTTT,OLED"};

  display.clear();
  
  drawFontText(myString);
  //drawRectEmpty(0, 0, OLED_X_MAX, OLED_Y_MAX);
  // write the buffer to the display
  display.display();
  
  while(true){
    if (false)
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
