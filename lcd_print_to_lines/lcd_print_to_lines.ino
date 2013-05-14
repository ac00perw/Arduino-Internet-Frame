// --- INTERNET FRAME project ---
//
// include the library code:
#include <SPI.h>
#include <Ethernet.h>
#include <SoftwareSerial.h>
#include <MsTimer2.h>
#include <serLCD.h>

serLCD lcd(2);
//SoftwareSerial lcd = SoftwareSerial(0, txPin);

const int LCDdelay=100; 

int pinMo = 7;
int pinBtn= 8;
char* line="Put a bird on it tumblr quis qui before they sold out keffiyeh, art party in PBR anim aliquip butcher irony freegan selvage. Banksy vero exercitation, authentic lomo id synth PBR. Pickled consectetur retro, accusamus sed cray anim portland beard flexitarian cosby sweater aliquip mlkshk tattooed. Anim ethical hoodie, wayfarers carles farm-to-table chillwave incididunt shoreditch craft beer. Magna fingerstache letterpress iphone, carles 3 wolf moon odd future dolor chambray. Whatever mustache keytar eiusmod non aliqua cupidatat, williamsburg photo booth occaecat vero. Etsy scenester vegan, flexitarian nihil eu reprehenderit.";


void setup() {
  Serial.begin(9600);
    for(char * cp = line; *cp; ++cp){
     int ip= (int)*cp;
    lcd.print(*cp);
    lcd.print(ip);
    Serial.println(ip);
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
