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
  LED_toggle(LED1);
  LED_toggle(LED2);
  LED_toggle(LED3);
  k_msleep(500);

  LED_toggle(LED1);
  k_msleep(250);
}
  return 0;
}
