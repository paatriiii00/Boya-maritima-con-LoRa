// Librerías:
#pragma once
#include <DallasTemperature.h>
#include <OneWire.h>
#include <Arduino.h>
#include <math.h>

// Variables globales:
extern const int pinSensor_Temp_1m;
extern const int pinSensor_pH;
extern const int pinSensor_T;
extern const int pin_mos;
extern const int pin_bat;

OneWire oneWireTemp(pinSensor_Temp_1m); // Objeto del sensor de temperatura en pin del sensor
DallasTemperature Temp1m(&oneWireTemp); // Inicialización del objeto

/* Función de dormir sensores: Desconecta los sensores de la alimentación */
void duerme_sensores(){
  Serial.println("Durmiendo sensores...");
  digitalWrite(pin_mos, LOW);               // Pin del mosfet a LOW
}

/* Función de despertar sensores: Conecta los sensores a alimentación */
void despierta_sensores(){
  Serial.println("Derpertando sensores...");
  digitalWrite(pin_mos, HIGH);            // Pin del mosfet a HIGH
}

/* Función de medida de la temperatura a 1 metro: */
uint16_t obten_temp1m(){
  Temp1m.requestTemperatures();                     // Leo la temperatura y la almaceno
  float temperatura1m = Temp1m.getTempCByIndex(0);  // Selecciono el valor de temperatura del indice 0
  Serial.print("La temperatura a 1m es: ");
  Serial.println(temperatura1m);
  return temperatura1m*100;                         // Multiplico por 100 para manejar un valor entero
}

/* Función de medida de la temperatura de la electrónica: */
uint16_t obten_temp_elec(){
  float Temp_analog=analogRead(pinSensor_T);        // Leo el pin del sensor de temperatura
  float temp_V=Temp_analog*3.3/4095;                // Transformo los bits a voltios
  Serial.print("El voltaje del sensor de T es: ");
  Serial.println(temp_V);
  float temperatura=temp_V*(45)/3.3;                // Recta de ajuste de la temperatura         
  Serial.print("La temperatura del circuito es: ");
  Serial.println(temperatura);
  return temperatura*100;                           // Multiplico por 100 para trabajar con valores enteros
}

/* Función de medida del pH: */
uint16_t obten_pH(){
  float medida_pH= analogRead(pinSensor_pH);  // Leo el pin del sensor de pH
  float pH_a_V=medida_pH*3.3/4095;            // Transformo los bits a voltios
  Serial.print("El voltaje de pH: ");
  Serial.println(pH_a_V);
  float valor_pH=(7/1.65)*pH_a_V;              // Recta de ajuste del pH
  Serial.print("El valor de pH es: ");
  Serial.println(valor_pH);
  return valor_pH*100;                        // Multiplico por 100 para manejar valores enteros
}

/* Función de medida de la batería: */
uint16_t obten_bat() {
  float Vbat_micro = (float)(analogRead(pin_bat))*3.3/4096; // Lectura y conversión del valor medido a voltios
  Serial.print("El voltaje medido en el micro es: ");
  Serial.print(Vbat_micro);
  Serial.println(" V");
  float Vbat_pila = Vbat_micro*5/3.3;                      // La batería es de 3.3V pero aporta 5V
  Serial.print("El voltaje en la pila es: ");
  Serial.print(Vbat_pila);
  Serial.println(" V");
  float Vbat=100*Vbat_pila/5;                             // Recta de ajuste de la batería
  Serial.print("El % de la bateria es: ");
  Serial.print(Vbat*100);
  return Vbat*100;                                        // Multiplico por 100 para trabajar con valores enteros

}

// Especificación del duty
int duty(){
  return SEND_INTERVAL;
}

/* Función de construcción del paquete a enviar: */
void BuildPacket(uint8_t txBuffer[8]) {
  //Inicio la medida y llamo a las funciones anteriores para obtener los datos
  Serial.println("Inicio medida...");
  uint16_t valor_pH = obten_pH();
  uint16_t valor_temp_elec = obten_temp_elec();
  uint16_t valor_temp1m = obten_temp1m();
  uint16_t valor_bat =obten_bat();

  // Ahora divido los datos en parte alta y baja para guardar cada una de ellas
  // en un byte para la transmisión.

  txBuffer[0] = lowByte(valor_pH);
  txBuffer[1] = highByte(valor_pH);

  txBuffer[2] = lowByte(valor_temp_elec);
  txBuffer[3] = highByte(valor_temp_elec);
  
  txBuffer[4] = lowByte(valor_temp1m);
  txBuffer[5] = highByte(valor_temp1m);
  
  txBuffer[6] = lowByte(valor_bat);
  txBuffer[7] = highByte(valor_bat);
  
  // Imprimo el buffer
  Serial.print("\nEl buffer enviado es [");
  for (int i = 0; i < 8; i++) {
    Serial.print(txBuffer[i]);
    Serial.print(" ");
  }
  Serial.println("]");

  Serial.println("Fin de la medida");
}


