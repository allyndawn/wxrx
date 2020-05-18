/**
 * lcd.h
 * Allen Snook
 * May 18, 2020
 */

#ifndef __LCD_H
#define __LCD_H

#include "stm32f4xx_hal.h"
#include "cmsis_os.h"

void LCD_Set_UART( UART_HandleTypeDef *huart );
void LCD_Set_Message_Queue( osMessageQueueId_t hqueue );
void LCD_Set_RTC( RTC_HandleTypeDef *hrtc );
void LCD_Init();
void LCD_Run();

#endif // __LCD_H
