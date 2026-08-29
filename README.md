# carrossa-leds (branca: dues-cares-un-arduino)

Control de la il·luminació del logo de la carrossa de l'associació
**"INDEPENDENTS"** (Carnaval de Sitges) amb dues tires de LEDs digitals
WS2812B/NeoPixel — una per cada cara de la carrossa (davant i darrere).

És un programa **estàtic**: cada cara del logo té 8 plomes, cadascuna
il·luminada d'un color fix, sense efectes ni animacions ni cap botó físic.

**Aquesta branca és una alternativa a `main`**: en lloc de dos Arduino
Mega (un per cara), fa servir **UN sol Arduino Mega** amb les dues tires
connectades a pins de dades diferents (`PIN_DAVANT`=2, `PIN_DARRERE`=3).
Si algun dia una tira falla, només cal desconnectar-la i endollar-hi la
de recanvi al pin corresponent — no cal tocar la placa ni el codi.
Avantatge sobre `main`: una sola placa a muntar/alimentar. Diferència a
tenir en compte: si mai es vol tenir una Mega sencera de recanvi
intercanviable (no només una tira), calen dues plaques com a `main`, no
una.

## Maquinari
- 1x Arduino Mega 2560, amb dues tires NeoPixel independents connectades
  a pins diferents (240 LEDs declarats per tira, `NUM_LEDS`).
- Pins de dades: GPIO 2 (cara davant), GPIO 3 (cara darrere).

## Estructura del projecte
- [`dues-cares-un-arduino/`](dues-cares-un-arduino/) — sketch únic per a
  l'Arduino, controla les dues tires.
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
1. Obre `dues-cares-un-arduino/dues-cares-un-arduino.ino`.
2. Selecciona la placa **Arduino Mega or Mega 2560**.
3. Instal·la la llibreria **Adafruit NeoPixel** (Gestor de llibreries).
4. Puja amb el botó habitual.

**arduino-cli:**
```
arduino-cli lib install "Adafruit NeoPixel"
arduino-cli compile --fqbn arduino:avr:mega dues-cares-un-arduino
arduino-cli upload --fqbn arduino:avr:mega -p COMx dues-cares-un-arduino
```
(substitueix `COMx` pel port real — `arduino-cli board list` per confirmar-lo).

## Mapa de LEDs per ploma

Confirmat: les 8 plomes van totes **seguides** (sense cap LED apagat entre
mig), en aquest ordre de colors, i les dues cares (davant/darrere) són
**idèntiques**.

> **Pendent de confirmar** — el nombre de LEDs de cada ploma és
> *placeholder* (9 a totes, el mateix que feia servir el programa
> original) — no es coneix encara la mida real, i poden no ser totes
> iguals. Per ajustar-ho: puja el programa, mira on cau realment el tall
> entre cada color a la tira física, i corregeix els 8 números de
> `PLOMA_LEDS[]` a `dues-cares-un-arduino/dues-cares-un-arduino.ino` (un
> sol array, compartit per les dues cares mentre estiguin confirmades
> idèntiques) fins que cada tall caigui just al final de la ploma
> corresponent. Com que les plomes van seguides, no cal calcular índexs a
> mà — cada ploma comença just on acaba l'anterior.

| Ploma | LEDs | Color |
|---|---|---|
| 1 | 9 (placeholder) | Groc |
| 2 | 9 (placeholder) | Taronja |
| 3 | 9 (placeholder) | Vermell |
| 4 | 9 (placeholder) | Magenta (lila1) |
| 5 | 9 (placeholder) | Verd |
| 6 | 9 (placeholder) | Blau clar |
| 7 | 9 (placeholder) | Blau fosc |
| 8 | 9 (placeholder) | Indi (lila2) |
