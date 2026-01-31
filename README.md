# IS31FL3197-LED-Driver-Library-for-STM32
IS31FL3197 LED Driver Library for STM32

Example Usage:
```
#include "IS31FL3197.h"

IS31FL3197_HandleTypeDef d={.hi2c = &hi2c1,.addr = IS31FL3197_ADDR_GND,.sdb_port = GPIOA,.sdb_pin = GPIO_PIN_8};
current_band_handle_t current_band = {.cb = {IS31FL3197_CB_FULL,IS31FL3197_CB_FULL,IS31FL3197_CB_FULL,IS31FL3197_CB_FULL}};

int main(){
  HAL_Init();
  SystemClock_Config();

  MX_GPIO_Init();
  MX_I2C1_Init();

  IS31FL3197_Init(&d);
  IS31FL3197_Current_Band(&d,&current_band);

  IS31FL3197_LED_All_Off(&d);

  while (1)
  {
	  IS31FL3197_LED_Blink_All(&d,1000);

  }
}
```
}
