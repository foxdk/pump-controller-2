/*
   Copyright (c) 2026 David K. Fox
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/foxdk/pump-controller-2
 */
#include <Arduino.h>
#include "pc-constants.h"

const unsigned long PUMP_RUN_DURATION_SECONDS = 90UL;
const unsigned long LARGE_FLOW_MIN_DURATION_SECONDS = 40UL;
const unsigned long MIN_WAIT_AFTER_LARGE_FLOW_MINUTES = 20UL;  // let water and pump cool before running again

unsigned long PUMP_RUN_DURATION_MILLIS = PUMP_RUN_DURATION_SECONDS * 1000UL;
const unsigned long LARGE_FLOW_MIN_DURATION_MILLIS = LARGE_FLOW_MIN_DURATION_SECONDS * 1000UL;
const unsigned long MIN_WAIT_AFTER_LARGE_FLOW_MILLIS = MIN_WAIT_AFTER_LARGE_FLOW_MINUTES * 60UL * 1000UL;
