// --- INTERNET FRAME project ---
//
// include the library code:
#include <SPI.h>
#include <Ethernet.h>
#include <SoftwareSerial.h>
#include <MsTimer2.h>
#include <serLCD.h>

//SERLCD setup

#define txPin 2
serLCD lcd(txPin);
//SoftwareSerial lcd = SoftwareSerial(0, txPin);

const int LCDdelay=100; 

int pinMo = 7;
int pinBtn=8;

byte mac[]    = {  0x90, 0xA2, 0xDA, 0x00, 0x41, 0xA9 };
byte ip[]     = { 10,1,1,88 };
IPAddress server (10,1,1,8 );
EthernetClient client;
int stage=0;
int category=0;
int discos = 0;
char *text;
int lineNum;
int col;

// wbp: goto with row & column
void goTo(int row, int col) {
  lcd.write(0xFE);   //command flag
  lcd.write((col + row * 64 + 128));    //position 
  lcd.write(0xFE);   //command flag
  lcd.write(0x0C);   //command flag  
  delay(LCDdelay);
}
void clearLCD(){
  lineNum=0;
  col=1;
  lcd.write(0xFE);   //command flag
  lcd.write(0x01);   //clear command.
  goTo(0,0);
  delay(LCDdelay);
}
void backlightOn() {  //turns on the backlight
  lcd.write(0x7C);   //command flag for backlight stuff
  lcd.write(157);    //light level.
  delay(LCDdelay);
}
void backlightOff(){  //turns off the backlight
  lcd.write(0x7C);   //command flag for backlight stuff
  lcd.write(128);     //light level for off.
   delay(LCDdelay);
}

void setup() {
  Serial.begin(9600);
  pinMode(txPin, OUTPUT);
  lcd.begin(9600);
  backlightOn();

  clearLCD();
  lcd.print("Internet frame      ");
  //goTo(1,0);
  lcd.print("v 3.0               ");
  //goTo(2,0);
  lcd.print("Starting...         ");

  pinMode(pinMo, INPUT);
  stage=-1;
  Ethernet.begin(mac, ip);
  delay(1000);
 
 getRead();

}

boolean webconnect(){
 if (client.connect(server, 80)) {
    //lcd.print("Loading... ");
    // Make a HTTP request:
    //client.println("GET /box/?s="+stage);
    client.print("GET /box/?s=");
    client.print(stage);
    client.print("&category=");
    client.print(category);
    client.print(" HTTP/1.1");
    //client.print(stage);
    client.println();
    client.println("Host: c00p.dynalias.org");
    client.println("User-Agent: arduino/lcdbox");
    client.println();
    delay(350); 
    return true;
  }else {
    // if you didn't get a connection to the server:
    goTo(0,0);
    lcd.print("[ERR] Connection");
    //Serial.println("could not connect");
    delay(3000);
    webconnect();
    //return false;
  }

}

void loop() {

 /*try to connect
    can connect? 
    get web page
    wait some time
    repeat


  */  
}

void getRead(){ 

  lineNum=0;
    col=1;
    //MsTimer2::set(0,0);
    //MsTimer2::stop();

    stage++;
    if(stage>10){   
        stage=0; 
    }
    Serial.println("stage=");
    Serial.print(stage);
    //char c;
    int charNum=0; 
    int16_t c;
    if(webconnect()==true){

      readPastHeader(&client);

      client.read();
      //char fulltext;
 
      while (client.available()){
            clearLCD();
      while (c = client.read()) {
        
      if(c > 0){
        charNum++;
        Serial.print((char)c);
        lcd.print((char)c);
        col++;
      }
      
      if(col>20){
        col=1;
        lineNum++; 
        //Serial.println("new row");
      }
      
      if (digitalRead(pinBtn)==HIGH){  
        //readUntilPipe(&client);
        client.stop();
        clearLCD(); 
        delay(100);
        category++;
        if(category>4) category=0;
       getRead();  
     } 

     //this one is done.
     if(c==-1){
        client.stop();
        delay(3000);
        getRead();
     }
     
     //clear screen to print more stuff
      if(lineNum > 3 && c > 0){
         delay(3000);
       clearLCD();
      }

      
       //fulltext=+c;
       //goTo(lineNumber, lineCount);
       delay(25);
      }//end read. EOF stuff here:

       } //end while client avail
       //clearLCD();
     client.flush();
    }//end if webconnect=true
    //client.stop();
    //delay(4000);
    //getRead();

    //MsTimer2::set(3000, getRead); // 500ms period
    //MsTimer2::start();
}

//read in information bilp
//is blip past 80 chars? 
bool readUntilPipe(Client *pClient){
  bool foundPipe = false;
  while(false){
      if (pClient->available()) {
        int16_t c = pClient->read();
        
        if(c!=124 && foundPipe!=true){
            c = pClient->read();
           // Serial.print((char)c);
            return false;
        }else if(c==124){
          foundPipe = true;
           return true;
        }
        
      if(c== 124){
        foundPipe=true;
      }else{
        foundPipe=false;
      }
      }
      
  }

}

bool readPastHeader(Client *pClient)
{
  bool bIsBlank = true;
  while(true)
  {
    if (pClient->available()) 
    {
      char c = pClient->read();
      if(c=='\r' && bIsBlank)
      {
        // throw away the /n
        c = pClient->read();
        return true;
      }
      if(c=='\n')
        bIsBlank = true;
      else if(c!='\r')
        bIsBlank = false;
      }
   }
}

