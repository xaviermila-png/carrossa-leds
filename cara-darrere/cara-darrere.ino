/*
  carrossa-leds — Cara DARRERE
  Il·luminació estàtica del logo de l'associació "INDEPENDENTS" (Carnaval
  de Sitges) amb una tira de LEDs digitals WS2812B/NeoPixel.

  Programa ESTÀTIC: setup() encén les 8 plomes del logo, cadascuna amb un
  color fix, i loop() es queda buit — no hi ha cap efecte ni commutació
  (substitueix Control_de_Tires_de_LED_LOGO_01.ino, a ../reference/, que
  tenia diversos bugs — vegeu el README).

  Confirmat per l'usuari: les 8 plomes van totes SEGUIDES (sense cap LED
  apagat entre mig), en el mateix ordre que la llista de colors més avall.
  Aquest sketch parteix del mateix disseny que el de la cara del davant,
  però és un fitxer independent — les dues cares NO es donen per
  simètriques (poden tenir un nombre de LEDs per ploma diferent), així
  que cadascuna té el seu propi PLOMA_LEDS[] i es pot ajustar sense
  afectar l'altra.

  *** ÚNIC VALOR PENDENT DE CONFIRMAR: PLOMA_LEDS més avall ***
  No es coneix encara el nombre exacte de LEDs de cada ploma — de moment
  totes 9 (placeholder, el mateix que feia servir el programa original).
  Per ajustar-ho: puja el programa, mira on cau realment el tall entre
  cada color a la tira física, i corregeix els 8 números de PLOMA_LEDS[]
  (poden ser diferents entre plomes) fins que cada tall caigui just al
  final de la ploma corresponent — no cal tocar res més del fitxer.
*/

#include <Adafruit_NeoPixel.h>

// Pin de dades net — l'original feia servir el mateix pin 2 per la tira
// I per un botó fantasma (mai declarat) que ja no existeix en aquest
// programa; sense aquell conflicte, el pin 2 torna a ser vàlid tal com
// ja estava cablejat.
constexpr uint8_t PIN_DADES = 2;
constexpr uint16_t NUM_LEDS = 240;

Adafruit_NeoPixel pixels(NUM_LEDS, PIN_DADES, NEO_GRB + NEO_KHZ800);

// Colors del logo, en l'ordre físic de les plomes — definits un sol cop
// (l'original els recalculava a cada volta del loop(), inútilment, ja
// que mai canviaven).
const uint32_t COLOR_GROC = pixels.Color(255, 215, 0);
const uint32_t COLOR_TARONJA = pixels.Color(255, 70, 0);
const uint32_t COLOR_VERMELL = pixels.Color(255, 0, 0);
const uint32_t COLOR_MAGENTA = pixels.Color(250, 0, 250);    // "lila1" al programa original
const uint32_t COLOR_VERD = pixels.Color(0, 255, 0);
const uint32_t COLOR_BLAU_CLAR = pixels.Color(0, 191, 255);
const uint32_t COLOR_BLAU_FOSC = pixels.Color(0, 0, 139);
const uint32_t COLOR_INDI = pixels.Color(75, 0, 130);    // "lila2" al programa original

constexpr uint8_t NUM_PLOMES = 8;

const uint32_t PLOMA_COLOR[NUM_PLOMES] = {
    COLOR_GROC, COLOR_TARONJA, COLOR_VERMELL, COLOR_MAGENTA,
    COLOR_VERD, COLOR_BLAU_CLAR, COLOR_BLAU_FOSC, COLOR_INDI,
};

// PLACEHOLDER — vegeu la nota de capçalera del fitxer.
const uint16_t PLOMA_LEDS[NUM_PLOMES] = {9, 9, 9, 9, 9, 9, 9, 9};

void setup() {
  pixels.begin();
  pixels.clear();

  // Les plomes van seguides: el LED on comença la ploma p és la suma dels
  // LEDs de totes les plomes anteriors.
  uint16_t idx = 0;
  for (uint8_t p = 0; p < NUM_PLOMES; p++) {
    for (uint16_t n = 0; n < PLOMA_LEDS[p]; n++) {
      pixels.setPixelColor(idx, PLOMA_COLOR[p]);
      idx++;
    }
  }
  pixels.show();
}

void loop() {
  // Programa estàtic: no cal fer res aquí.
}
