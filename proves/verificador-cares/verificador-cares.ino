/*
  carrossa-leds — Verificador de cara sencera per polsador (Cara A / Cara B)
  Sketch AÏLLAT, no forma part del programa final — simula alhora el
  comportament final de cara-davant.ino I cara-darrere.ino (les 8 ales
  enceses a la vegada, cadascuna amb el seu color fix) amb un sol Arduino
  Mega i un sol polsador: cada pulsació alterna quina cara s'il·lumina
  sencera. Com que només hi ha una tira connectada cada vegada, l'usuari
  canvia el connector (Cara A <-> Cara B) entre polsades.

  Fa servir els mateixos rangs i LEDs solts confirmats a
  ../verificador-ales/ (mateixa font de veritat, cap dada duplicada a mà).

  *** SUPOSICIONS A CONFIRMAR — canvia-les si no encaixen ***
  - Pin de dades sempre el 5 (mateix pin que cara-davant.ino / cara-darrere.ino
    — corregit del 2 original).
  - Pantalla LCD 4 línies I2C (adreça 0x27) — mateix maquinari que
    ../comptador-polsador/ i ../verificador-ales/.
  - Polsador entre PIN_POLSADOR i GND, pull-up intern — igual que als
    altres sketches de proves/.
*/

#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

constexpr uint8_t PIN_DADES = 5;  // corregit de 2 a 5 — pin real del cablejat

constexpr uint8_t PIN_POLSADOR = 4;
constexpr unsigned long RETARD_REBOT_MS = 40;

constexpr uint8_t LCD_ADRECA = 0x27;
constexpr uint8_t LCD_COLUMNES = 20;
constexpr uint8_t LCD_FILES = 4;

// Mida per la tira mes gran de les dues (Cara B, 868); quan hi ha
// connectada la tira de la Cara A (853) simplement no s'adrecen els
// ultims indexs.
constexpr uint16_t NUM_LEDS = 868;

Adafruit_NeoPixel pixels(NUM_LEDS, PIN_DADES, NEO_GRB + NEO_KHZ800);
LiquidCrystal_I2C lcd(LCD_ADRECA, LCD_COLUMNES, LCD_FILES);

constexpr uint8_t CARA_A = 0;
constexpr uint8_t CARA_B = 1;
constexpr uint8_t NUM_ALES = 8;

// Mateixos colors i mateix ordre que cara-davant.ino / cara-darrere.ino.
const uint32_t PLOMA_COLOR[NUM_ALES] = {
    pixels.Color(255, 215, 0),    // 1 Groc
    pixels.Color(255, 70, 0),     // 2 Taronja
    pixels.Color(255, 0, 0),      // 3 Vermell
    pixels.Color(40, 10, 255),    // 4 Violeta
    pixels.Color(25, 0, 255),     // 5 Indi
    pixels.Color(0, 0, 139),      // 6 Blau fosc
    pixels.Color(0, 191, 255),    // 7 Blau clar
    pixels.Color(0, 255, 0),      // 8 Verd
};

struct Rang {
  uint16_t inici;  // 1-based, tal com es va comptar a ma
  uint16_t fi;
};

// Rang principal (seqüencial) de cada ala, índex [ala - 1]. Confirmat per
// l'usuari — Cara B ala 8 arriba fins al 868 (no 866, correcció posterior
// a la primera prova amb ../verificador-ales/).
const Rang RANGS_A[NUM_ALES] = {
    {1, 105}, {106, 210}, {211, 312}, {313, 412},
    {424, 528}, {529, 638}, {639, 745}, {746, 853},
};
const Rang RANGS_B[NUM_ALES] = {
    {1, 110}, {111, 216}, {217, 327}, {328, 436},
    {440, 543}, {544, 647}, {648, 754}, {755, 868},
};

struct Solt {
  uint8_t cara;
  uint8_t ala;      // 1-based
  uint16_t led;     // 1-based dins la tira d'aquesta cara
};

// LEDs que cauen fora del rang seqüencial de la seva ala (donen llum a
// una ploma diferent d'on "toca" pel número).
const Solt SOLTS[] = {
    {CARA_A, 3, 413}, {CARA_A, 3, 414}, {CARA_A, 3, 415}, {CARA_A, 3, 416}, {CARA_A, 3, 417},
    {CARA_A, 2, 418}, {CARA_A, 2, 419},
    {CARA_A, 7, 420}, {CARA_A, 7, 421}, {CARA_A, 7, 422},
    {CARA_A, 6, 423},
    {CARA_B, 3, 437},
    {CARA_B, 2, 438}, {CARA_B, 2, 439},
};
constexpr uint8_t NUM_SOLTS = sizeof(SOLTS) / sizeof(SOLTS[0]);

uint8_t caraActual = CARA_A;
bool polsadorAnterior = HIGH;

void encendreCara(uint8_t cara) {
  pixels.clear();

  const Rang *rangs = (cara == CARA_A) ? RANGS_A : RANGS_B;
  for (uint8_t ala = 0; ala < NUM_ALES; ala++) {
    const Rang &r = rangs[ala];
    for (uint16_t led = r.inici; led <= r.fi; led++) {
      pixels.setPixelColor(led - 1, PLOMA_COLOR[ala]);
    }
  }
  for (uint8_t i = 0; i < NUM_SOLTS; i++) {
    if (SOLTS[i].cara == cara) {
      pixels.setPixelColor(SOLTS[i].led - 1, PLOMA_COLOR[SOLTS[i].ala - 1]);
    }
  }

  pixels.show();
}

void mostrarLCD(uint8_t cara) {
  const Rang *rangs = (cara == CARA_A) ? RANGS_A : RANGS_B;
  const uint16_t total = rangs[NUM_ALES - 1].fi;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Cara ");
  lcd.print(cara == CARA_A ? "A" : "B");
  lcd.print(" sencera");
  lcd.setCursor(0, 1);
  lcd.print("8 ales enceses");
  lcd.setCursor(0, 2);
  lcd.print("Total: ");
  lcd.print(total);
  lcd.print(" LEDs");
  lcd.setCursor(0, 3);
  lcd.print("Prem per canviar");
}

void mostrarCaraActual() {
  encendreCara(caraActual);
  mostrarLCD(caraActual);
}

void setup() {
  pixels.begin();
  pinMode(PIN_POLSADOR, INPUT_PULLUP);

  // Marge d'inicialitzacio I2C (vegeu ../comptador-polsador/ per l'explicacio
  // del bug que aixo evita).
  delay(500);
  lcd.init();
  delay(50);
  lcd.clear();
  lcd.init();
  lcd.backlight();

  mostrarCaraActual();  // encen Cara A sencera des del principi
}

void loop() {
  const bool polsadorActual = digitalRead(PIN_POLSADOR);

  if (polsadorAnterior == HIGH && polsadorActual == LOW) {
    delay(RETARD_REBOT_MS);
    if (digitalRead(PIN_POLSADOR) == LOW) {
      caraActual = (caraActual == CARA_A) ? CARA_B : CARA_A;
      mostrarCaraActual();
    }
  }
  polsadorAnterior = polsadorActual;
}
