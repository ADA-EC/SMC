/*
 * DS RTC Library: DS1307 and DS3231 driver library
 * (C) 2011 Akafugu Corporation
 *
 * This program is free software; you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 *
 */
 /*
  * Code simplified for DS3231 only by
  * ADA - Projetos em Engenharia de Computacao
  * Author: Pedro V. B. Jeronymo (pedrovbj@gmail.com)
  */

#ifndef DS3231_H
#define DS3231_H

#include <stdlib.h>
#include <avr/io.h>
#include "twi.h"
#include <stdbool.h>

// Get/set time
void rtc_get_date_time(uint8_t* hour, uint8_t* min, uint8_t* sec, uint8_t *day, uint8_t *month, uint8_t *year);
void rtc_set_date_time(uint8_t hour, uint8_t min, uint8_t sec, uint8_t day, uint8_t month, uint8_t year);

// Read Temperature
void ds3231_get_temp_int(int8_t* i, uint8_t* f);
void rtc_force_temp_conversion(uint8_t block);

// Auxillary functions
enum RTC_SQW_FREQ { FREQ_1 = 0, FREQ_1024, FREQ_4096, FREQ_8192 };

void rtc_SQW_enable(bool enable);
void rtc_SQW_set_freq(enum RTC_SQW_FREQ freq);
void rtc_osc32kHz_enable(bool enable);

// Alarm functionality
void rtc_reset_alarm(void);
void rtc_enable_alarm(bool enable);
void rtc_set_alarm(uint8_t mask, uint8_t hour, uint8_t min, uint8_t sec);
void rtc_get_alarm(uint8_t* hour, uint8_t* min, uint8_t* sec);
bool rtc_check_alarm(void);

#endif
