/*
  carrossa-leds — Escàner I2C
  Sketch AÏLLAT de diagnòstic: busca l'adreça real del mòdul I2C de la
  pantalla LCD (SDA=pin 20, SCL=pin 21 al Mega), en lloc d'anar provant
  adreces a cegues (0x27, 0x3F...). Obre el Monitor Sèrie a 115200 baud
  després de pujar-lo.
*/

#include <Wire.h>

void setup() {
  Wire.begin();
  Serial.begin(115200);
  while (!Serial) {
  }
  Serial.println();
  Serial.println(F("Escanejant el bus I2C..."));
}

void loop() {
  uint8_t trobats = 0;

  for (uint8_t adreca = 1; adreca < 127; adreca++) {
    Wire.beginTransmission(adreca);
    const uint8_t error = Wire.endTransmission();

    if (error == 0) {
      Serial.print(F("Dispositiu trobat a l'adreça 0x"));
      if (adreca < 16) Serial.print('0');
      Serial.println(adreca, HEX);
      trobats++;
    }
  }

  if (trobats == 0) {
    Serial.println(F("Cap dispositiu trobat — revisa el cablejat SDA/SCL/VCC/GND."));
  }

  Serial.println(F("---"));
  delay(3000);
}
