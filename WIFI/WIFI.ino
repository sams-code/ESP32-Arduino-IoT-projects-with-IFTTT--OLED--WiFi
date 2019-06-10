//Code for the www.MakeUseOf.com Wi-Fi connected Button tutorial by Ian Buckley

#include <IFTTTWebhook.h>
//#include <ESP8266WiFi.h>
#include <WiFi.h>
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`

#define ledPin 5 
#define wakePin 16
#define Sleeptime 5*60
#define ssid "PAL"
#define password "SAJHOU123"
#define IFTTT_API_KEY "b9LeJdavlm7YpzjpAHiBMv"
#define IFTTT_EVENT_NAME "Pushbutton"

// Initialize the OLED display using Wire library
//SSD1306Wire  display(0x3c, 18, 19);  //18=SDK  19=SCK  As per labeling on ESP32 DevKit
SSD1306Wire  display(0x3c, 4, 15);  //18=SDK  19=SCK  As per labeling on ESP32 DevKit

WiFiClient client;
//WiFiClientSecure client;
bool dataAvailable = false;
String postData;
String value1, value2, value3 = "";

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
    //display.setFont(ArialMT_Plain_10);
    //display.drawString(0, 0, "Hello world");
    //display.setFont(ArialMT_Plain_16);
    //display.drawString(0, 10, "Hello world");
    //display.setFont(ArialMT_Plain_24);
    //display.drawString(0, 26, "Hello world");

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

  //connectToWifi();
  //WiFi.persistent(false);

  while(true){
    if (checkconnectToWifi())
    {
      if (checkconnectToIFTTT())
      {        
        pinMode(ledPin, OUTPUT);
        digitalWrite(ledPin, HIGH);       
        
        //IFTTTWebhook hook(IFTTT_API_KEY, IFTTT_EVENT_NAME);
        //hook.trigger();
        
        Serial.println("POST");
        if (true)
        {
          int temp = random(300);
          
          setValue(1,String(temp));
          setValue(2,"250");
          setValue(3,"50");
        }
        post();   
        
        delay(200);    
        digitalWrite(ledPin, LOW); 
      }
      //WiFi.disconnect();
    }
    //ESP.deepSleep(wakePin); 

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

bool checkconnectToWifi() {
  if(WiFi.status() != WL_CONNECTED) {
    Serial.println("Wifi not connected");
    return connectToWifi();
  }
  return true;
}

bool checkconnectToIFTTT(){
    if (!client.connect("maker.ifttt.com", 80)) {
    //if (!client.connect("192.168.1.100", 80)) {
      Serial.println("Server connection failed");
      return false;
    }              
    return true;
}

bool post()
{
  compileData();
  client.print("POST /trigger/");
  client.print(IFTTT_EVENT_NAME);
  client.print("/with/key/");
  client.print(IFTTT_API_KEY);
  client.println(" HTTP/1.1");

  client.println("Host: maker.ifttt.com");
  client.println("User-Agent: Arduino/1.0");
  client.println("Connection: close");
  if (dataAvailable)
  { // append json values if available
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(postData.length());
    client.println();
    client.println(postData);
    Serial.println(postData);
  }
  else
    client.println();

  return true;    
}

bool setValue(int valueToSet, String value)
{
  switch (valueToSet)
  {
    case 1:
      value1 = value;
      break;
    case 2:
      value2 = value;
      break;
    case 3:
      value3 = value;
      break;
    default:
      return false;
      break;
  }
  return true;
}

void compileData()
{
  if (value1 != "" || value2 != "" || value3 != "")
  {
    dataAvailable = true;
    bool valueEntered = false;
    postData = "{";
    if (value1 != "")
    {
      postData.concat("\"value1\":\"");
      postData.concat(value1);
      valueEntered = true;
    }
    if (value2 != "")
    {
      if (valueEntered)postData.concat("\",");
      postData.concat("\"value2\":\"");
      postData.concat(value2);
      valueEntered = true;
    }
    if (value3 != "")
    {
      if (valueEntered)postData.concat("\",");
      postData.concat("\"value3\":\"");
      postData.concat(value3);
    }
    postData.concat("\"}");
  }
  else dataAvailable = false;
}

bool connectToWifi() {
  Serial.print("Connecting to: "); //uncomment next line to show SSID name
  Serial.print(ssid); 
  WiFi.begin(ssid, password);  
  Serial.println(" ");
  Serial.print("Attempting to connect: ");

  //try to connect for 10 seconds
  int i = 10;
  while(WiFi.status() != WL_CONNECTED && i >=0) {
    delay(1000);
    Serial.print(i);
    Serial.print(", ");
    i--;
  }
  Serial.println(" ");

  //print connection result
  if(WiFi.status() == WL_CONNECTED){
    Serial.println("Connected."); 
    Serial.print("NodeMCU IP Address: "); 
    Serial.println(WiFi.localIP());
    return true; 
  }
  else{
    Serial.println("Connection failed - check your credentials or connection");
    return false; 
  }
  //delay(3000);
}



void loop(){
//if deep sleep is working, this code will never run.
Serial.println("This should never get printed");
delay(1000);
}