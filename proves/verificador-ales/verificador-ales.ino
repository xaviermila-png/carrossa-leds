/*
  carrossa-leds — Verificador d'ales per polsador (Cara A + Cara B)
  Sketch AÏLLAT, no forma part del programa final — eina per confirmar que
  els números de LED de cada ala (recollits a mà, ploma a ploma, amb
  ../comptador-polsador/) són correctes: cada pulsació encén NOMÉS els
  LEDs d'una ala concreta (rang principal + LEDs solts que hi pertanyen
  encara que no siguin seqüencials), amb el color que li toca. Mires
  quina ploma s'il·lumina físicament i la compares amb el número
  d'ala/cara que mostra l'LCD.

  Seqüència de pulsacions (es repeteix en cicle):
    Cara A ala 1, ala 2, ..., ala 8, Cara B ala 1, ala 2, ..., ala 8, i
    torna a Cara A ala 1. Només hi ha UNA tira connectada cada vegada
    (sempre al pin 5): en acabar les 8 ales de la Cara A, l'usuari
    desconnecta aquella tira i connecta la de la Cara B abans de seguir
    prement — el programa no distingeix quina tira hi ha físicament
    connectada, només compta pulsacions.

  *** SUPOSICIONS A CONFIRMAR — canvia-les si no encaixen ***
  - Pin de dades sempre el 5 (mateix pin que fan servir cara-davant.ino i
    cara-darrere.ino al programa final — corregit del 2 original).
  - Pantalla LCD 4 línies I2C (adreça 0x27, vegeu LCD_ADRECA si surt en
    blanc) — mateix maquinari que ../comptador-polsador/.
  - Polsador entre PIN_POLSADOR i GND, sense resistència externa (pull-up
    intern) — igual que ../comptador-polsador/.
  - Els LEDs solts de cada ala formen un únic bloc seguit (mai dos grups
    separats dins la mateixa ala) — cert per a totes les dades actuals;
    si mai s'afegeix una ala amb solts en dos grups deslligats, la línia
    de rang de solts a l'LCD ("primer-últim") s'hauria de repensar.

  Dades (rangs i solts) confirmades per l'usuari el 2026-09-06.
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

// Mateixos colors i mateix ordre que cara-davant.ino / cara-darrere.ino,
// per ploma/ala 1-8 (compartits entre les dues cares).
const uint32_t PLOMA_COLOR[NUM_ALES] = {
    pixels.Color(255, 215, 0),    // 1 Groc
    pixels.Color(255, 70, 0),     // 2 Taronja
    pixels.Color(255, 0, 0),      // 3 Vermell
    pixels.Color(60, 0, 220),    // 4 Violeta
    pixels.Color(75, 0, 130),     // 5 Indi
    pixels.Color(0, 0, 139),      // 6 Blau fosc
    pixels.Color(0, 191, 255),    // 7 Blau clar
    pixels.Color(0, 255, 0),      // 8 Verd
};
const char *PLOMA_NOM[NUM_ALES] = {
    "Groc", "Taronja", "Vermell", "Violeta",
    "Indi", "Blau fosc", "Blau clar", "Verd",
};

struct Rang {
  uint16_t inici;  // 1-based, tal com es va comptar a ma
  uint16_t fi;
};

// Rang principal (seqüencial) de cada ala, índex [ala - 1].
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

uint8_t pas = 0;  // 0..15 -> recorre A1,A2,...,A8,B1,B2,...,B8
bool polsadorAnterior = HIGH;

const Rang &rangDe(uint8_t cara, uint8_t ala) {
  return (cara == CARA_A ? RANGS_A : RANGS_B)[ala - 1];
}

void encendreAla(uint8_t cara, uint8_t ala) {
  pixels.clear();

  const Rang &r = rangDe(cara, ala);
  const uint32_t color = PLOMA_COLOR[ala - 1];

  for (uint16_t led = r.inici; led <= r.fi; led++) {
    pixels.setPixelColor(led - 1, color);
  }
  for (uint8_t i = 0; i < NUM_SOLTS; i++) {
    if (SOLTS[i].cara == cara && SOLTS[i].ala == ala) {
      pixels.setPixelColor(SOLTS[i].led - 1, color);
    }
  }

  pixels.show();
}

void mostrarLCD(uint8_t cara, uint8_t ala) {
  const Rang &r = rangDe(cara, ala);

  uint8_t numSolts = 0;
  uint16_t primerSolt = 0, ultimSolt = 0;
  for (uint8_t i = 0; i < NUM_SOLTS; i++) {
    if (SOLTS[i].cara == cara && SOLTS[i].ala == ala) {
      if (numSolts == 0) primerSolt = SOLTS[i].led;
      ultimSolt = SOLTS[i].led;
      numSolts++;
    }
  }
  const uint16_t total = (r.fi - r.inici + 1) + numSolts;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Cara ");
  lcd.print(cara == CARA_A ? "A" : "B");
  lcd.print(" - Ala ");
  lcd.print(ala);

  lcd.setCursor(0, 1);
  lcd.print(PLOMA_NOM[ala - 1]);

  lcd.setCursor(0, 2);
  lcd.print(r.inici);
  lcd.print("-");
  lcd.print(r.fi);
  lcd.print(" (");
  lcd.print(total);
  lcd.print(")");

  lcd.setCursor(0, 3);
  if (numSolts > 0) {
    lcd.print("+solts ");
    lcd.print(primerSolt);
    if (ultimSolt != primerSolt) {
      lcd.print("-");
      lcd.print(ultimSolt);
    }
  } else {
    lcd.print("Prem per avancar");
  }
}

void avancar() {
  const uint8_t cara = (pas < NUM_ALES) ? CARA_A : CARA_B;
  const uint8_t ala = (pas < NUM_ALES) ? (pas + 1) : (pas - NUM_ALES + 1);
  encendreAla(cara, ala);
  mostrarLCD(cara, ala);
  pas = (pas + 1) % (NUM_ALES * 2);
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

  avancar();  // encen Cara A, ala 1 des del principi
}

void loop() {
  const bool polsadorActual = digitalRead(PIN_POLSADOR);

  if (polsadorAnterior == HIGH && polsadorActual == LOW) {
    delay(RETARD_REBOT_MS);
    if (digitalRead(PIN_POLSADOR) == LOW) {
      avancar();
    }
  }
  polsadorAnterior = polsadorActual;
}
