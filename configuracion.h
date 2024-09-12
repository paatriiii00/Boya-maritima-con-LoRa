
#pragma once
#include <Arduino.h>
#include <lmic.h>

void ttn_register(void (*callback)(uint8_t message));

// -----------------------------------------------------------------------------------------------------------------------------------------------------------
// Configuration
// -----------------------------------------------------------------------------------------------------------------------------------------------------------

// If using a single-channel gateway, uncomment this next option and set to your gateway's channel
//#define SINGLE_CHANNEL_GATEWAY  0

// If you are having difficulty sending messages to TTN after the first successful send, uncomment the next option and experiment with values (~ 1 - 5)
#define CLOCK_ERROR             1

#define DEBUG_PORT              Serial      // Serial debug port
#define SERIAL_BAUD             115200      // Serial debug baud rate
#define ENABLE_DEBUG            1           // Serial debug on/off
#define SLEEP_BETWEEN_MESSAGES  1           // Do sleep between messages
#define SEND_INTERVAL           3600000     // Sleep for these many millis
//#define SEND_INTERVAL           60000
#define LORAWAN_PORT            1           // Port the messages will be sent to
#define LORAWAN_CONFIRMED_EVERY 0           // Send confirmed message every these many messages (0 means never)
#define LORAWAN_SF              DR_SF7      // Spreading factor
#define LORAWAN_ADR             1           // Enable ADR
#define TARGET_ARRAY_LENGTH     5           // Tamaño del Buffer Circular

// -----------------------------------------------------------------------------------------------------------------------------------------------------------
// DEBUG
// -----------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef DEBUG_PORT
#define DEBUG_MSG(...) DEBUG_PORT.printf( __VA_ARGS__ )
#else
#define DEBUG_MSG(...)
#endif

// -----------------------------------------------------------------------------------------------------------------------------------------------------------
// Custom messages
// -----------------------------------------------------------------------------------------------------------------------------------------------------------
#define EV_QUEUED       100
#define EV_PENDING      101
#define EV_ACK          102
#define EV_RESPONSE     103

// -----------------------------------------------------------------------------------------------------------------------------------------------------------
// Sensores
// -----------------------------------------------------------------------------------------------------------------------------------------------------------
#define trigPin         13                  // Defino pin para 'Trig' - JSN-SR04T
#define echoPin         12                  // Defino pin para 'Echo' - JSN-SR04T
#define MAX_PH          14                  // En 'MAX_PH' guardaré de forma inmutable el valor maximo de pH posible
#define MIN_PH          0                   // En 'MIN_PH' guardaré de forma inmutable el valor mínimo de pH posible
#define MAX_TEMP_1M     125                 // En 'MAX_TEMP_1M' guardaré de forma inmutable el valor máximo de temperatura a 1m posible
#define MIN_TEMP_1M     -55                 // En 'MIN_TEMP_1M' guardaré de forma inmutable el valor mínimo de temperatura a 1m posible
#define MAX_T           60                  // En 'MAX_T' guardaré de forma inmutable el valor mínimo de temperatura para la electrónica
#define MIN_T           0                   // En 'MIN_T' guardaré de forma inmutable el valor mínimo de temperatura para la electrónica
#define VBATPIN         34                  // Defino pin 'vbatPin' - Voltaje


// -----------------------------------------------------------------------------------------------------------------------------------------------------------
// LoRa SPI
// -----------------------------------------------------------------------------------------------------------------------------------------------------------
#define SCK_GPIO        5
#define MISO_GPIO       19
#define MOSI_GPIO       27
#define NSS_GPIO        18
#define RESET_GPIO      23
#define DIO0_GPIO       26
#define DIO1_GPIO       33
#define DIO2_GPIO       32

// -----------------------------------------------------------------------------------------------------------------------------------------------------------
// AXP192 (Rev1-specific options)
// -----------------------------------------------------------------------------------------------------------------------------------------------------------
#define LORA_POWER_CTRL_CH    2



