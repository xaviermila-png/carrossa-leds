/*
  carrossa-leds — Un sol Arduino, dues cares (BRANCA ALTERNATIVA)
  Il·luminació estàtica del logo de l'associació "INDEPENDENTS" (Carnaval
  de Sitges) amb dues tires de LEDs digitals WS2812B/NeoPixel, totes dues
  controlades des d'UN sol Arduino Mega 2560 (a diferència de la branca
  `main`, que en fa servir un per cara).

  Cada cara té el seu propi pin de dades independent — PIN_DAVANT / PIN_
  DARRERE — així que si algun dia una tira falla, només cal desconnectar-
  la i endollar-hi la de recanvi al pin corresponent; no cal tocar codi
  ni la placa.

  Programa ESTÀTIC: setup() encén les 8 plomes de cada cara i prou, loop()
  es queda buit — no hi ha cap efecte ni commutació. Mateix disseny de
  plomes/colors que la branca `main` (vegeu-hi el README per l'origen
  d'aquests valors i els bugs que corregeix respecte al programa
  original a ../reference/).

  *** ÚNIC VALOR PENDENT DE CONFIRMAR: PLOMA_LEDS_DAVANT/DARRERE més avall ***
  No es coneix encara el nombre exacte de LEDs de cada ploma — de moment
  totes 9 (placeholder). Les dues cares NO es donen per simètriques, així
  que cadascuna té el seu propi array — ajustar-ne un no afecta l'altre.
  Per ajustar-ho: puja el programa, mira on cau realment el tall entre
  cada color a la tira física, i corregeix els 8 números corresponents
  (poden ser diferents entre plomes, i entre cares) fins que cada tall
  caigui just al final de la ploma corresponent.
*/

#include <Adafruit_NeoPixel.h>

// Un pin de dades independent per cara — si una tira falla, es
// desconnecta i s'hi endolla la de recanvi al pin corresponent, sense
// tocar-hi res més.
constexpr uint8_t PIN_DAVANT = 2;
constexpr uint8_t PIN_DARRERE = 3;
constexpr uint16_t NUM_LEDS = 240;

Adafruit_NeoPixel tiraDavant(NUM_LEDS, PIN_DAVANT, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel tiraDarrere(NUM_LEDS, PIN_DARRERE, NEO_GRB + NEO_KHZ800);

// Colors del logo, en l'ordre físic de les plomes — definits un sol cop.
// pixels.Color() només empaqueta els 3 bytes RGB, no depèn de begin(),
// així que és igual fer-ho servir sobre qualsevol de les dues tires.
const uint32_t COLOR_GROC = tiraDavant.Color(255, 215, 0);
const uint32_t COLOR_TARONJA = tiraDavant.Color(255, 70, 0);
const uint32_t COLOR_VERMELL = tiraDavant.Color(255, 0, 0);
const uint32_t COLOR_MAGENTA = tiraDavant.Color(250, 0, 250);    // "lila1" al programa original
const uint32_t COLOR_VERD = tiraDavant.Color(0, 255, 0);
const uint32_t COLOR_BLAU_CLAR = tiraDavant.Color(0, 191, 255);
const uint32_t COLOR_BLAU_FOSC = tiraDavant.Color(0, 0, 139);
const uint32_t COLOR_INDI = tiraDavant.Color(75, 0, 130);    // "lila2" al programa original

constexpr uint8_t NUM_PLOMES = 8;

const uint32_t PLOMA_COLOR[NUM_PLOMES] = {
    COLOR_GROC, COLOR_TARONJA, COLOR_VERMELL, COLOR_MAGENTA,
    COLOR_VERD, COLOR_BLAU_CLAR, COLOR_BLAU_FOSC, COLOR_INDI,
};

// PLACEHOLDER — vegeu la nota de capçalera del fitxer. Un array propi per
// cara: no es dona per fet que les dues siguin simètriques.
const uint16_t PLOMA_LEDS_DAVANT[NUM_PLOMES] = {9, 9, 9, 9, 9, 9, 9, 9};
const uint16_t PLOMA_LEDS_DARRERE[NUM_PLOMES] = {9, 9, 9, 9, 9, 9, 9, 9};

// Encén les 8 plomes (seguides, sense buit) sobre la tira indicada, amb
// els comptatges de LEDs propis d'aquesta cara — compartit entre les
// dues cares perquè el PATRÓ (colors i lògica) és idèntic, només canvien
// quin objecte Adafruit_NeoPixel (i per tant quin pin) rep les ordres i
// quants LEDs té cada ploma.
void encendrePlomes(Adafruit_NeoPixel &tira, const uint16_t plomaLeds[NUM_PLOMES]) {
  tira.begin();
  tira.clear();

  uint16_t idx = 0;
  for (uint8_t p = 0; p < NUM_PLOMES; p++) {
    for (uint16_t n = 0; n < plomaLeds[p]; n++) {
      tira.setPixelColor(idx, PLOMA_COLOR[p]);
      idx++;
    }
  }
  tira.show();
}

void setup() {
  encendrePlomes(tiraDavant, PLOMA_LEDS_DAVANT);
  encendrePlomes(tiraDarrere, PLOMA_LEDS_DARRERE);
}

void loop() {
  // Programa estàtic: no cal fer res aquí.
}
