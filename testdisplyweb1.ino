 
#include <WiFi.h>
#include <HTTPClient.h>
#include <UrlEncode.h>
#include <SPI.h>
#include "src/epd/epd4in37g.h"
#include "imagedata.h"
#include "src/gfx/GUI_Paint.h"
#include "src/fonts/fonts.h"

const char* ssid = "C3 LABS";
const char* password = "c34874455";

String getData, Link;
String URL = "http://eink-demo-psbdkefa.azurewebsites.net"; 


Epd epd;

unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;

uint8_t *BlackImage;
uint32_t Imagesize;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());


  Serial.print("e-Paper init ");
  if (epd.Init() != 0) {
      Serial.print("e-Paper init failed");
      return;
  }

   Serial.print("White \r\n");
   epd.Clear(white);
   delay(2000);

//   //uint8_t *BlackImage;
//    //uint16_t WIDTH, HEIGHT;
//     /* you have to edit the startup_stm32fxxx.s file and set a big enough heap size */
      Imagesize = ((EPD_WIDTH % 4 == 0)? (EPD_WIDTH / 4 ): (EPD_WIDTH / 4 + 1)) * EPD_HEIGHT;
		  Imagesize = Imagesize/4;
		Serial.print("Not enough memory, only part of the window is displayed\r\n");
    if((BlackImage = (uint8_t *)malloc(Imagesize)) == NULL) {
        Serial.print("Failed to apply for black memory...\r\n");
        return ;
    }
//     Serial.print("Paint_NewImage\r\n");
      Paint_NewImage(BlackImage, EPD_WIDTH, EPD_HEIGHT , 0, white);
      Paint_SetScale(4);
      Serial.print("SelectImage:BlackImage\r\n");
      Paint_SelectImage(BlackImage);
      //Paint_Clear(WHITE);
//      Paint_DrawString_EN(710, 60, "321", &Font24, white, black);
//      Paint_DrawString_EN(80, 200, "TAK TIK TUK", &Ebrima28, white, black);
//     delay(1000);
 
//   delay(1000);
//   Serial.print("Goto Sleep...\r\n");
//   epd.Display(BlackImage);
  //epd.Sleep();
   delay(1000);

   
}

void loop() {
   

 confirmAdding();
 delay(20000);

}


void image(){
  
    Paint_Clear(WHITE);
    delay(2000);
    Paint_DrawBitMap(Image4color);
    delay(2000);
 
 
  Serial.print("Goto Sleep...\r\n");
  epd.Display(BlackImage);
  //epd.Sleep();
   delay(1000);

}

void text(){
  // Serial.print("e-Paper init ");
  // if (epd.Init() != 0) {
  //     Serial.print("e-Paper init failed");
  //     return;
  // }

  Paint_Clear(WHITE);
  delay(2000);
 
  Paint_DrawString_EN(710, 60, "321", &Font24, white, black);
  Paint_DrawString_EN(80, 200, "TAK TIK TUK", &Ebrima28, white, black);
  delay(1000);
  Serial.print("Goto Sleep...\r\n");
  epd.Display(BlackImage);
 // epd.Sleep();
  delay(1000);

}

void confirmAdding(){
 // Serial.println("confirm Adding");
  if(WiFi.status() == WL_CONNECTED){
    HTTPClient http;    //Declare object of class HTTPClient
    //GET Data
    
    getData = "/api/Barcode/UpdateStatus"; // Add the Fingerprint ID to the Post array in order to send it
    //GET methode
    Link = URL + getData;
    
    http.begin(Link); //initiate HTTP request,
  //   Serial.println(Link);
    int httpCode = http.GET();   //Send the request
    String payload = http.getString();    //Get the response payload
    if(httpCode == 200){
      Serial.println(payload);
      delay(2000);

      if(payload == "1"){
         image();
        // delay(10000);
      }
      else if(payload == "0"){
         Serial.println("no data"); 
      }
      else if(payload == "3"){
         Serial.println("no data"); 
      }
    }
    else{
      Serial.println("Error Confirm!!");      
    }
    http.end();  //Close connection
  }
}
