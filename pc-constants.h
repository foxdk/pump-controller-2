/*
   Copyright (c) 2026 David K. Fox
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/foxdk/pump-controller-2
 */
#ifndef PcConstants_h
#define PcConstants_h

extern const unsigned long PUMP_RUN_DURATION_SECONDS;          // How long the pump should run, in seconds
extern const unsigned long LARGE_FLOW_MIN_DURATION_SECONDS;    // The minimum flow duration that will reset the wait timer
extern const unsigned long MIN_WAIT_AFTER_LARGE_FLOW_MINUTES;  // The wait timer, which lets water and pump cool before running the pump again

extern const unsigned long PUMP_RUN_DURATION_MILLIS;
extern const unsigned long LARGE_FLOW_MIN_DURATION_MILLIS;
extern const unsigned long MIN_WAIT_AFTER_LARGE_FLOW_MILLIS;

#endif  // PcConstants_h