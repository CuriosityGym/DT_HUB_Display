/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************

  You can send/receive any data using WidgetTerminal object.

  App project setup:
    Terminal widget attached to Virtual Pin V1
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <TimeLib.h>
#include <WidgetRTC.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
 #define PIN 2
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "7eac5c97945b4a41ad5188072cb3aaee";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "DT_LAB";
char passwd[] = "fthu@050318";

BlynkTimer timer;

WidgetRTC rtc;

String hours="";
String minuits="";
int h=0;
int m=0;
 String data="";
 String defautMsg= "Design Thinking Hub is open, Lets make something interesting";
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(64, 8, PIN,
  NEO_MATRIX_TOP    + NEO_MATRIX_LEFT + 
  NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);
 
const uint16_t colors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(255, 255, 0),matrix.Color(0, 0, 255), matrix.Color(255, 0, 255), matrix.Color(0, 255, 255), matrix.Color(255, 255, 255)};

  
int x    = matrix.width();
int pass = 0;
int val = 0;
bool dataRcvd = false;
// Attach virtual serial terminal to Virtual Pin V1
WidgetTerminal terminal(V1);


// Digital clock display of the time
void clockDisplay()
{
  // You can call hour(), minute(), ... at any time
  // Please see Time library examples for details

  String currentTime = String(hour()) + ":" + minute() + ":" + second();
  String currentDate = String(day()) + " " + month() + " " + year();
  Serial.print("Current time: ");
  Serial.print(currentTime);
  Serial.print(" ");
  Serial.print(currentDate);
  Serial.println();
  m=minute();
  h=hour();
  // Send time to the App
  Blynk.virtualWrite(V1, currentTime);
  // Send date to the App
  Blynk.virtualWrite(V2, currentDate);
}
int roundMinuits(int mnt){
  if (mnt <= 10) mnt = 10;
  if (mnt > 10 && mnt <= 20) mnt = 20;
  if (mnt > 20 && mnt <= 30) mnt = 30;
  if (mnt > 30 && mnt <= 40) mnt = 40;
  if (mnt > 40 && mnt <= 50) mnt = 50;
  if (mnt > 50 && mnt < 60) mnt = 00;
  return mnt;
}
// You can send commands from Terminal to your hardware. Just use
// the same Virtual Pin as your Terminal Widget
BLYNK_WRITE(V1)
{
  data = param.asStr();
  Serial.println(data);
  if(data == "sixty"){
    int hr=h;
    hr=(hr+1)%12;
    m = roundMinuits(m);
    minuits = String(m);
    if(m ==0) {minuits = "00"; hr=hr+1;}
    hours = String(hr);
    data="Design Thinking Hub is closed. Hub will open at "+hours +":"+minuits+"pm";
    Serial.println(data);
  }
  if(data == "thirty"){
    int mint = (m+30)%60;
    int hrs= h;
    hrs= hrs%12;
    if((m+30)>=59) {
      hrs= hrs+1;
      
    }
    
   /* if (mint <= 10) mint = 10;
    if (mint > 10 && mint <= 20) mint = 20;
    if (mint > 20 && mint <= 30) mint = 30;
    if (mint > 30 && mint <= 40) mint = 40;
    if (mint > 40 && mint <= 50) mint = 50;
    if (mint > 50 && mint < 60)  mint = 55;*/
    mint = roundMinuits(mint);
    minuits = String(mint);
    if(mint ==0) {minuits = "00";hrs=hrs+1;}
    hours = String(hrs);
    data="Design Thinking Hub is closed. Hub will open at "+hours +":"+minuits+"pm";
    Serial.println(data);
  }

  if(data == "LT"){
    int hrs=h;
    hrs= (hrs+1)%12;
    
    
    m = roundMinuits(m);
    minuits = String(m);
    if(m ==0) {minuits = "00";hrs = hrs+1;}
    hours = String(hrs);
    data ="Lunch Time. Hub will open at "+hours +":"+minuits+"pm";
    Serial.println(data);
  }
  dataRcvd = true;
  // if you type "Marco" into Terminal Widget - it will respond: "Polo:"
 // if (String("Marco") == param.asStr()) {
 //   terminal.println("You said: 'Marco'") ;
 //   terminal.println("I said: 'Polo'") ;
 /* } else {

    // Send it back
    terminal.print("You said:");
    terminal.write(param.getBuffer(), param.getLength());
    terminal.println();
  }*/

  // Ensure everything is sent
  terminal.flush();
}
BLYNK_WRITE(V2)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  // You can also use:
  // String i = param.asStr();
  // double d = param.asDouble();
  Serial.print("V2 Slider value is: ");
  Serial.println(pinValue);
  matrix.setBrightness(pinValue);
}
void setup()
{
  // Debug console
  Serial.begin(9600);
  rtc.begin();
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(5);
  matrix.setTextColor(colors[0]);
  Blynk.begin(auth, ssid, passwd);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 8442);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8442);

  // This will print Blynk Software version to the Terminal Widget when
  // your hardware gets connected to Blynk Server
  terminal.println(F("Blynk v" BLYNK_VERSION ": Device started"));
  terminal.println(F("-------------"));
  terminal.println(F("Smart Display"));
  //terminal.println(F("anything else and get it printed back."));
  terminal.flush();
   // Begin synchronizing time
  

  // Display digital clock every 10 seconds
  timer.setInterval(10000L, clockDisplay);
  
}

void loop()
{
  
  Blynk.run();
  timer.run();
  if(dataRcvd == false)
    {
     matrix.fillScreen(0);
     matrix.setCursor(x, 0);
     matrix.print(defautMsg);
     val = defautMsg.length();
     val= val*7;
     if(--x < -val) {
        x = matrix.width();
     
        if(++pass >= 8) pass = 0;
        matrix.setTextColor(colors[pass]);
      }
    }
  if(dataRcvd == true){  
  matrix.fillScreen(0);
  matrix.setCursor(x, 0);
  matrix.print(data);
  val = data.length();
  val= val*7;
  if(--x < -val) {
    x = matrix.width();
 
    if(++pass >= 8) pass = 0;
    matrix.setTextColor(colors[pass]);
  }}
  matrix.show();
  delay(30);
}
