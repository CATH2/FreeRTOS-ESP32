

#ifndef DHT11_H_
#define DHT11_H_

#include <driver/rmt.h>
#include <soc/rmt_reg.h>
#include "driver/gpio.h" 
#include <esp_log.h>

void DHT11_Init(uint8_t dht11_pin);
int DHT11_StartGet(int *temperature, int *humidity);

#endif