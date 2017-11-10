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
 * DS1307 register map
 *
 *  00h-06h: seconds, minutes, hours, day-of-week, date, month, year (all in BCD)
 *     bit 7 of seconds enables/disables clock
 *     bit 6 of hours toggles 12/24h mode (1 for 12h, 0 for 24h)
 *       when 12h mode is selected bit 5 is high for PM, low for AM
 *  07h: control
 *      bit7: OUT
 *      bit6: 0
 *      bit5: 0
 *      bit4: SQWE
 *      bit3: 0
 *      bit2: 0
 *      bit1: RS0
 *      bit0: RS1
 *  08h-3fh: 56 bytes of SRAM
 *
 * DS3231 register map
 *
 *  00h-06h: seconds, minutes, hours, day-of-week, date, month, year (all in BCD)
 *       bit 7 should be set to zero: The DS3231 clock is always running
 *  07h: A1M1  Alarm 1 seconds
 *  08h: A1M2  Alarm 1 minutes
 *  09h: A1M3  Alarm 1 hour (bit6 is am/pm flag in 12h mode)
 *  0ah: A1M4  Alarm 1 day/date (bit6: 1 for day, 0 for date)
 *  0bh: A2M2  Alarm 2 minutes
 *  0ch: A2M3  Alarm 2 hour (bit6 is am/pm flag in 12h mode)
 *  0dh: A2M4  Alarm 2 day/data (bit6: 1 for day, 0 for date)
 *       <see data sheet page12 for Alarm register mask bit tables:
 *        for alarm when hours, minutes and seconds match set 1000 for alarm 1>
 *  0eh: control
 *      bit7: !EOSC
 *      bit6: BBSQW
 *      bit5: CONV
 *      bit4: RS2
 *      bit3: RS1
 *      bit2: INTCN
 *      bit1: A2IE
 *      bit0: A1IE
 *  0fh: control/status
 *      bit7: OSF
 *      bit6: 0
 *      bit5: 0
 *      bit4: 0
 *      bit3: EN32kHz
 *      bit2: BSY
 *      bit1: A2F alarm 2 flag
 *      bit0: A1F alarm 1 flag
 * 10h: aging offset (signed)
 * 11h: MSB of temp (signed)
 * 12h: LSB of temp in bits 7 and 6 (0.25 degrees for each 00, 01, 10, 11)
 *
 */
/*
 * Code simplified for DS3231 only by
 * ADA - Projetos em Engenharia de Computacao
 * Author: Pedro V. B. Jeronymo (pedrovbj@gmail.com)
 */
#include "ds3231.h"

// I2C address
#define RTC_ADDR 0x68

uint8_t dec2bcd(uint8_t d)
{
  return ((d/10 * 16) + (d % 10));
}

uint8_t bcd2dec(uint8_t b)
{
  return ((b/16 * 10) + (b % 16));
}

uint8_t rtc_read_byte(uint8_t offset)
{
	twi_begin_transmission(RTC_ADDR);
	twi_send_byte(offset);
	twi_end_transmission();

	twi_request_from(RTC_ADDR, 1);
	return twi_receive();
}

void rtc_write_byte(uint8_t b, uint8_t offset)
{
	twi_begin_transmission(RTC_ADDR);
	twi_send_byte(offset);
	twi_send_byte(b);
	twi_end_transmission();
}

void rtc_get_date_time(uint8_t* hour, uint8_t* min, uint8_t* sec, uint8_t *day, uint8_t *month, uint8_t *year)
{
	uint8_t rtc[9];
    uint8_t i;

	// read 7 bytes starting from register 0
	// sec, min, hour, day-of-week, date, month, year
	twi_begin_transmission(RTC_ADDR);
	twi_send_byte(0x0);
	twi_end_transmission();

	twi_request_from(RTC_ADDR, 7);

	for(i=0; i<7; i++) {
		rtc[i] = twi_receive();
	}

	twi_end_transmission();

	if (sec)  *sec =  bcd2dec(rtc[0]);
	if (min)  *min =  bcd2dec(rtc[1]);
	if (hour) *hour = bcd2dec(rtc[2]);
    //Byte 0x03 is for day of the week
    if (day)  *day =  bcd2dec(rtc[4]);
	if (month) *month =  bcd2dec(rtc[5]);
	if (year) *year = bcd2dec(rtc[6]);
}

void rtc_set_date_time(uint8_t hour, uint8_t min, uint8_t sec, uint8_t day, uint8_t month, uint8_t year)
{
	twi_begin_transmission(RTC_ADDR);
	twi_send_byte(0x0);

	twi_send_byte(dec2bcd(sec)); // seconds
	twi_send_byte(dec2bcd(min)); // minutes
	twi_send_byte(dec2bcd(hour)); // hours
    twi_send_byte(dec2bcd(0)); // day-of-week
    twi_send_byte(dec2bcd(day)); // date
	twi_send_byte(dec2bcd(month)); // month
	twi_send_byte(dec2bcd(year)); // year

	twi_end_transmission();
}

void ds3231_get_temp_int(int8_t* i, uint8_t* f)
{
	uint8_t msb, lsb;

	*i = 0;
	*f = 0;

	twi_begin_transmission(RTC_ADDR);
	// temp registers 0x11 and 0x12
	twi_send_byte(0x11);
	twi_end_transmission();

	twi_request_from(RTC_ADDR, 2);

	if (twi_available()) {
		msb = twi_receive(); // integer part (in twos complement)
		lsb = twi_receive(); // fraction part

		// integer part in entire byte
		*i = msb;
		// fractional part in top two bits (increments of 0.25)
		*f = (lsb >> 6) * 25;

		// float value can be read like so:
		// float temp = ((((short)msb << 8) | (short)lsb) >> 6) / 4.0f;
	}
}

void rtc_force_temp_conversion(uint8_t block)
{
	// read control register (0x0E)
	twi_begin_transmission(RTC_ADDR);
	twi_send_byte(0x0E);
	twi_end_transmission();

	twi_request_from(RTC_ADDR, 1);
	uint8_t ctrl = twi_receive();

	ctrl |= 0b00100000; // Set CONV bit

	// write new control register value
	twi_begin_transmission(RTC_ADDR);
	twi_send_byte(0x0E);
	twi_send_byte(ctrl);
	twi_end_transmission();

	if (!block) return;

	// Temp conversion is ready when control register becomes 0
	do {
		// Block until CONV is 0
		twi_begin_transmission(RTC_ADDR);
		twi_send_byte(0x0E);
		twi_end_transmission();
		twi_request_from(RTC_ADDR, 1);
	} while ((twi_receive() & 0b00100000) != 0);
}

void rtc_SQW_enable(bool enable)
{
	twi_begin_transmission(RTC_ADDR);
	twi_send_byte(0x0E);
	twi_end_transmission();

	// read control
	twi_request_from(RTC_ADDR, 1);
	uint8_t control = twi_receive();

	if (enable) {
		control |=  0b01000000; // set BBSQW to 1
		control &= ~0b00000100; // set INTCN to 0
	}
	else {
		control &= ~0b01000000; // set BBSQW to 0
	}

	// write control back
	twi_begin_transmission(RTC_ADDR);
	twi_send_byte(0x0E);
	twi_send_byte(control);
	twi_end_transmission();
}

void rtc_SQW_set_freq(enum RTC_SQW_FREQ freq)
{
	twi_begin_transmission(RTC_ADDR);
	twi_send_byte(0x0E);
	twi_end_transmission();

	// read control (uses bits 3 and 4)
		twi_request_from(RTC_ADDR, 1);
	uint8_t control = twi_receive();

	control &= ~0b00011000; // Set to 0
	control |= (freq << 4); // Set freq bitmask

	// write control back
	twi_begin_transmission(RTC_ADDR);
	twi_send_byte(0x0E);
	twi_send_byte(control);
	twi_end_transmission();
}

void rtc_osc32kHz_enable(bool enable)
{
	twi_begin_transmission(RTC_ADDR);
	twi_send_byte(0x0F);
	twi_end_transmission();

	// read status
	twi_request_from(RTC_ADDR, 1);
	uint8_t status = twi_receive();

	if (enable)
		status |= 0b00001000; // set to 1
	else
		status &= ~0b00001000; // Set to 0

	// write status back
	twi_begin_transmission(RTC_ADDR);
	twi_send_byte(0x0F);
	twi_send_byte(status);
	twi_end_transmission();
}

void rtc_reset_alarm(void)
{
	// writing 0 to bit 7 of all four alarm 1 registers disables alarm
	rtc_write_byte(0, 0x07); // second
	rtc_write_byte(0, 0x08); // minute
	rtc_write_byte(0, 0x09); // hour
	rtc_write_byte(0, 0x0a); // day
}

void rtc_enable_alarm(bool enable) {
    uint8_t val = rtc_read_byte(0x0e);
    if (enable) {
        rtc_write_byte(val | 0b00000101, 0x0e);
    } else {
        rtc_write_byte(val & 0b11111010, 0x0e);
    }
}

// fixme: add an option to set whether or not the INTCN and Interrupt Enable flag is set when setting the alarm
void rtc_set_alarm(uint8_t mask, uint8_t hour, uint8_t min, uint8_t sec)
{
	if (hour > 23) return;
	if (min > 59) return;
	if (sec > 59) return;

	/*
	 *  07h: A1M1:0  Alarm 1 seconds
	 *  08h: A1M2:0  Alarm 1 minutes
	 *  09h: A1M3:0  Alarm 1 hour (bit6 is am/pm flag in 12h mode)
	 *  0ah: A1M4:1  Alarm 1 day/date (bit6: 1 for day, 0 for date)
	 *  Sets alarm to fire when hour, minute and second matches
	 */
	rtc_write_byte(((mask & 0b00000001) << 7) | dec2bcd(sec),  0x07); // second
	rtc_write_byte((((mask & 0b00000010) >> 1) << 7) | dec2bcd(min),  0x08); // minute
	rtc_write_byte((((mask & 0b00000100) >> 2) << 7) | dec2bcd(hour), 0x09); // hour
	rtc_write_byte(0b10000000, 0x0a); // day (upper bit must be set)

	// clear alarm flag
	uint8_t val = rtc_read_byte(0x0f);
	rtc_write_byte(val & ~0b00000001, 0x0f);
}

void rtc_get_alarm(uint8_t* hour, uint8_t* min, uint8_t* sec)
{
	*sec  = bcd2dec(rtc_read_byte(0x07) & ~0b10000000);
	*min  = bcd2dec(rtc_read_byte(0x08) & ~0b10000000);
	*hour = bcd2dec(rtc_read_byte(0x09) & ~0b10000000);
}

bool rtc_check_alarm(void)
{
	// Alarm 1 flag (A1F) in bit 0
	uint8_t val = rtc_read_byte(0x0f);

	// clear flag when set
	if (val & 1)
		rtc_write_byte(val & 0xfe, 0x0f);

	return val & 1 ? 1 : 0;
}
