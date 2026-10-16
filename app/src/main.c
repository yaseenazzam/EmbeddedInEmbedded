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
  int count = 0;
  while (1) {
    if (BTN_check_clear_pressed(BTN0)){
      count++;
    }

    if (count & 1){
      LED_set(LED3, LED_ON);
    } else {
      LED_set(LED3, LED_OFF);
    }

    if (count & 2){
      LED_set(LED2, LED_ON);
    } else {
      LED_set(LED2, LED_OFF);
    }

    if (count & 4){
      LED_set(LED1, LED_ON);
    } else {
      LED_set(LED1, LED_OFF);
    }

    if (count & 8){
      LED_set(LED0, LED_ON);
    } else {
      LED_set(LED0, LED_OFF);
    }
    if (count == 16){
      printk("Button 0 pressed 16 times\n");
      count = 0;
    }
    k_msleep(SLEEP_MS);
  }
  return 0;
}
