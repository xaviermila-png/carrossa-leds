#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>
#include <avr/power.h>

//Pin de dades
#define PIN 2
//Numero de leds que te la nostra cinta
#define NUMPIXELS      240

//Informem al sistema
//El tercer parametre es el mateix per aquest tipus de cintes de leds
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


void setup() {
// Inicialitzem la cinta de leds
  pixels.begin();


  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  pinMode(2,INPUT_PULLUP);  // internal pull-up resistor
  attachInterrupt (digitalPinToInterrupt (BUTTON), changeEffect, CHANGE); // pressed
}

void loop() {

  //Definim les variables de color
  // pixels.Color te els valors RGB, des de 0,0,0 fins a 255,255,255
   uint32_t vermell = pixels.Color(255,0,0);
   uint32_t verd = pixels.Color(0,255,0);
   uint32_t blauclar = pixels.Color(0,191,255);
   uint32_t blaufosc = pixels.Color(0,0,139);
   uint32_t groc = pixels.Color(255,215,0);
   uint32_t lila1 = pixels.Color(250,0,250);
   uint32_t lila2 = pixels.Color(75,0,130);
   uint32_t taronja = pixels.Color(255,70,0);
   uint32_t blanc = pixels.Color(250,250,250);
   uint32_t nul = pixels.Color(0,0,0);

  int delayval = 50; // Pausa de 50 milisegons

  // ences tot
  for (int x=0;x<10;x++){
    for(int i=0;i<9;i++){
      pixels.setPixelColor(i, groc);
      }
    pixels.show();
    for(int i=10;i<19;i++){
      pixels.setPixelColor(i, taronja);
      }
    pixels.show();
    for(int i=20;i<29;i++){
      pixels.setPixelColor(i, vermell);
      }
    pixels.show();
    for(int i=30;i<39;i++){
      pixels.setPixelColor(i, lila1);
      }
    pixels.show();
    for(int i=40;i<49;i++){
      pixels.setPixelColor(i, verd);
      }
    pixels.show();
    for(int i=50;i<59;i++){
      pixels.setPixelColor(i, blauclar);
      }
    pixels.show();
    for(int i=60;i<69;i++){
      pixels.setPixelColor(i, blaufosc);
      }
    pixels.show();
    for(int i=70;i<79;i++){
      pixels.setPixelColor(i, lila2);
      }
    pixels.show();
    delay(1000);
   }

       delay(1000);  // Pausa


}
void rainbowCycle(int SpeedDelay) {
  byte *c;
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< NUMPIXELS; i++) {
      c=Wheel(((i * 256 / NUMPIXELS) + j) & 255);
      setPixel(i, *c, *(c+1), *(c+2));
    }
    showStrip();
    delay(SpeedDelay);
  }
}
// used by rainbowCycle and theaterChaseRainbow
byte * Wheel(byte WheelPos) {
  static byte c[3];

  if(WheelPos < 85) {
   c[0]=WheelPos * 3;
   c[1]=255 - WheelPos * 3;
   c[2]=0;
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   c[0]=255 - WheelPos * 3;
   c[1]=0;
   c[2]=WheelPos * 3;
  } else {
   WheelPos -= 170;
   c[0]=0;
   c[1]=WheelPos * 3;
   c[2]=255 - WheelPos * 3;
  }

  return c;
}
// Set a LED color (not yet visible)
void setPixel(int Pixel, byte red, byte green, byte blue) {
 #ifdef ADAFRUIT_NEOPIXEL_H
   // NeoPixel
   strip.setPixelColor(Pixel, strip.Color(red, green, blue));
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
 #endif
}
// Apply LED color changes
void showStrip() {
 #ifdef ADAFRUIT_NEOPIXEL_H
   // NeoPixel
   strip.show();
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   FastLED.show();
 #endif
}
