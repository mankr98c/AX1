/*
 * gpio_handler.c
 *
 *  Created on: May 9, 2024
 *      Author: 91944
 */

#include"gpio_handler.h"
#include "SMS_COTA.h"
ADC_HandleTypeDef *evbSense, *ivbSense;
#define REFERANCE_VOLTAGE 11
#define INTERNAL_BATTERY 1.9
RTC_HandleTypeDef *rtc;
RTC_TimeTypeDef sTime;
RTC_DateTypeDef sDate;
bool sleepStatus = false;
extern RTC_HandleTypeDef hrtc;
extern TIM_HandleTypeDef htim6;
extern int FLAG_RESET_On_Sleep;
extern bool sendPDDataFlag;
int FLAG_RESET_On_Sleep =0;
uint32_t WRT_Time = 0x57E40; //0x36EE80;
bool getSleepStatus() {
	return sleepStatus;
}
void setSleppStatus(bool sleep) {
	sleepStatus = sleep;
}
void initEvb(ADC_HandleTypeDef *adc) {
	evbSense = adc;
}
void initIvb(ADC_HandleTypeDef *adc) {
	ivbSense = adc;
}

bool getInputVoltage(float *opADC) {
	uint16_t raw = 0;
	// Wait for ADC conversion to complete
	HAL_ADC_Start(evbSense);
	if (HAL_ADC_PollForConversion(evbSense, 2000) == HAL_OK) {
		// Read ADC value
		raw = HAL_ADC_GetValue(evbSense);
		*opADC = (raw * 3.3 * REFERANCE_VOLTAGE) / ((1 << 12) - 1);
		// Stop ADC conversion
		HAL_ADC_Stop(evbSense);
		return true;
	}
	return false;
}
bool getInternalBattery(float *opADC) {
	uint16_t raw = 0;
	// Wait for ADC conversion to complete
	HAL_ADC_Start(ivbSense);
	if (HAL_ADC_PollForConversion(ivbSense, 2000) == HAL_OK) {
		// Read ADC value
		raw = HAL_ADC_GetValue(ivbSense);
		*opADC = (raw * 3.3 * INTERNAL_BATTERY) / ((1 << 12) - 1);
		// Stop ADC conversion
		HAL_ADC_Stop(evbSense);
		return true;
	}
	return false;
}

bool getIgnition() {
	return LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_0);
}

bool getTemperStatus() {
	return LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_15);
}

void initRTC(RTC_HandleTypeDef *hrtc) {
	rtc = hrtc;
}
bool setTime(uint8_t hours, uint8_t minute, uint8_t second) {
	clearBuffer((uint8_t*) &sTime, sizeof(sTime));
	sTime.Hours = hours;
	sTime.Minutes = minute;
	sTime.Seconds = second;
	if (HAL_RTC_SetTime(rtc, &sTime, RTC_FORMAT_BIN) != HAL_OK) {
		// Handle error
		return false;
	}
	return true;
}
bool setDate(uint8_t date, uint8_t month, uint8_t year) {
	clearBuffer((uint8_t*) &sDate, sizeof(sDate));
	sDate.Date = date;
	sDate.Month = month;
	sDate.Year = year;
	if (HAL_RTC_SetDate(rtc, &sDate, RTC_FORMAT_BIN) != HAL_OK) {
		return false;
	}
	return true;
}
bool getDateTime(char *opDate, char *opTime) {
	clearBuffer((uint8_t*) &sTime, sizeof(sTime));
	clearBuffer((uint8_t*) &sDate, sizeof(sDate));
	if (HAL_RTC_GetTime(rtc, &sTime, RTC_FORMAT_BIN) != HAL_OK) {
		return false;
	}
	if (HAL_RTC_GetDate(rtc, &sDate, RTC_FORMAT_BIN) != HAL_OK) {
		return false;
	}

	sprintf(opDate, "%02d%02d20%02d", sDate.Date, sDate.Month, sDate.Year);
	sprintf(opTime, "%02d%02d%02d", sTime.Hours, sTime.Minutes, sTime.Seconds);
	return true;
}

void config_IT() {

	HAL_NVIC_DisableIRQ(EXTI8_IRQn);
	HAL_NVIC_SetPriority(EXTI8_IRQn, 0, 0);
	HAL_NVIC_DisableIRQ(EXTI15_IRQn);
	HAL_NVIC_SetPriority(EXTI15_IRQn, 0, 0);
	HAL_NVIC_DisableIRQ(TIM6_IRQn);
	HAL_NVIC_SetPriority(TIM6_IRQn, 0, 0);
	HAL_NVIC_DisableIRQ(USART1_IRQn);
	HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
	HAL_NVIC_DisableIRQ(UART5_IRQn);
	HAL_NVIC_SetPriority(UART5_IRQn, 0, 0);
	HAL_NVIC_SetPriority(FDCAN2_IT0_IRQn, 0, 0);
	HAL_NVIC_DisableIRQ(FDCAN2_IT0_IRQn);
	HAL_NVIC_SetPriority(GPDMA1_Channel4_IRQn, 0, 0);
	HAL_NVIC_DisableIRQ(GPDMA1_Channel4_IRQn);

	HAL_NVIC_SetPriority(FDCAN1_IT0_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(FDCAN1_IT0_IRQn);
	HAL_NVIC_EnableIRQ(USART6_IRQn);
	HAL_NVIC_SetPriority(USART6_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(RTC_IRQn);
	HAL_NVIC_SetPriority(RTC_IRQn, 0, 0);

}
void On_Wakeup_Config() {

	HAL_NVIC_EnableIRQ(EXTI8_IRQn);
	HAL_NVIC_SetPriority(EXTI8_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI15_IRQn);
	HAL_NVIC_SetPriority(EXTI15_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(TIM6_IRQn);
	HAL_NVIC_SetPriority(TIM6_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART1_IRQn);
	HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(UART5_IRQn);
	HAL_NVIC_SetPriority(UART5_IRQn, 0, 0);
	HAL_NVIC_SetPriority(FDCAN2_IT0_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(FDCAN2_IT0_IRQn);
	HAL_NVIC_SetPriority(GPDMA1_Channel4_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(GPDMA1_Channel4_IRQn);
	HAL_NVIC_SetPriority(FDCAN1_IT0_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(FDCAN1_IT0_IRQn);
	HAL_NVIC_EnableIRQ(USART6_IRQn);
	HAL_NVIC_SetPriority(USART6_IRQn, 0, 0);
	HAL_NVIC_DisableIRQ(RTC_IRQn);
	HAL_NVIC_SetPriority(RTC_IRQn, 0, 0);

}

void Configure_RTC_WakeUp_Timer() {

	if (HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 1800,
	RTC_WAKEUPCLOCK_CK_SPRE_16BITS, 0) != HAL_OK) {
		Error_Handler();
	}


}
void GO_TO_SLEEP() {
	if (LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_0) == RESET) {
		printc(MinDelay, "Sleep_routine\r\n");
		FLAG_RESET_On_Sleep =1;
		Sleep_routine();


		printc(MinDelay, "Sleep_Mode\r\n");
		HAL_NVIC_SystemReset();
	}

}

// Function to disable EXTI line 0 interrupt
void Disable_EXTI0() {
	// Disable EXTI0 interrupt
	HAL_NVIC_DisableIRQ(EXTI0_IRQn);
	// Clear any pending EXTI0 interrupt
	LL_EXTI_ClearRisingFlag_0_31(LL_EXTI_EXTI_LINE0);

}

void SL_WK_GPIO_Int() {

	LL_EXTI_InitTypeDef EXTI_InitStruct = { 0 };
	LL_EXTI_SetEXTISource(LL_EXTI_EXTI_PORTA, LL_EXTI_EXTI_LINE0);

	EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_0;
	EXTI_InitStruct.Line_32_63 = LL_EXTI_LINE_NONE;
	EXTI_InitStruct.LineCommand = ENABLE;
	EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
	EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING;
	LL_EXTI_Init(&EXTI_InitStruct);
	LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_0, LL_GPIO_PULL_NO);
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_0, LL_GPIO_MODE_INPUT);
	NVIC_SetPriority(EXTI0_IRQn,
			NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
	NVIC_EnableIRQ(EXTI0_IRQn);
}
//void WAKEUP() {
////	HAL_ResumeTick();
////	Turn_ON_Modules();
////	HAL_Delay(5000);
//	On_Wakeup_Config();
//	printc(MinDelay, "\r\nWokeUp\r\n");
////	init();
//	Disable_EXTI0();
//	wakeUpRoutine();
//}

void Turn_OFF_Modules() {
	GNSS_OFF();
	GPIO_LTE_OFF();
	GPS_LIGHT_OFF();
	PWR_LIGHT_OFF();
	GSM_LIGHT_OFF();
	GPIO_3V3_OFF();
}

void Turn_ON_Modules() {

	GPIO_3V3_ON();
	Handle_LEDS();
	GNSS_ON();
	GPIO_LTE_ON();

}
void Sleep_Mode()
{
	char SF[8] = { 0 };
	FLASH_Read_Buff(FL_On_Sl_Addr, SF, 8);
	FLAG_RESET_On_Sleep = SF[0];
	if (FLAG_RESET_On_Sleep == 1) {
		sendPDDataFlag = true;
//		setSleppStatus(true);
		config_IT();
		SL_WK_GPIO_Int();
		Configure_RTC_WakeUp_Timer();
		__HAL_RTC_WAKEUPTIMER_ENABLE(&hrtc);
		Turn_OFF_Modules();
		HAL_PWR_EnableSleepOnExit();
		HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);

		getAllConfig();
		FLAG_RESET_On_Sleep = 0;
		FeedAllConfig();
		Disable_EXTI0();
		printc(MinDelay, "\r\nWokeUp\r\n");
//		setSleppStatus(false);
	}
}
