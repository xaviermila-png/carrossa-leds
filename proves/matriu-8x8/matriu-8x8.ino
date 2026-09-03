/*
  carrossa-leds — Prova amb matriu de LEDs digitals 8x8
  Sketch AÏLLAT, no forma part del programa final de la carrossa —
  simulador visual del logo en petit: cada FILA de la matriu representa
  una ploma, encesa amb el mateix color que li correspon al programa
  real (cara-davant/cara-darrere a `main`).

  Pin de control: GPIO 2 — el mateix pin de dades que ja fa servir la
  resta del projecte.

  Nota sobre el cablejat de la matriu (serpentina o no): com que cada
  fila s'encén sencera amb un sol color, no importa si dins la fila els
  LEDs van d'esquerra a dreta o al revés (típic de les matrius en
  serpentina) — només cal que els 8 índexs d'una fila coincideixin amb
  els 8 LEDs físics d'aquella fila, cosa que ja passa amb la numeració
  habitual (fila 0 = índexs 0-7, fila 1 = índexs 8-15, etc.).

  Programa ESTÀTIC (setup() ho encén tot i prou), com el programa final.
*/

#include <Adafruit_NeoPixel.h>

constexpr uint8_t PIN_DADES = 2;
constexpr uint8_t COLUMNES = 8;
constexpr uint8_t FILES = 8;
constexpr uint16_t NUM_LEDS = COLUMNES * FILES;  // matriu 8x8

Adafruit_NeoPixel pixels(NUM_LEDS, PIN_DADES, NEO_GRB + NEO_KHZ800);

// Mateixos colors, en el mateix ordre de plomes, que cara-davant/cara-darrere.
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

void setup() {
  pixels.begin();
  pixels.clear();

  for (uint8_t fila = 0; fila < FILES; fila++) {
    for (uint8_t col = 0; col < COLUMNES; col++) {
      pixels.setPixelColor(fila * COLUMNES + col, PLOMA_COLOR[fila]);
    }
  }
  pixels.show();
}

void loop() {
  // Simulació estàtica: no cal fer res aquí.
}
