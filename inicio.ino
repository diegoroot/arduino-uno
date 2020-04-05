#include "Adafruit_GFX.h"     
#include "Adafruit_ILI9341.h" 
#include "URTouch.h"          
#include "qrcode.h"
#include "SPI.h"

#define TFT_DC 9              
#define TFT_CS 10            
#define TFT_RST 8
#define TFT_MISO 12         
#define TFT_MOSI 11           
#define TFT_CLK 13 

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

#define t_SCK 3              
#define t_CS 4                
#define t_MOSI 5              
#define t_MISO 6             
#define t_IRQ 7     
         
String men2 = "123123";
int led = 2;

URTouch ts(t_SCK, t_CS, t_MOSI, t_MISO, t_IRQ);
QRCode qrcode;

String Key[4][3] = {
  { "7", "8", "9"  },
  { "4", "5", "6"  },
  { "1", "2", "3"  },
  { "Ba", "0", "En"}
};
String N1, N2, ShowSC, opt;
bool updata=false;
float answers=-1;

int pantalla = 1;

void setup(){
  Serial.begin(9600);
  pinMode(led,OUTPUT);
  digitalWrite(led,HIGH);
  delay(100);
  digitalWrite(led,LOW);
  tft.begin();                     
  tft.setRotation(3);
  ts.InitTouch();                   
  ts.setPrecision(PREC_LOW);           
  delay(10);
  cambiar();     
}

void loop()
{
  int x, y;                        
  
  while(ts.dataAvailable())        
  {
    ts.read();                      
    x = ts.getX();                 
    y = ts.getY();
    Serial.println(x);
    Serial.println(y);                
    if((x!=-1) && (y!=-1)){
      if( pantalla == 1 ){
        pantalla = 2;
        ShowSC ="";
        if((x>30 && x<80) && (y>24 && y<200)){
          ver_teclado();
        }
      }else{

        if(pantalla == 2 ){

        String val = validar(x,y);
        Serial.print("val = ");
        Serial.print(val);
        Serial.println();
        if(val != "Ba" && val != "En"){
          ShowSC += val;
          Serial.print("Show");
          Serial.print(ShowSC);
          Serial.println();
          actualizar(true);
          
        }else{
          if(val == "Ba"){
            ShowSC = "";
            pantalla = 1;
            cambiar();
          }else{

            if(ShowSC == men2){
              entro();
              ShowSC = "";
              cambiar();
            }else{
              no_entro();
              ShowSC = "";
              delay(300);
              ver_teclado();
            }
            
          }
          
          
        }
        
      }
      }
      

    }
      
    }
  }

  void cambiar(){
        pantalla =1;
        tft.fillScreen(ILI9341_WHITE);
        tft.setRotation(0);
        tft.setCursor(80, 5);
        tft.setTextColor(ILI9341_BLACK);
        tft.setTextSize(3);
        tft.println("UCEVA");
        tft.setTextSize(2);
        tft.setCursor(10, 55);
        tft.println("Sala A");
        tft.setCursor(10, 85);
        tft.println("08:12:49 P.M");
        //TFTscreen.fillScreen(BLACK);
        
        char str_array[men2.length()+1];
        men2.toCharArray(str_array, men2.length()+1);
        uint8_t qrcodeData[qrcode_getBufferSize(5)];
        
        
        qrcode_initText(&qrcode, qrcodeData, 3, 0,  str_array); // your text in last parameter, e.g. "Hello World"
        for (uint8_t y = 0; y < qrcode.size; y++) {
            // Each horizontal module
            for (uint8_t x = 0; x < qrcode.size; x++) {
                if(qrcode_getModule(&qrcode, x, y)) {
                    tft.fillRect(45+(x*4), 110+(y*4), 4, 4,ILI9341_BLACK);
                }
            }
        }

        tft.fillRect(35, 240, 180, 60, ILI9341_BLACK);
        tft.setTextColor(ILI9341_WHITE);
        tft.setTextSize(2);
        tft.setCursor(60,260);
        tft.println("Ver teclado");
        Serial.println("listo");
}

void ver_teclado(){
  tft.setCursor(0,0);
  tft.setRotation(0);
  tft.fillScreen(ILI9341_BLACK);
  
  tft.fillRect(0, 80, 240, 240, ILI9341_WHITE);
  tft.drawFastHLine(0, 140, 240, ILI9341_BLACK);
  tft.drawFastHLine(0, 200, 240, ILI9341_BLACK);
  tft.drawFastHLine(0, 260, 240, ILI9341_BLACK);

  tft.drawFastVLine(0, 80, 240, ILI9341_BLACK);
  tft.drawFastVLine(80, 80, 240, ILI9341_BLACK);
  tft.drawFastVLine(160, 80, 240, ILI9341_BLACK);
  tft.drawFastVLine(240-1, 80, 240, ILI9341_BLACK);

  for (int y=0;y<4;y++) {
    for (int x=0;x<3;x++) {
      tft.setCursor(35 + (70*x), 100 + (62*y));
      tft.setTextSize(3);
      tft.setTextColor(ILI9341_BLACK);
      tft.println(Key[y][x]);
    }
  }
  pantalla =2;
  
}

void actualizar(boolean updata){
    if (updata) {
    tft.fillRect(0, 0, 240, 80, ILI9341_BLACK);
  
    tft.setCursor(10, 10);
    tft.setTextSize(3);
    tft.setTextColor(ILI9341_WHITE);
    tft.println(ShowSC);
    Serial.println(ShowSC);
  }
}

void entro(){

  tft.fillScreen(ILI9341_WHITE);
  digitalWrite(led,HIGH);
  tft.setRotation(0);
  tft.setCursor(5, 150);
  tft.setTextColor(ILI9341_BLACK);
  tft.setTextSize(3);
  tft.println("Bienvenido a la sala A");
  delay(300);
  digitalWrite(led,LOW);
  
  
}

void no_entro(){

  tft.fillScreen(ILI9341_WHITE);
  tft.setRotation(0);
  tft.setCursor(5, 150);
  tft.setTextColor(ILI9341_BLACK);
  tft.setTextSize(3);
  tft.println("Contrasela incorrecta para la sala A");
  
}

String validar(int x, int y){

  return Key[3-((int)x/70)][2-((int)y/80)];
  
}
