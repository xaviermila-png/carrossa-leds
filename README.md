# carrossa-leds — firmware Arduino Mega

Projecte nou, independent de la família ARDMX (`c:\ARDMX\...`).

## Maquinari
- Arduino Mega

## Compilar i pujar (arduino-cli)
```
arduino-cli compile --fqbn arduino:avr:mega carrossa-leds
arduino-cli upload --fqbn arduino:avr:mega -p COMx carrossa-leds
```
(substitueix `COMx` pel port real — `arduino-cli board list` per confirmar-lo).

## Estat
Just creat, encara sense funcionalitat.
