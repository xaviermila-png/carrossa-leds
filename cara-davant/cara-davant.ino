/*
  carrossa-leds — Cara DAVANT (Cara A)
  Programa FINAL per a l'Arduino Mega que controla la tira de LEDs
  WS2812B/NeoPixel de la cara del davant del logo de l'associació
  "INDEPENDENTS" (Carnaval de Sitges).

  És un programa ESTÀTIC: el setup() encén les 8 ales del logo, cadascuna
  amb un color fix, i el loop() no fa res — no hi ha animacions ni
  polsadors, la carrossa només s'ha d'encendre un cop i quedar-se així.

  Dades (rangs de LED, LEDs solts i colors definitius) confirmades per
  l'usuari provant amb els sketches de ../proves/verificador-ales/ i
  ../proves/verificador-cares/ — vegeu-los si cal repetir alguna prova.
*/

#include <Adafruit_NeoPixel.h>

// Pin de dades de la tira. Un únic pin: aquest Arduino només controla la
// tira de la Cara A (l'altra cara té el seu propi Arduino Mega, vegeu
// ../cara-darrere/). Corregit de 2 a 5 — el cablejat real de la carrossa
// fa servir el pin 5, no el 2 (que era el pin habitual d'altres projectes
// NeoPixel, però no d'aquest).
constexpr uint8_t PIN_DADES = 5;

// Nombre total de LEDs de la tira d'aquesta cara: el LED més alt que fem
// servir és el final de l'ala 8 (vegeu RANGS més avall). No hi ha cap
// límit "de mentida" imposat aquí — és el nombre real confirmat físicament.
constexpr uint16_t NUM_LEDS = 853;

Adafruit_NeoPixel pixels(NUM_LEDS, PIN_DADES, NEO_GRB + NEO_KHZ800);

constexpr uint8_t NUM_ALES = 8;

// Colors finals de cada ala (índex 0 = ala 1, ... índex 7 = ala 8),
// ajustats a ull sobre la tira real perquè cap parella d'ales veïnes es
// confongui. Segueixen l'ordre físic del logo: pugen en tons càlids pel
// costat esquerre (ala 1 groc, baix, fins a l'ala 4 violeta, dalt) i
// baixen en tons freds pel costat dret (ala 5 indi, dalt, fins a l'ala 8
// verd, baix) — violeta i indi es toquen a dalt de tot, tal com es veu a
// les plomes del logo.
const uint32_t PLOMA_COLOR[NUM_ALES] = {
    pixels.Color(255, 215, 0),   // 1 Groc      — baix esquerra
    pixels.Color(255, 70, 0),    // 2 Taronja
    pixels.Color(255, 0, 0),     // 3 Vermell
    pixels.Color(40, 10, 255),   // 4 Violeta   — dalt esquerra
    pixels.Color(25, 0, 255),    // 5 Indi      — dalt dreta
    pixels.Color(0, 0, 139),     // 6 Blau fosc
    pixels.Color(0, 191, 255),   // 7 Blau clar
    pixels.Color(0, 255, 0),     // 8 Verd      — baix dreta
};

// Un rang de LEDs "seqüencial": del primer al darrer LED d'una ala,
// comptats tal com surten un darrere l'altre a la tira física.
struct Rang {
  uint16_t inici;  // número de LED 1-based (el primer LED de la tira és l'1, no el 0)
  uint16_t fi;
};

// Rang principal de cada ala de la Cara A, índex [ala - 1]. Confirmat a
// mà, ala per ala, amb ../proves/comptador-polsador/.
const Rang RANGS[NUM_ALES] = {
    {1, 105},    // ala 1
    {106, 210},  // ala 2
    {211, 312},  // ala 3
    {313, 412},  // ala 4
    {424, 528},  // ala 5
    {529, 638},  // ala 6
    {639, 745},  // ala 7
    {746, 853},  // ala 8
};

// Un LED "solt": dona llum a una ala diferent de la que li tocaria pel
// seu número (per exemple, per com passa el cablejat físicament entre
// dues ales veïnes). S'encén amb el color de "ala", no amb el de l'ala on
// li tocaria pel seu número de LED.
struct Solt {
  uint8_t ala;   // 1-based — l'ala a la qual pertany de veritat
  uint16_t led;  // 1-based — número de LED a la tira
};

// LEDs solts de la Cara A: cauen al buit entre el final de l'ala 4 (412)
// i el començament de l'ala 5 (424), però no pertanyen a cap d'aquestes
// dues ales.
const Solt SOLTS[] = {
    {3, 413}, {3, 414}, {3, 415}, {3, 416}, {3, 417},  // pertanyen a l'ala 3
    {2, 418}, {2, 419},                                // pertanyen a l'ala 2
    {7, 420}, {7, 421}, {7, 422},                      // pertanyen a l'ala 7
    {6, 423},                                          // pertany a l'ala 6
};
constexpr uint8_t NUM_SOLTS = sizeof(SOLTS) / sizeof(SOLTS[0]);

void setup() {
  pixels.begin();  // inicialitza la llibreria i reserva el buffer intern per als NUM_LEDS declarats
  pixels.clear();  // tots els LEDs apagats (negre) fins que els encenem explícitament

  // Recorre les 8 ales; per cadascuna, pinta tots els LEDs del seu rang
  // principal amb el color que li toca. Els números de LED que fem
  // servir a RANGS són 1-based (el primer LED físic és l'1), però
  // setPixelColor() de la llibreria espera un índex 0-based (el primer
  // LED és el 0) — per això sempre restem 1.
  for (uint8_t ala = 0; ala < NUM_ALES; ala++) {
    const Rang &r = RANGS[ala];
    for (uint16_t led = r.inici; led <= r.fi; led++) {
      pixels.setPixelColor(led - 1, PLOMA_COLOR[ala]);
    }
  }

  // Un cop pintats tots els rangs principals, sobreescrivim els LEDs
  // solts amb el color de l'ala a la qual pertanyen de veritat (no la
  // seva posició numèrica).
  for (uint8_t i = 0; i < NUM_SOLTS; i++) {
    pixels.setPixelColor(SOLTS[i].led - 1, PLOMA_COLOR[SOLTS[i].ala - 1]);
  }

  pixels.show();  // envia tot el buffer a la tira — fins aquí no s'encén cap LED de veritat
}

void loop() {
  // Programa estàtic: un cop encesa la carrossa a setup(), no cal fer
  // res més — sense animacions ni polsadors.
}
