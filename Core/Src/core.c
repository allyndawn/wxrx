/**
 * core.c
 * Allen Snook
 * May 28, 2020
 */

#include "core.h"
#include "lcd.h"
#include "radio.h"

static RTC_HandleTypeDef *core_hrtc;
static osMessageQueueId_t core_lcd_hqueue;
static osMessageQueueId_t core_radio_hqueue;
static HAL_StatusTypeDef core_hal_status;
static osStatus_t core_os_status;

static radio_packet_data_type radio_packet;
static lcd_combined_data_type lcd_combined_data;

void Core_Set_RTC_Handle( RTC_HandleTypeDef *hrtc ) {
	core_hrtc = hrtc;
}

void Core_Set_LCD_Message_Queue( osMessageQueueId_t hqueue ) {
	core_lcd_hqueue = hqueue;
}

void Core_Set_Radio_Message_Queue( osMessageQueueId_t hqueue ) {
	core_radio_hqueue = hqueue;
}

void _Core_Update_RTC() {
	RTC_TimeTypeDef rtc_time = {0};
	RTC_DateTypeDef rtc_date = {0};

	rtc_time.Hours = radio_packet.hour;
	rtc_time.Minutes = radio_packet.minutes;
	rtc_time.Seconds = radio_packet.seconds;
	rtc_time.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	rtc_time.StoreOperation = RTC_STOREOPERATION_RESET;
	core_hal_status = HAL_RTC_SetTime( core_hrtc, &rtc_time, RTC_FORMAT_BIN );
	if ( core_hal_status != HAL_OK ) {
		return;
	}
	rtc_date.WeekDay = RTC_WEEKDAY_MONDAY; // TODO
	rtc_date.Month = radio_packet.month;
	rtc_date.Date = radio_packet.day;
	rtc_date.Year = radio_packet.year;

	core_hal_status = HAL_RTC_SetDate( core_hrtc, &rtc_date, RTC_FORMAT_BIN );
	if ( core_hal_status != HAL_OK ) {
		return;
	}
}

void _Core_Update_LCD() {
	// Do we have a lcd message queue handle?
	if ( ! core_lcd_hqueue ) {
		return;
	}

	// Copy the appropriate values from the core data into the lcd data struct
	// Note that the RTC will take care of the date and time
	lcd_combined_data.temperature = radio_packet.temperature;
	lcd_combined_data.pressure = radio_packet.pressure;
	lcd_combined_data.humidity = radio_packet.humidity;

	lcd_combined_data.latitude_degrees = radio_packet.latitude_degrees;
	lcd_combined_data.latitude_minutes = radio_packet.latitude_minutes;
	lcd_combined_data.latitude_seconds = radio_packet.latitude_seconds;
	lcd_combined_data.latitude_hem = radio_packet.latitude_hem;

	lcd_combined_data.longitude_degrees = radio_packet.longitude_degrees;
	lcd_combined_data.longitude_minutes = radio_packet.longitude_minutes;
	lcd_combined_data.longitude_seconds = radio_packet.longitude_seconds;
	lcd_combined_data.longitude_hem = radio_packet.longitude_hem;

	lcd_combined_data.rssi = radio_packet.rssi;

	// Send it
	osMessageQueuePut( core_lcd_hqueue, (void *) &(lcd_combined_data), 0U, 0U );
}

void _Core_Handle_Radio_Queue() {
	// Do we have a radio message queue handle?
	if ( ! core_radio_hqueue ) {
		return;
	}

	// Listen for radio messages
	core_os_status = osMessageQueueGet( core_radio_hqueue, (void *) &radio_packet, NULL, 0U );
	if ( core_os_status == osOK ) {
		_Core_Update_RTC();
		_Core_Update_LCD();
	}
}

void Core_Run() {
	_Core_Handle_Radio_Queue();
	osDelay( 250 );
}
