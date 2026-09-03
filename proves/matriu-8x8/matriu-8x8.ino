/*
  carrossa-leds — Prova amb matriu de LEDs digitals 8x8
  Sketch AÏLLAT, no forma part del programa final de la carrossa — només
  per verificar en un Arduino Mega de proves que el cablejat i el codi
  base (Adafruit_NeoPixel) funcionen abans de connectar-hi les tires
  reals de les plomes.

  Suposa que la matriu és WS2812B/NeoPixel (mateixa família que la resta
  del projecte) — si en canvi és d'un altre tipus (p. ex. MAX7219, que
  fa servir un altre protocol i més d'un pin), digues-m'ho i ho adapto.

  Pin de control: GPIO 2 — el mateix pin de dades que ja fa servir la
  resta del projecte (cara-davant/cara-darrere a `main`), perquè aquesta
  prova assagi exactament el mateix cablejat que s'usarà de veritat.

  Comportament: encén cada LED (blanc) un darrere l'altre, l'apaga abans
  de passar al següent, i torna a començar indefinidament — mai hi ha
  més d'un LED encès alhora, així que no cal alimentació externa per
  aquesta prova (el propi USB de l'Arduino ja n'hi ha de sobres).
*/

#include <Adafruit_NeoPixel.h>

constexpr uint8_t PIN_DADES = 2;
constexpr uint16_t NUM_LEDS = 64;  // matriu 8x8
constexpr uint16_t RETARD_MS = 150;

Adafruit_NeoPixel pixels(NUM_LEDS, PIN_DADES, NEO_GRB + NEO_KHZ800);
const uint32_t COLOR_PROVA = pixels.Color(255, 255, 255);  // blanc

void setup() {
  pixels.begin();
  pixels.clear();
  pixels.show();
}

void loop() {
  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    pixels.setPixelColor(i, COLOR_PROVA);
    pixels.show();
    delay(RETARD_MS);
    pixels.setPixelColor(i, 0);  // apaga abans de passar al següent
    pixels.show();
  }
}
