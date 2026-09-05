/*
  carrossa-leds — Comptador de LEDs amb polsador + pantalla LCD
  Sketch AÏLLAT, no forma part del programa final — eina per esbrinar el
  nombre exacte de LEDs de cada ala/ploma: encén un LED de la tira, i
  avança al següent NOMÉS quan es prem un polsador extern (mai sol,
  sense polsador no passa res). Mentre vas prement, mires a quin LED
  visualment cau el tall de color entre plomes i llegeixes el número a
  la pantalla LCD per anotar-ho.

  *** SUPOSICIONS A CONFIRMAR — canvia-les si no encaixen ***
  - Pantalla LCD de 4 línies amb mòdul I2C (adreça 0x27, la més habitual
    — si la teva surt en blanc, prova 0x3F a LCD_ADRECA més avall, és
    l'altra adreça freqüent d'aquests mòduls).
  - Polsador connectat entre el pin PIN_POLSADOR i GND, sense cap
    resistència externa (es fa servir la resistència de pull-up interna
    de l'Arduino) — en repòs llegeix HIGH, en prémer llegeix LOW.
  - Tira NeoPixel al pin de dades habitual del projecte (pin 2).

  Consum: mai hi ha més d'un LED encès alhora, així que no cal
  alimentació externa per aquesta prova concreta (a diferència de la
  simulació de matriu completa, que sí que ho necessita — vegeu
  ../matriu-8x8/).
*/

#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

constexpr uint8_t PIN_DADES = 2;
// Un "sense límit" de veritat no és possible (l'Arduino Mega només té
// 8 KB de RAM, i Adafruit_NeoPixel en reserva 3 bytes per LED per poder
// arribar-hi amb el senyal) — però 1000 hi cap còmodament (3000 bytes,
// deixant més de 5 KB lliures per a la resta del programa i la pantalla
// LCD) i és molt més que qualsevol tira real d'aquest projecte, així que
// mai tornarà a la volta abans d'arribar al final de veritat. Baixa-ho a
// 64 si proves amb la matriu 8x8; si algun dia calgués encara més,
// puja-ho amb marge (per exemple 2000 = 6000 bytes) i comprova que
// segueix funcionant bé (símptoma de quedar-se sense RAM: la pantalla
// LCD tornant a mostrar caràcters estranys, com el bug que ja vam
// solucionar).
constexpr uint16_t NUM_LEDS = 1000;

constexpr uint8_t PIN_POLSADOR = 4;
// Temps a esperar just després de detectar una pulsació abans de tornar
// a mirar el pin: els contactes mecànics d'un polsador "reboten" uns
// mil·lisegons en tocar-se, generant diverses transicions HIGH/LOW
// dins d'una mateixa pulsada real — sense això, un sol clic es
// comptaria com si fossin diversos.
constexpr unsigned long RETARD_REBOT_MS = 40;

constexpr uint8_t LCD_ADRECA = 0x27;
constexpr uint8_t LCD_COLUMNES = 20;
constexpr uint8_t LCD_FILES = 4;

Adafruit_NeoPixel pixels(NUM_LEDS, PIN_DADES, NEO_GRB + NEO_KHZ800);
LiquidCrystal_I2C lcd(LCD_ADRECA, LCD_COLUMNES, LCD_FILES);

const uint32_t COLOR_COMPTADOR = pixels.Color(255, 255, 255);  // blanc

uint16_t ledActual = 0;         // índex 0-based del LED encès ara mateix
bool polsadorAnterior = HIGH;   // HIGH = no premut (pull-up)

void encendreLed(uint16_t index) {
  pixels.clear();
  pixels.setPixelColor(index, COLOR_COMPTADOR);
  pixels.show();
}

void mostrarComptador() {
  lcd.setCursor(0, 0);
  lcd.print("Comptador LEDs");
  lcd.setCursor(0, 1);
  lcd.print("LED actual: ");
  lcd.print(ledActual + 1);
  lcd.print("    ");  // sobreescriu restes de xifres més llargues
  lcd.setCursor(0, 3);
  lcd.print("Prem per avancar");
}

void setup() {
  pixels.begin();
  pinMode(PIN_POLSADOR, INPUT_PULLUP);

  // Marge abans de parlar amb el mòdul I2C de la pantalla — confirmat en
  // maquinari real que alguns d'aquests mòduls encara no estan del tot
  // assentats just després d'alimentar-se, i la seqüència d'inicialització
  // (molt sensible als temps, en mode 4 bits) es perd en silenci, deixant
  // la pantalla mostrant caràcters aleatoris per sempre fins a un
  // reinici. init()+clear() doblat per si el primer intent no ha arribat
  // a assentar-se del tot.
  delay(500);
  lcd.init();
  delay(50);
  lcd.clear();
  lcd.init();
  lcd.backlight();

  encendreLed(ledActual);
  mostrarComptador();
}

void loop() {
  const bool polsadorActual = digitalRead(PIN_POLSADOR);

  // Flanc de baixada (HIGH->LOW): s'acaba de prémer. El delay() +
  // segona lectura confirma que segueix premut després del possible
  // rebot, no és una lectura de soroll — no hi ha cap altra feina
  // urgent en aquest loop(), així que bloquejar-lo uns ms és inofensiu.
  if (polsadorAnterior == HIGH && polsadorActual == LOW) {
    delay(RETARD_REBOT_MS);
    if (digitalRead(PIN_POLSADOR) == LOW) {
      ledActual = (ledActual + 1) % NUM_LEDS;
      encendreLed(ledActual);
      mostrarComptador();
    }
  }
  polsadorAnterior = polsadorActual;
}
