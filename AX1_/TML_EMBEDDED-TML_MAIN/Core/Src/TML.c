/*
 * TML.c
 *
 *  Created on: Apr 9, 2024
 *      Author: 91944
 */

#ifndef INC_TML_C_
#define INC_TML_C_
#include "TML.h"
extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart6;
extern UART_HandleTypeDef huart1;
extern TIM_HandleTypeDef htim6;
extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern RTC_HandleTypeDef hrtc;
extern LedHendler ledHendler;

extern uint32_t L1, L2, L3;
int seconds = 0;
int count = 0;
int tick = 0;
int FRAME_NUMBER = 1;
int SAMPLIMG_FREQUENCY = 60;
GPSData L86Data;
char finalData[1024] = { };
//Queue dataQueue;
Stack dataQueue;
uint8_t appAddrs = L2_v2; //we have to change the appAdrrs when new update has to be published
char Packet_Header[] = "MMI123", Hardware_Version[] = "1.0.0",
		Firmware_Version[] = "1.0.11",
//Packet_Type[]="NR",
		Message_ID[] = "1", Packet_Status[] = "L", IMEI[16] = "00000000",
		Vehicle_Reg_No[] = "DL1ABC1234";

float inputVoltage = 0;
float internalVoltage = 0;
bool vehicalBatteryStatus = false;

bool uartCmdStatus = false;
uint8_t CMD_Buf[200] = { };

char updateVersion[4] = { };
extern FLASH_AddressTypeDef write_Address, L86_Read_Address;
char tmp_str[500] = { };
char mqttString[1024] = { };
int8_t signalStrength = 0;
char MNC[20] = { }, MCC[20] = { }, LAC[20] = { }, Celid[20] = { }, Network[30] =
		"NA", temper = 'C';
extern uint64_t CP;
char CP_Data[8] = { 0 };
char tmpSplit[5][100];

void init() {
	init_GPSData(&L86Data);
	initRTC(&hrtc);
	setupPrintUart(&huart6, true);
	getAllConfig();
	SET_FOTA_SIZE("149");
	initQueue(&dataQueue);
	HAL_TIM_Base_Start_IT(&htim6);
	initEvb(&hadc1);
	initIvb(&hadc2);

	printc(MinDelay, "\nTML Set Up Started.....\n\r Version %s",
			Firmware_Version);
	resetEC200: if (setupEc200(&huart1)) {
		printc(MinDelay, "SetupEc200Done\n");
	} else {
		printc(MinDelay, "SetupEc200 Failed\n");
		goto resetEC200;
	}

	resetL86Data();
	SMS_Init();
	TCP_init();

	L86_setup(&huart5);

	printc(MinDelay, "\nTML Set Up Done.....\n\r");
}
float internal = 0;
void loop() {

	GO_TO_SLEEP();
	if (HAL_GetTick() % 10000 <= 100) {
		if (count >= 6) {
			gsmParameterUpdate();
			count = 0;
		}

		count++;
	}

// Read uart Cmd for cota and handle it
	if (getPrintUartCmdStatus()) {
		char cotaResp[200] = { };
		if (SMS_COTA(CMD_Buf, cotaResp)) {
			printc(MinDelay, "SMS COTA Success %s\n", cotaResp);
		} else {
			printc(MinDelay, "SMS COTA Fail\n");
		}
		setPrintUartCmdStatus(false);
	}
// Handle When any SMS come for get information
	if (getMessageStatus())
		handleCotaSMS();
//When update is available it will update the device
	if (getUpdateStatus()) {
		GNSS_OFF();
		update();
		setUpdateStaus(false);
	} else {
		task2();
	}

}
void onTemperCallback() {
	generateData("TA");
}
void handleCotaSMS() {
	clearBuffer(CMD_Buf, sizeof(CMD_Buf));
	SMS_Read(0, CMD_Buf);
	if (strlen(CMD_Buf) > 6) {
		clearBuffer(tmpSplit, sizeof(tmpSplit));
		splitString(CMD_Buf, "\r\n", tmpSplit);
		clearBuffer(CMD_Buf, sizeof(CMD_Buf));
		if (tmpSplit[1] == '\0') {
			printc(MinDelay, "Failed to Parse Number\n");
			return;
		}
		if (tmpSplit[2] == '\0') {
			printc(MinDelay, "Failed to Parse Message");
			clearBuffer(CMD_Buf, sizeof(CMD_Buf));
			strcpy(CMD_Buf, "Failed to Parse Message\n");
			goto sendMsg;
		}
		printc(MinDelay, "Received MSG %s", CMD_Buf);
		if (!SMS_COTA(tmpSplit[2], CMD_Buf)) {
			clearBuffer(CMD_Buf, sizeof(CMD_Buf));
			strcpy(CMD_Buf, "INVALID CMD");
		} else {
			char *tmpData = strdup(CMD_Buf);
			sprintf(CMD_Buf, "Device-%s %s", IMEI, tmpData);
		}
		sendMsg:
		char *numbr = strdup(tmpSplit[1]);

		clearBuffer(tmpSplit, sizeof(tmpSplit));
		splitString(numbr, ",", tmpSplit);
		SMS_Send(&tmpSplit[1][4], CMD_Buf);
		printc(MinDelay, "\n phone No is: %s\n", tmpSplit[1]);
	} else {
		printc(MinDelay, "UNABLE TO Parse Message");
	}
	setMessageStatus(false);

}

char* getUpdateVersion() {
	return updateVersion;
}
void setIMEI(char *imei) {
	if (imei == NULL)
		return;
	clearBuffer(IMEI, sizeof(IMEI));
	strcpy(IMEI, imei);
}
void Sleep_routine() {
	ledHendler.pwrLedState = OFF;
	ledHendler.gsmLedState = OFF;
	ledHendler.gpsLedState = OFF;
	FeedAllConfig();
	generateData("IF");
	generateData("NR");
	GNSS_OFF();
	setSleppStatus(true);
	//task2();
	sendHistory();
}
void wakeUpRoutine() {
	ledHendler.pwrLedState = SOLID;
	ledHendler.gsmLedState = LOW;
	ledHendler.gpsLedState = LOW;
	generateData("IN");
}
void setPrintUartCmdStatus(bool tmpStatus) {
	uartCmdStatus = tmpStatus;
}
bool getPrintUartCmdStatus() {
	return uartCmdStatus;
}
void onPrintUartCmd(char *cmd) {
	clearBuffer(CMD_Buf, sizeof(CMD_Buf));
	strcpy(CMD_Buf, cmd);
	setPrintUartCmdStatus(true);
}
void gsmParameterUpdate() {
	signalStrength = getSignal();
	if (signalStrength == -1) {
		printc(MinDelay, "Fail to get Signal");
		signalStrength = 0;
	}
	clearBuffer(MCC, sizeof(MCC));
	clearBuffer(MNC, sizeof(MNC));
	clearBuffer(LAC, sizeof(LAC));
	clearBuffer(Celid, sizeof(Celid));
	bool tmpStatus = get_MCC_MNC_CellID(MCC, MNC, LAC, Celid);
	if (!tmpStatus) {
		printc(MinDelay, "Don't get MMC  MNC Celid \n");
		strcpy(MCC, "NA");
		strcpy(MNC, "NA");
		strcpy(LAC, "NA");
		strcpy(Celid, "NA");
	}
	clearBuffer(Network, sizeof(Network));
	tmpStatus = getOperatorName(Network);
	Network[strlen(Network) - 1] = '\0';
	if (!tmpStatus) {
		printc(MinDelay, "Don't get NETWRORK \n");
		strcpy(Network, "NA");
		ledHendler.gsmLedState = LOW;
	} else {
		ledHendler.gsmLedState = HEIGH;
	}
}

void makeMessageHeader(char *mqttString, char *Packet_Type) {
	sprintf(mqttString, "$%s,%s,%s,%s,%s,%s,%s,%s", Packet_Header,
			Hardware_Version, Firmware_Version, Packet_Type, Message_ID,
			Packet_Status, IMEI, Vehicle_Reg_No);
}
void addGSMData(char *tmp_str) {

	temper = 'C';
	if (getTemperStatus()) {
		temper = 'O';
	}
	sprintf(tmp_str, "%s,%d,%d,%.2f,%.2f,%c,%d,%s,%s,%s,%s,%s,%s,%s,%s",
			&Network[1], //"Network",
			getIgnition(), //ignition
			vehicalBatteryStatus, inputVoltage, //inputVoltage
			internalVoltage, //"Battery Voltage",
			temper, //"Alert",
			signalStrength, //"SignalStrength",
			MCC, //"MCC",
			MNC, //"MNC",
			LAC, //"LAC",
			Celid, //"Cell ID",
			"1000",	//"Digital Input",
			"10",	//"Digital Output",
			"0.3",	//"Analog Input 1",
			"0.3,"	//"Analog Input 2,"
			);
}

void addGPSData(GPSData *data, char *tmp_str, char *tmpTime, char *tmpDate) {
	sprintf(tmp_str, ",%d,%s,%s,%.6f,%c,%.6f,%c,%.2f,%d,%d,%d,%.2f,%.2f,",
			data->gps_fix, // Convert bool to int
			tmpDate, tmpTime, data->latitude, data->lat_dir, data->longitude,
			data->lon_dir, data->speed, data->heading, data->sat_num, data->alt,
			data->PDOP, data->HDOP);
}

void GPSDataCallback(GPSData *gpsData) {
	clearBuffer(&L86Data, sizeof(GPSData));
	memcpy(&L86Data, gpsData, sizeof(GPSData));
}

bool task2() {
	clearBuffer(finalData, sizeof(finalData));
//	dequeue(&dataQueue, finalData);
	pop(&dataQueue, finalData);

//	 		printc(MinDelay,finalData);
//	 		TCP_Send(finalData);
//	 		MQTT_Publish(finalData);
	if (!TCP_Send(finalData)) {
		finalData[25] = 'H';
		writeL86Data(finalData, strlen(finalData));
		return false;
	} else {
		clearBuffer(finalData, sizeof(finalData));
		dataQueue.top=0;
//		if (readL86Data(finalData, sizeof(finalData))) {
//			if (enqueue(&dataQueue, finalData)) {
//				deleteL86Data();
//			} else {
//				printc(MinDelay, "Queue is Full");
//			}
//		} else {
//			printc(MinDelay, "NAND FLASH is empty\n");
//		}
	}
	//TCP_Send(finalData);
	clearBuffer(finalData, sizeof(finalData));
	printc(MinDelay, "Data sent");
return true;
}

void sendHistory(){
	clearBuffer(finalData, sizeof(finalData));
	//dataQueue.top=0;
	while(readL86Data(finalData, sizeof(finalData))){
		push(&dataQueue, finalData, strlen(finalData));
		if(!task2()){
			break;
		}
//			if (readL86Data(finalData, sizeof(finalData))) {
//				if (enqueue(&dataQueue, finalData)) {
//					deleteL86Data();
//				} else {
//					printc(MinDelay, "Queue is Full");
//				}
//			} else {
//				printc(MinDelay, "NAND FLASH is empty\n");
//			}
	}
}

void generateData(char *packetType) {
	memset(mqttString, 0, sizeof(mqttString));
	makeMessageHeader(mqttString, packetType);
	char tmpTime[7] = { 0 };
	char tmpDate[9] = { 0 };
	getDateTime(tmpDate, tmpTime);
	printc(MinDelay, "\n\nTime: %s \r\n",&tmpTime);
	printc(MinDelay, "\n\n\Date: %s \r\n",&tmpDate);

//add GPS DATA
	clearBuffer(tmp_str, sizeof(tmp_str));
	addGPSData(&L86Data, tmp_str, tmpTime, tmpDate);
	strcat(mqttString, tmp_str);

	memset(tmp_str, 0, sizeof(tmp_str));
	inputVoltage = 0;
	vehicalBatteryStatus = false;
	if (getInputVoltage(&inputVoltage)) {
		if (inputVoltage > 7) {
			vehicalBatteryStatus = true;
		}
	}
	internalVoltage = 0;
	getInternalBattery(&internalVoltage);
	addGSMData(tmp_str);
	strcat(mqttString, tmp_str);
	memset(tmp_str, 0, sizeof(tmp_str));
	Map copyCanData;
	getCANData(&copyCanData);
	char canStr[NUM_PID * 8 * 8] = { };
	for (int i = 0; i < copyCanData.size; i++) {
		char tmpCan[32] = { };
		sprintf(tmpCan, "%04X:", copyCanData.pairs[i].key);
		for (uint8_t x = 0; x < 8; x++)
			sprintf(tmpCan + strlen(tmpCan), "%02X",
					copyCanData.pairs[i].value[x]);
		if (i + 1 != copyCanData.size) {
			strcat(tmpCan, ";");
		}
		strcat(canStr, tmpCan);
	}
	strcat(mqttString, canStr);
	strcat(mqttString, ",()"); //OTA
	sprintf(mqttString + strlen(mqttString), ",%d", FRAME_NUMBER);
	uint8_t checksum = Calculate_CRC8(&mqttString[1], strlen(mqttString) - 1);
	sprintf(mqttString + strlen(mqttString), "*%02X", checksum);
	printc(1000, "Data To Send\n%s\n", mqttString);
	//enqueue(&dataQueue, mqttString);
	push(&dataQueue, mqttString, strlen(mqttString));
	FRAME_NUMBER++;

}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim6) {

	if (htim6->Instance == TIM6) {
		seconds++;
	}
	blinkLed(seconds);
	if (getSleepStatus())
		return;
//	Setting Up sampling frequency if we have latitude and longitude
	if ((seconds / (100 * SAMPLIMG_FREQUENCY)) > 0) {
		if (!getUpdateStatus() && L86Data.latitude != 0
				&& L86Data.longitude != 0) {
			ledHendler.gpsLedState = HEIGH;
			generateData("NR");
		} else {
			ledHendler.gpsLedState = LOW;
		}
		seconds = 0;
	}
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {

	if (huart->Instance == UART5) {
		L86_RxCpltCallback();
	} else if (huart->Instance == USART1) {
		void E200_RXCpltCallback();
	}
}
void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == UART5) {
		L86_RxHalfCpltCallback();
	} else if (huart->Instance == USART1) {
		E200_RxHAlfCpltCallback();
	}
}
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
	if (huart->Instance == UART5) {
//			L86_RxCpltCallback();
	} else if (huart->Instance == USART1) {
		Ec200_RxEventCallback(Size);
	} else if (huart->Instance == USART6) {
		print_RxEventCallback(Size);
	}

}
void setSampling(int sampling) {
	SAMPLIMG_FREQUENCY = sampling;
}

#endif /* INC_TML_C_ */
