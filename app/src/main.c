/**
 * @file main.c
 */

#include <inttypes.h>

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#include "BTN.h"
#include "LED.h"

#define SLEEP_MS 1

int main(void) {
  if (0 > BTN_init()) {
    return 0;
  }
  if (0 > LED_init()) {
    return 0;
  }

  uint8_t brightness = 0;
  LED_pwm(LED0, brightness);

  while (1) {
    if (BTN_is_pressed(BTN0) && brightness < 100){
      brightness++;
       LED_pwm(LED0, brightness);
    }
    k_msleep(20);
  }
  return 0;
}
