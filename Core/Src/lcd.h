/**
 * lcd.h
 * Allen Snook
 * May 18, 2020
 */

#ifndef __LCD_H
#define __LCD_H

#include "stm32f4xx_hal.h"
#include "cmsis_os.h"

typedef struct {
	int16_t temperature; // deg C, 0.1 deg res, -90 (-900) to +140 (+1400) deg C
	uint16_t pressure; // mbar, 0.1 mbar res, 870 (8700) to 1100 (11000) mbar
	uint16_t humidity; // percent, 0.1 percent res, 0 to 100 (1000) perfect
	uint8_t latitude_degrees;	// 0 to 90
	uint8_t latitude_minutes;	// 0 to 59
	uint8_t latitude_seconds;	// 0 to 59
	char latitude_hem;			// N or S
	uint8_t longitude_degrees;	// 0 to 180
	uint8_t longitude_minutes;	// 0 to 59
	uint8_t longitude_seconds;	// 0 to 59
	char longitude_hem;			// W or E
	int8_t rssi;				// -115 to 0 dBm, at receiver
} lcd_combined_data_type; // 15 bytes

void LCD_Set_UART( UART_HandleTypeDef *huart );
void LCD_Set_Message_Queue( osMessageQueueId_t hqueue );
void LCD_Set_RTC( RTC_HandleTypeDef *hrtc );
void LCD_Init();
void LCD_Run();

#endif // __LCD_H
