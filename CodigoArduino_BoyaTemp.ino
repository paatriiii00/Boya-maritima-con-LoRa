
#pragma once
#include <OneWire.h>
#include <SPI.h>
#include "rom/rtc.h"

#include "configuracion.h"
#include "credenciales.h"

const int pinSensor_Temp_1m=15;
const int pinSensor_pH=12;
const int pinSensor_T=14;
const int pin_mos=13;
const int pin_bat=34;

static uint8_t txBuffer[8];   // Paquete de datos como variable estática global


// -----------------------------------------------------------------------------------------------------------------------------------------------------------
// Funcion para enviar el paquete de datos LoRa
// -----------------------------------------------------------------------------------------------------------------------------------------------------------
void send(){
  despierta_sensores();
  delay(20000);           // 20 segundos para estabilización de la alimentación antes de la medida de pH
  BuildPacket(txBuffer);
  duerme_sensores();
  delay(1000);            // 1 segundo de delay para garantizar la buena desconexión de los sensores

#if LORAWAN_CONFIRMED_EVERY > 0
  bool confirmed = (count % LORAWAN_CONFIRMED_EVERY == 0);
#else
  bool confirmed = false;
#endif

  //ttn_cnt(count);
  
  ttn_send(txBuffer, sizeof(txBuffer), LORAWAN_PORT, confirmed);
  
  //count++;                                                  // send count plus one
}

// -----------------------------------------------------------------------------------------------------------------------------------------------------------
// Funcion para dormir el ESP32 durante el tiempo de duty cycle
// -----------------------------------------------------------------------------------------------------------------------------------------------------------
void sleep(){
#if SLEEP_BETWEEN_MESSAGES == 1
  Serial.println("Iniciando deep sleep...");                // Mostrar mensaje de que se ha iniciado el deep sleep
  Serial.println();

  uint32_t sleep_for = (millis() < duty()) ? duty() - millis() : duty();; 
  sleep_millis(sleep_for);
#endif
}

// -----------------------------------------------------------------------------------------------------------------------------------------------------------
// Funcion para mostrar mensajes por monitor serial segun se interactue con TTN
// -----------------------------------------------------------------------------------------------------------------------------------------------------------
void callback(uint8_t message){
  if(EV_JOINING == message)Serial.println("Joining TTN...");
  if(EV_JOINED == message)Serial.println("TTN joined!");
  if(EV_JOIN_FAILED == message)Serial.println("TTN join failed");
  if(EV_REJOIN_FAILED == message)Serial.println("TTN rejoin failed");
  if(EV_RESET == message)Serial.println("Reset TTN connection");
  if(EV_LINK_DEAD == message)Serial.println("TTN link dead");
  if(EV_ACK == message)Serial.println("ACK received");
  if(EV_PENDING == message)Serial.println("Message discarded");
  if(EV_QUEUED == message)Serial.println("Message queued");
  if(EV_TXCOMPLETE == message){
    Serial.println(F("EV_TXCOMPLETE (includes waiting for RX windows)"));
    if(LMIC.txrxFlags & TXRX_ACK){
      Serial.println(F("Received ack"));
    }
    if(LMIC.dataLen){
      Serial.print(F("Received "));
      Serial.print(LMIC.dataLen);
      Serial.println(F(" bytes of payload"));
      for (int i = 0; i < LMIC.dataLen; i++){
        if (LMIC.frame[LMIC.dataBeg + i] < 0x10){
          Serial.print(F("0"));
        }
        Serial.print(F("Received payload: "));
        Serial.print(LMIC.frame[LMIC.dataBeg + i], HEX);
      }
      Serial.println();
    }
    sleep();
  }

  if(EV_RESPONSE == message){
    Serial.print(F("[TTN] Response: "));

    size_t len = ttn_response_len();
    uint8_t data[len];
    ttn_response(data, len);

    char buffer[6];
    for(uint8_t i = 0; i < len; i++){
      Serial.print(F(buffer));
    }
    Serial.println();
  }
}

// -----------------------------------------------------------------------------------------------------------------------------------------------------------
// Funcion de temporizacion en milisegundos personalizable. Como 'delay()', pero sin detener la ejecución del programa
// -----------------------------------------------------------------------------------------------------------------------------------------------------------
void mi_timer(int intervalo){                                
  unsigned long inicioTiempo = millis();                     
  while(millis() - inicioTiempo < intervalo){} 
}

// -----------------------------------------------------------------------------------------------------------------------------------------------------------
// Funcion para activar o desactivar desde "configuration.h" el monitor serial para debugging
// -----------------------------------------------------------------------------------------------------------------------------------------------------------
void debug_code(){
#if ENABLE_DEBUG == 1
  DEBUG_PORT.begin(SERIAL_BAUD);
#endif
}

// ===========================================================================================================================================================
// Setup main
// ===========================================================================================================================================================
void setup() {

  debug_code();
  //
  // Init pines de perifericos que envian por LoRa -----------------------------------------------------------------------------------------------------------
  //
  pinMode(VBATPIN, INPUT);
  pinMode(pinSensor_Temp_1m, INPUT);
  pinMode(pinSensor_T, INPUT);
  pinMode(pinSensor_pH, INPUT);
  pinMode(pin_mos, OUTPUT);

  //
  // TTN setup -----------------------------------------------------------------------------------------------------------------------------------------------
  //
  if(!ttn_setup()){
    Serial.println("[ERR] Radio module not found!");
    sleep_forever();
  }

  //
  // TTN register --------------------------------------------------------------------------------------------------------------------------------------------
  //
  ttn_register(callback);
  ttn_join();
  ttn_sf(LORAWAN_SF);
  ttn_adr(LORAWAN_ADR);

}

// ===========================================================================================================================================================
// Loop main
// ===========================================================================================================================================================
void loop() {
  ttn_loop();

  static uint32_t last = 0;
   static bool first = true;
    if(0 == last || millis() - last > duty()){
      last = millis();
      first = false;
      Serial.println("TRANSMITTING");
      send();
    }
}
