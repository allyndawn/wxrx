/**
 * core.c
 * Allen Snook
 * May 28, 2020
 */

#include "core.h"
#include "lcd.h"

typedef struct {
	uint8_t length;
	uint8_t dest_addr;
	uint8_t src_addr;
	uint8_t control;
	int16_t temperature; // deg C, 0.1 deg res, -90 (-900) to +140 (+1400) deg C
	uint16_t pressure; // mbar, 0.1 mbar res, 870 (8700) to 1100 (11000) mbar
	uint16_t humidity; // percent, 0.1 percent res, 0 to 100 (1000) perfect
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
} core_combined_data_type; // 24 bytes

static RTC_HandleTypeDef *core_hrtc;
static osMessageQueueId_t core_lcd_hqueue;
static osMessageQueueId_t core_radio_hqueue;
static HAL_StatusTypeDef core_hal_status;
static osStatus_t core_os_status;

static core_combined_data_type core_combined_data;
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

	rtc_time.Hours = core_combined_data.hour;
	rtc_time.Minutes = core_combined_data.minutes;
	rtc_time.Seconds = core_combined_data.seconds;
	rtc_time.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	rtc_time.StoreOperation = RTC_STOREOPERATION_RESET;
	core_hal_status = HAL_RTC_SetTime( core_hrtc, &rtc_time, RTC_FORMAT_BIN );
	if ( core_hal_status != HAL_OK ) {
		return;
	}
	rtc_date.WeekDay = RTC_WEEKDAY_MONDAY; // TODO
	rtc_date.Month = core_combined_data.month;
	rtc_date.Date = core_combined_data.day;
	rtc_date.Year = core_combined_data.year;

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
	lcd_combined_data.temperature = core_combined_data.temperature;
	lcd_combined_data.pressure = core_combined_data.pressure;
	lcd_combined_data.humidity = core_combined_data.humidity;

	lcd_combined_data.latitude_degrees = core_combined_data.latitude_degrees;
	lcd_combined_data.latitude_minutes = core_combined_data.latitude_minutes;
	lcd_combined_data.latitude_seconds = core_combined_data.latitude_seconds;
	lcd_combined_data.latitude_hem = core_combined_data.latitude_hem;

	lcd_combined_data.longitude_degrees = core_combined_data.longitude_degrees;
	lcd_combined_data.longitude_minutes = core_combined_data.longitude_minutes;
	lcd_combined_data.longitude_seconds = core_combined_data.longitude_seconds;
	lcd_combined_data.longitude_hem = core_combined_data.longitude_hem;

	// Send it
	osMessageQueuePut( core_lcd_hqueue, (void *) &(lcd_combined_data), 0U, 0U );
}

void _Core_Handle_Radio_Queue() {
	// Do we have a radio message queue handle?
	if ( ! core_radio_hqueue ) {
		return;
	}

	// Listen for radio messages
	core_hal_status = osMessageQueueGet( core_radio_hqueue, (void *) &core_combined_data, NULL, 0U );
	if ( core_os_status == osOK ) {
		_Core_Update_RTC();
		_Core_Update_LCD();
	}
}

void Core_Run() {
	_Core_Handle_Radio_Queue();
	osDelay( 250 );
}
