/*
  carrossa-leds — Cara DARRERE
  Il·luminació estàtica del logo de l'associació "INDEPENDENTS" (Carnaval
  de Sitges) amb una tira de LEDs digitals WS2812B/NeoPixel.

  Programa ESTÀTIC: setup() encén les 8 plomes del logo, cadascuna amb un
  color fix, i loop() es queda buit — no hi ha cap efecte ni commutació
  (substitueix Control_de_Tires_de_LED_LOGO_01.ino, a ../reference/, que
  tenia diversos bugs — vegeu el README).

  *** VALORS PENDENTS DE CONFIRMAR (marcats PLACEHOLDER més avall) ***
  Els trams [inici, fi] de cada ploma s'han extret dels índexs que feia
  servir el programa original, NO són mesures reals confirmades:
    - Nombre exacte de LEDs de cada ploma (poden no ser tots iguals).
    - Si hi ha LEDs apagats entre ploma i ploma, o van totes seguides
      (l'original tenia un buit d'1 LED entre plomes — es manté aquí).
    - Si l'ordre físic del cablejat coincideix amb l'ordre de colors de
      l'original.
    - Si aquesta cara (darrere) fa servir els mateixos 8 colors/ordre que
      la de davant — de moment són idèntics als dos fitxers.
*/

#include <Adafruit_NeoPixel.h>

// Pin de dades net — l'original feia servir el mateix pin 2 per la tira
// I per un botó fantasma (mai declarat) que ja no existeix en aquest
// programa; sense aquell conflicte, el pin 2 torna a ser vàlid tal com
// ja estava cablejat.
constexpr uint8_t PIN_DADES = 2;
constexpr uint16_t NUM_LEDS = 240;

Adafruit_NeoPixel pixels(NUM_LEDS, PIN_DADES, NEO_GRB + NEO_KHZ800);

// Colors del logo — definits un sol cop (l'original els recalculava a
// cada volta del loop(), inútilment, ja que mai canviaven).
const uint32_t COLOR_GROC = pixels.Color(255, 215, 0);
const uint32_t COLOR_TARONJA = pixels.Color(255, 70, 0);
const uint32_t COLOR_VERMELL = pixels.Color(255, 0, 0);
const uint32_t COLOR_MAGENTA = pixels.Color(250, 0, 250);    // "lila1" al programa original
const uint32_t COLOR_VERD = pixels.Color(0, 255, 0);
const uint32_t COLOR_BLAU_CLAR = pixels.Color(0, 191, 255);
const uint32_t COLOR_BLAU_FOSC = pixels.Color(0, 0, 139);
const uint32_t COLOR_INDI = pixels.Color(75, 0, 130);    // "lila2" al programa original

struct Ploma {
  uint16_t inici;  // primer índex de LED (inclòs)
  uint16_t fi;     // darrer índex de LED (inclòs)
  uint32_t color;
};

// PLACEHOLDER: 8 plomes de 9 LEDs cadascuna, amb 1 LED de buit entre
// ploma i ploma — exactament els trams que feia servir el programa
// original. Ajustar amb els valors reals un cop confirmats.
const Ploma PLOMES[] = {
    {0, 8, COLOR_GROC},
    {10, 18, COLOR_TARONJA},
    {20, 28, COLOR_VERMELL},
    {30, 38, COLOR_MAGENTA},
    {40, 48, COLOR_VERD},
    {50, 58, COLOR_BLAU_CLAR},
    {60, 68, COLOR_BLAU_FOSC},
    {70, 78, COLOR_INDI},
};
constexpr uint8_t NUM_PLOMES = sizeof(PLOMES) / sizeof(PLOMES[0]);

void setup() {
  pixels.begin();
  pixels.clear();

  for (uint8_t p = 0; p < NUM_PLOMES; p++) {
    for (uint16_t i = PLOMES[p].inici; i <= PLOMES[p].fi; i++) {
      pixels.setPixelColor(i, PLOMES[p].color);
    }
  }
  pixels.show();
}

void loop() {
  // Programa estàtic: no cal fer res aquí.
}
