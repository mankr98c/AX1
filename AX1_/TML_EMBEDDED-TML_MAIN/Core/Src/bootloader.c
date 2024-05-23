#include "main.h"

extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart6;
extern UART_HandleTypeDef huart1;
extern TIM_HandleTypeDef htim6;
extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern RTC_HandleTypeDef hrtc;
extern FDCAN_HandleTypeDef hfdcan1;
extern FDCAN_HandleTypeDef hfdcan2;
extern CRYP_HandleTypeDef hcryp;

typedef void (*pFunction)(void);

unsigned long CP_Address;

uint32_t L1;    // Sector 0-15  or 0-31
uint32_t L2;	 // Sector 16-31 or 32 to 64
uint32_t L3;  // Sector 32-48   or 64 to 96
extern uint32_t VT_OFF;
pFunction Jump_To_Application;

extern uint64_t CP;
extern int LCP;

void DE_INIT_Func() {
	HAL_Init();
//		    // Deinitialize GPIOs
	MX_UART5_Init();
	MX_USART6_UART_Init();
	MX_USART1_UART_Init();
	MX_FDCAN2_Init();
	MX_FDCAN1_Init();
	MX_ADC1_Init();
	MX_ADC2_Init();
	MX_RTC_Init();
	 MX_TIM6_Init();
	LL_GPIO_DeInit(GPIOA);
	LL_GPIO_DeInit(GPIOB);
	LL_GPIO_DeInit(GPIOC);
	LL_DMA_DeInit(GPDMA1, LL_DMA_CHANNEL_4);
	HAL_UART_DeInit(&huart5);
	HAL_UART_DeInit(&huart6);
	HAL_UART_DeInit(&huart1);
	HAL_ICACHE_Disable();
	HAL_ICACHE_DeInit();
	HAL_FDCAN_DeInit(&hfdcan1);
	HAL_FDCAN_DeInit(&hfdcan2);
	HAL_TIM_Base_DeInit(&htim6);
	LL_SPI_DeInit(SPI3);
	HAL_ADC_DeInit(&hadc1);
	HAL_ADC_DeInit(&hadc2);
	HAL_RTC_DeInit(&hrtc);
	LL_RCC_DeInit();
__HAL_RCC_APB1_FORCE_RESET();
__HAL_RCC_APB3_FORCE_RESET();
__HAL_RCC_APB2_FORCE_RESET();
}

void Application_jump() {
	SGET_ALOC();
	if (CP == 2) {
		printc(MinDelay,"\n\rBootloader Function For Location2\n\r");
		void (*app_reset_handler)(
				void) = (void (*)(void))(*((volatile uint32_t*) (L2 + 4U)));


		// Reset the SysTick
				SysTick->CTRL = 0;
				SysTick->LOAD = 0;
				SysTick->VAL = 0;


				// Set the main stack pointer to the application's stack pointer
				__set_MSP(*(volatile uint32_t*) L2);
				VT_OFF = 0x40000;


				// Jump to the application's reset handler
				app_reset_handler();
			}
	else if (CP == 3) {

				void (*app_reset_handler)(
						void) = (void*)(*((volatile uint32_t*) (L3 + 4U)));

		printc(MinDelay,"\n\rBootloader running into @Location3\n\r");


				//	  __set_MSP(*(volatile uint32_t*) 0x081E0000);
				SysTick->CTRL = 0;
				SysTick->LOAD = 0;
				SysTick->VAL = 0;
				__set_MSP(*(volatile uint32_t*) L3);
				VT_OFF = 0x80000;
				/* Jump to application */
				app_reset_handler();
			}
			else if (CP == 1) {
				printc(MinDelay, "\n\rBootloader Function For Golden Image Location\n\r");
		}
			else {
				writeF_CP(1);
				printc(MinDelay,"Not Updating");
			}
		}

