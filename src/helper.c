// SPDX-License-Identifier: GPL-2.0-only
// Copyright (C) 2022, Input Labs Oy.

#include <stdio.h>
#include <pico/stdlib.h>

uint32_t bin(uint8_t k) {
    return (k == 0 || k == 1 ? k : ((k % 2) + 10 * bin(k / 2)));
}

uint32_t bin16(uint16_t k) {
    return (k == 0 || k == 1 ? k : ((k % 2) + 10 * bin(k / 2)));
}
