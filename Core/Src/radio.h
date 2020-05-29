/**
 * radio.h
 * Allen Snook
 * May 21, 2020
 */

#ifndef __RADIO_H
#define __RADIO_H

#include "stm32f4xx_hal.h"
#include "cmsis_os.h"

typedef struct {
	uint8_t length;
	uint8_t dest_addr;
	uint8_t src_addr;
	uint8_t control;
	int16_t temperature;		// deg C, 0.1 deg res, -90 (-900) to +140 (+1400) deg C
	uint16_t pressure;			// mbar, 0.1 mbar res, 870 (8700) to 1100 (11000) mbar
	uint16_t humidity;			// percent, 0.1 percent res, 0 to 100 (1000) perfect
	uint8_t year;				// Years since 2000
	uint8_t month;				// 1 to 12
	uint8_t day;				// 1 to 31
	uint8_t hour;				// 0 to 23
	uint8_t minutes;			// 0 to 59
	uint8_t seconds;			// 0 to 59
	uint8_t latitude_degrees;	// 0 to 90
	uint8_t latitude_minutes;	// 0 to 59
	uint8_t latitude_seconds;	// 0 to 59
	char latitude_hem;			// N or S
	uint8_t longitude_degrees;	// 0 to 180
	uint8_t longitude_minutes;	// 0 to 59
	uint8_t longitude_seconds;	// 0 to 59
	char longitude_hem;			// W or E
	int8_t rssi;				// dBm -115 to 0 (Overwritten by receiver)
} radio_packet_data_type; // 25 bytes

void Radio_Set_SPI( SPI_HandleTypeDef *spi );
void Radio_Set_Reset_Pin( GPIO_TypeDef* gpio, uint16_t pin );
void Radio_Set_NCS_Pin( GPIO_TypeDef* gpio, uint16_t pin );

void Radio_Set_Message_Queue( osMessageQueueId_t hqueue );

uint8_t Radio_Init();
void Radio_Run();

#endif // __RADIO_H
