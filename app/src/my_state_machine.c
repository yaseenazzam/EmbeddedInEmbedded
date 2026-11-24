
/** my_state_machine.c */

#include <zephyr/smf.h>
#include <zephyr/kernel.h>
#include <stdint.h>

#include "LED.h"
#include "my_state_machine.h"
#include "BTN.h"

static void S0_entry(void *o);
static void S1_entry(void *o);
static void S2_entry(void *o);
static void S3_entry(void *o);
static void S4_entry(void *o);
static enum smf_state_result S0_run(void *o);
static enum smf_state_result S1_run(void *o);
static enum smf_state_result S2_run(void *o);
static enum smf_state_result S3_run(void *o);
static enum smf_state_result S4_run(void *o);


enum state_machine_states {
    S0,
    S1,
    S2,
    S3,
    S4,
};

typedef struct {
    struct smf_ctx ctx;

    uint16_t count;
    uint32_t events;

    k_timeout_t timeout;
} led_state_object_t;



static led_state_object_t led_state_object;

static const struct smf_state led_states[] = {
    [S0] = SMF_CREATE_STATE(S0_entry, S0_run, NULL, NULL, NULL),
    [S1] = SMF_CREATE_STATE(S1_entry, S1_run, NULL, NULL, NULL),
    [S2] = SMF_CREATE_STATE(S2_entry, S2_run, NULL, NULL, NULL),
    [S3] = SMF_CREATE_STATE(S3_entry, S3_run, NULL, NULL, NULL),
    [S4] = SMF_CREATE_STATE(S4_entry, S4_run, NULL, NULL, NULL),

};

static void S0_entry(void *o) {
    LED_set(LED0, LED_OFF);
    LED_set(LED1, LED_OFF);
    LED_set(LED2, LED_OFF);
    LED_set(LED3, LED_OFF);
}

static enum smf_state_result S0_run(void *o) {
    if (led_state_object.events & BTN0_EVENT) {
        smf_set_state(&led_state_object.ctx, &led_states[S1]);
    }

    return SMF_EVENT_HANDLED;
}

static void S1_entry(void *o) {
    LED_toggle(LED0);
}

static enum smf_state_result S1_run(void *o) {
    if (led_state_object.events & BTN1_EVENT) {
        smf_set_state(&led_state_object.ctx, &led_states[S2]);
    }else if (led_state_object.events & BTN2_EVENT) {
        smf_set_state(&led_state_object.ctx, &led_states[S4]);      
    }else if (led_state_object.events & BTN3_EVENT) {
        smf_set_state(&led_state_object.ctx, &led_states[S0]);
    }else if (led_state_object.events == 0) {
        LED_toggle(LED0);
        led_state_object.timeout = K_MSEC(125);
    }
    return SMF_EVENT_HANDLED;
}

static void S2_entry(void *o) {
    LED_set(LED1, LED_OFF);
    LED_set(LED3, LED_OFF);
    LED_set(LED0, LED_ON);
    LED_set(LED2, LED_ON);
    led_state_object.count = 0;
    led_state_object.timeout = K_MSEC(2);
}

static enum smf_state_result S2_run(void *o) {
    if (led_state_object.events & BTN3_EVENT) {
        smf_set_state(&led_state_object.ctx, &led_states[S0]);
    }else if (led_state_object.count >= 500){
        smf_set_state(&led_state_object.ctx, &led_states[S3]);
    }else if (led_state_object.events == 0) {
    led_state_object.count++;
    }

    return SMF_EVENT_HANDLED;
}

static void S3_entry(void *o) {
    LED_set(LED1, LED_ON);
    LED_set(LED3, LED_ON);
    LED_set(LED0, LED_OFF);
    LED_set(LED2, LED_OFF);
    led_state_object.count = 0;
    led_state_object.timeout = K_MSEC(2);
}

static enum smf_state_result S3_run(void *o) {
    if (led_state_object.count > 1000){
        smf_set_state(&led_state_object.ctx, &led_states[S2]);
    }else if (led_state_object.events & BTN3_EVENT) {
        smf_set_state(&led_state_object.ctx, &led_states[S0]);
    }else if (led_state_object.events == 0) {
        led_state_object.count++;
    }

    return SMF_EVENT_HANDLED;
}

static void S4_entry(void *o) {

}

static enum smf_state_result S4_run(void *o) {

    return SMF_EVENT_HANDLED;
}

void state_machine_init(void) {
    led_state_object.count = 0;
    led_state_object.events = 0;
    smf_set_initial(&led_state_object.ctx, &led_states[S0]);
}

int state_machine_run(void) {
    led_state_object.events =
        k_event_wait_safe(&btn_event, BTN_ANY_EVENT, false, led_state_object.timeout);

    return smf_run_state(&led_state_object.ctx);
}
