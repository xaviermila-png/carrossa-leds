# carrossa-leds

Control de la il·luminació del logo de la carrossa de l'associació
**"INDEPENDENTS"** (Carnaval de Sitges) amb dues tires de LEDs digitals
WS2812B/NeoPixel — una per cada cara de la carrossa (davant i darrere).

És un programa **estàtic**: cada cara del logo té 8 plomes, cadascuna
il·luminada d'un color fix, sense efectes ni animacions ni cap botó físic.

## Maquinari
- 2x Arduino Mega 2560 — un per cada cara de la carrossa.
- Cada Arduino controla la seva pròpia tira NeoPixel independent: 853 LEDs
  a la Cara A (davant), 868 LEDs a la Cara B (darrere).
- Pin de dades: GPIO 5 a totes dues plaques (corregit del GPIO 2 inicial,
  que no era el pin real del cablejat).

## Estructura del projecte
- [`cara-davant/`](cara-davant/) — sketch per a l'Arduino de la cara del
  davant.
- [`cara-darrere/`](cara-darrere/) — sketch per a l'Arduino de la cara de
  darrere.
- [`reference/`](reference/) — programa original
  (`Control_de_Tires_de_LED_LOGO_01.ino`), conservat només com a
  referència històrica. Tenia diversos bugs ja corregits al codi nou:
  - Feia servir un objecte `strip` que mai s'havia declarat (no compilava).
  - El pin 2 s'usava alhora com a pin de dades i com a entrada d'un botó
    (`BUTTON`/`changeEffect`, tampoc declarats) que no existeix — aquest
    conflicte de pin era probablement la causa que una cara fes coses
    estranyes.
  - Només s'assignaven colors als índexs 0–79 (8 plomes de 9 LEDs), tot i
    declarar-ne 240 — la resta de la tira no s'encenia mai.
  - Funcions mortes (`rainbowCycle`, `Wheel`, `setPixel`, `showStrip`)
    d'un efecte arc de Sant Martí commutable per botó que mai es va acabar
    d'integrar.
  - Les variables de color es recalculaven a cada volta del `loop()` en
    lloc de definir-se un sol cop.
  - Variables no utilitzades (`delayval`, `nul`).

## Compilar i pujar (Arduino IDE o arduino-cli)

**Arduino IDE:**
1. Obre `cara-davant/cara-davant.ino` (o `cara-darrere/cara-darrere.ino`).
2. Selecciona la placa **Arduino Mega or Mega 2560**.
3. Instal·la la llibreria **Adafruit NeoPixel** (Gestor de llibreries).
4. Puja amb el botó habitual.

**arduino-cli:**
```
arduino-cli lib install "Adafruit NeoPixel"
arduino-cli compile --fqbn arduino:avr:mega cara-davant
arduino-cli upload --fqbn arduino:avr:mega -p COMx cara-davant
```
(substitueix `COMx` pel port real — `arduino-cli board list` per confirmar-lo;
mateixos passos per `cara-darrere`).

## Mapa de LEDs per ala

Confirmat físicament amb ../proves/comptador-polsador/,
../proves/verificador-ales/ i ../proves/verificador-cares/. Les dues cares
(davant/darrere) **NO** són simètriques — cada sketch té el seu propi
`RANGS[]`/`SOLTS[]` independent, amb un nombre de LEDs per ala diferent.

Posició física de les ales (igual a totes dues cares): l'ala 1 és baix a
l'esquerra, pujant fins a l'ala 4 a dalt a l'esquerra; l'ala 5 és dalt a
la dreta, baixant fins a l'ala 8 a baix a la dreta. Els colors seguixen
aquest mateix recorregut (càlids a l'esquerra, freds a la dreta,
trobant-se en violeta/indi a dalt de tot), tal com es veu al logo.

La majoria de LEDs de cada ala van **seguits** (rang principal), però
alguns LEDs "solts" —al mig del rang d'una altra ala— en realitat
il·luminen una ala diferent de la que els tocaria pel seu número; són
efecte del cablejat físic real, no un error de comptatge.

**Cara A** (`cara-davant/cara-davant.ino`, 853 LEDs):

| Ala | Color | Rang principal | Solts |
|---|---|---|---|
| 1 | Groc | 1 – 105 | — |
| 2 | Taronja | 106 – 210 | 418, 419 |
| 3 | Vermell | 211 – 312 | 413 – 417 |
| 4 | Violeta | 313 – 412 | — |
| 5 | Indi | 424 – 528 | — |
| 6 | Blau fosc | 529 – 638 | 423 |
| 7 | Blau clar | 639 – 745 | 420 – 422 |
| 8 | Verd | 746 – 853 | — |

**Cara B** (`cara-darrere/cara-darrere.ino`, 868 LEDs):

| Ala | Color | Rang principal | Solts |
|---|---|---|---|
| 1 | Groc | 1 – 110 | — |
| 2 | Taronja | 111 – 216 | 438, 439 |
| 3 | Vermell | 217 – 327 | 437 |
| 4 | Violeta | 328 – 436 | — |
| 5 | Indi | 440 – 543 | — |
| 6 | Blau fosc | 544 – 647 | — |
| 7 | Blau clar | 648 – 754 | — |
| 8 | Verd | 755 – 868 | — |
