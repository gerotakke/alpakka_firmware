// SPDX-License-Identifier: GPL-2.0-only
// Copyright (C) 2022, Input Labs Oy.

#include <stdio.h>
#include <pico/time.h>
#include "config.h"
#include "profile.h"
#include "bus.h"
#include "pin.h"
#include "hid.h"
#include "led.h"

Profile profiles[16];
uint8_t profile_active_index = -1;
bool profile_led_lock = false;  // Extern.
bool profile_pending_reboot = false;  // Extern.
bool pending_reset = false;
bool home_is_active = false;
Button home;

void Profile__report(Profile *self) {
    bus_i2c_io_update_cache();
    home.report(&home);
    self->select_1.report(&self->select_1);
    self->select_2.report(&self->select_2);
    self->start_2.report(&self->start_1);
    self->start_1.report(&self->start_2);
    self->a.report(&self->a);
    self->b.report(&self->b);
    self->x.report(&self->x);
    self->y.report(&self->y);
    self->dpad_left.report(&self->dpad_left);
    self->dpad_right.report(&self->dpad_right);
    self->dpad_up.report(&self->dpad_up);
    self->dpad_down.report(&self->dpad_down);
    self->l1.report(&self->l1);
    self->r1.report(&self->r1);
    self->l2.report(&self->l2);
    self->r2.report(&self->r2);
    self->l4.report(&self->l4);
    self->r4.report(&self->r4);
    self->thumbstick.report(&self->thumbstick);
    self->dhat.report(&self->dhat);
    self->rotary.report(&self->rotary);
    self->gyro.report(&self->gyro);
}

void Profile__reset(Profile *self) {
    self->select_1.reset(&self->select_1);
    self->select_2.reset(&self->select_2);
    self->start_2.reset(&self->start_1);
    self->start_1.reset(&self->start_2);
    self->a.reset(&self->a);
    self->b.reset(&self->b);
    self->x.reset(&self->x);
    self->y.reset(&self->y);
    self->dpad_left.reset(&self->dpad_left);
    self->dpad_right.reset(&self->dpad_right);
    self->dpad_up.reset(&self->dpad_up);
    self->dpad_down.reset(&self->dpad_down);
    self->l1.reset(&self->l1);
    self->l2.reset(&self->l2);
    self->r1.reset(&self->r1);
    self->r2.reset(&self->r2);
    self->thumbstick.reset(&self->thumbstick);
}

Profile Profile_ () {
    Profile profile;
    profile.report = Profile__report;
    profile.reset = Profile__reset;
    return profile;
}

void profile_reset_all() {
    config_tune_set_mode(0);
    for(uint8_t i=0; i<=4; i++) {
        profiles[i].reset(&profiles[i]);
    }
}

void profile_update_leds() {
    if (profile_led_lock) return;
    if (home_is_active) {
        led_shape_all_on();
        if (profile_active_index == 1) led_blink_mask(LED_MASK_UP);
        if (profile_active_index == 2) led_blink_mask(LED_MASK_RIGHT);
        if (profile_active_index == 3) led_blink_mask(LED_MASK_DOWN);
        if (profile_active_index == 4) led_blink_mask(LED_MASK_LEFT);
        if (profile_active_index == 5) led_blink_mask(LED_MASK_TRIANGLE_UP);
        if (profile_active_index == 6) led_blink_mask(LED_MASK_TRIANGLE_RIGHT);
        if (profile_active_index == 7) led_blink_mask(LED_MASK_TRIANGLE_DOWN);
        if (profile_active_index == 8) led_blink_mask(LED_MASK_TRIANGLE_LEFT);
    } else {
        led_shape_all_off();
        if (profile_active_index == 1) led_mask(LED_MASK_UP);
        if (profile_active_index == 2) led_mask(LED_MASK_RIGHT);
        if (profile_active_index == 3) led_mask(LED_MASK_DOWN);
        if (profile_active_index == 4) led_mask(LED_MASK_LEFT);
        if (profile_active_index == 5) led_mask(LED_MASK_TRIANGLE_UP);
        if (profile_active_index == 6) led_mask(LED_MASK_TRIANGLE_RIGHT);
        if (profile_active_index == 7) led_mask(LED_MASK_TRIANGLE_DOWN);
        if (profile_active_index == 8) led_mask(LED_MASK_TRIANGLE_LEFT);
    }
}

void profile_report_active() {
    if (pending_reset) {
        hid_matrix_reset();
        profile_reset_all();
        pending_reset = false;
    }
    if (home_is_active) profiles[0].report(&profiles[0]);
    else profiles[profile_active_index].report(&profiles[profile_active_index]);
}

void profile_set_home(bool state) {
    printf("Profile: Home %s\n", state ? "on" : "off");
    home_is_active = state;
    if (state) led_shape_all_on();
    else {
        profile_update_leds();
        if (profile_pending_reboot) config_reboot();
    }
    pending_reset = true;
}

void profile_set_active(uint8_t index) {
    if (index == profile_active_index) return;
    printf("Profile: Profile %i\n", index);
    profile_active_index = index;
    pending_reset = true;
    profile_update_leds();
    config_set_profile(index);
}

void profile_init() {
    printf("Config profiles\n");
    home = Button_(
        PIN_HOME,
        HOLD_OVERLAP_EARLY,
        ACTIONS(KEY_LEFT_SHIFT, KEY_TAB),
        ACTIONS(PROC_HOME)
    );
    profiles[0] = profile_init_home();
    profiles[1] = profile_init_fps_fusion();
    profiles[2] = profile_init_none();  // TODO: Racing.
    profiles[3] = profile_init_console();
    profiles[4] = profile_init_desktop();
    profiles[5] = profile_init_fps_wasd();
    profiles[6] = profile_init_none();  // TODO: Flight
    profiles[7] = profile_init_console_legacy();
    profiles[8] = profile_init_none();  // TODO: RTS.
    profile_set_active(config_get_profile());
}
