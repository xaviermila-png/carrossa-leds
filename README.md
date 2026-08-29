# carrossa-leds

Control de la il·luminació del logo de la carrossa de l'associació
**"INDEPENDENTS"** (Carnaval de Sitges) amb dues tires de LEDs digitals
WS2812B/NeoPixel — una per cada cara de la carrossa (davant i darrere).

És un programa **estàtic**: cada cara del logo té 8 plomes, cadascuna
il·luminada d'un color fix, sense efectes ni animacions ni cap botó físic.

## Maquinari
- 2x Arduino Mega 2560 — un per cada cara de la carrossa.
- Cada Arduino controla la seva pròpia tira NeoPixel independent (240 LEDs
  declarats, `NUM_LEDS`).
- Pin de dades: GPIO 2 a totes dues plaques.

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

## Mapa de LEDs per ploma

> **Pendent de confirmar** — els trams `[inici, fi]` de cada ploma són
> *placeholder*, extrets directament dels índexs que feia servir el
> programa original (8 plomes de 9 LEDs, amb 1 LED de buit entre ploma i
> ploma), NO són mesures reals. Cal confirmar/ajustar:
> - Nombre exacte de LEDs de cada ploma (poden no ser tots iguals).
> - Si hi ha algun LED apagat entre ploma i ploma, o van totes seguides.
> - Si l'ordre físic del cablejat coincideix amb aquest ordre de colors.
> - Si les dues cares (davant/darrere) fan servir els mateixos 8
>   colors/ordre — de moment són idèntics als dos sketches.

| Ploma | LEDs (índex) | Color |
|---|---|---|
| 1 | 0–8 | Groc |
| 2 | 10–18 | Taronja |
| 3 | 20–28 | Vermell |
| 4 | 30–38 | Magenta (lila1) |
| 5 | 40–48 | Verd |
| 6 | 50–58 | Blau clar |
| 7 | 60–68 | Blau fosc |
| 8 | 70–78 | Indi (lila2) |

Per ajustar-ho, edita l'array `PLOMES[]` a `cara-davant/cara-davant.ino` /
`cara-darrere/cara-darrere.ino`.
