/**
 * @file main.c
 */

#include <inttypes.h>

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#include "BTN.h"
#include "LED.h"

#define SLEEP_MS 500

int main(void) {
  if (0 > BTN_init()) {
    return 0;
  }
  if (0 > LED_init()) {
    return 0;
  }
  
  while (1) {
  LED_set(LED0, LED_ON);
  k_msleep(500);
  LED_set(LED0, LED_OFF);

  LED_set(LED1, LED_ON);
  k_msleep(500);
  LED_set(LED1, LED_OFF);

  LED_set(LED2, LED_ON);
  k_msleep(500);
  LED_set(LED2, LED_OFF);

  LED_set(LED3, LED_ON);
  k_msleep(500);
  LED_set(LED3, LED_OFF);
}
  return 0;
}
