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
int seconds = 0, nsam_seconds = 0;
double latHitory = 0, lonHistory = 0;
int count = 0;
int sleepcount = 0;
int FRAME_NUMBER = 1;
int SAMPLIMG_FREQUENCY = 60;
GPSData L86Data;
char finalData[1024] = { };
char historyData[1024] = { };
uint64_t SP;
bool SendIFDataFlag = false;
//Queue dataQueue;
Stack dataQueue;
Stack packetHistoryQueue;
bool interflag = false;
bool sendPDDataFlag = false;
//Queue dataQueue;
uint8_t appAddrs = L2_v2; //we have to change the appAdrrs when new update has to be published
char Packet_Header[] = "MMI123", Hardware_Version[] = "1.0.0",
		Firmware_Version[] = "5.0.16",    // Updating FLash Base Address
//Packet_Type[]="NR",
		Message_ID[] = "1", Packet_Status[] = "L", IMEI[16] = "00000000",
		Vehicle_Reg_No[] = "DL1ABC1234";
int FLAG_IGN = 0;

float inputVoltage = 0;
float internalVoltage = 0;
bool vehicalBatteryStatus = false;
extern bool SRESET_FLAG;
extern bool updateStatus;
bool uartCmdStatus = false;
uint8_t CMD_Buf[200] = { };

char updateVersion[4] = { };
extern FLASH_AddressTypeDef write_Address, L86_Read_Address;
char tmp_str[500] = { };
char mqttString[1024] = { };
int8_t signalStrength = 0;
char MNC[20] = { }, MCC[20] = { }, LAC[20] = { }, Celid[20] = { }, Network[30] =
		"NA", temper = 'C';
extern uint64_t CP, CD;
char CP_Data[8] = { 0 };
char tmpSplit[5][100];
char PHnum[10] = { 0 };
void init() {
	init_GPSData(&L86Data);
	initRTC(&hrtc);
	setupPrintUart(&huart6, true);
	clear_RConfiguration();
	getAllConfig();
	SGET_LAT(&latHitory);
	SGET_LON(&lonHistory);
	initializeStack(&dataQueue);
	initializeStack(&packetHistoryQueue);
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
	SET_FOTA_SIZE("155");

	SET_FOTA_UPDATE("5.0.9");
//	updateStatus = true;
//		SET_SKEY3("1D68A17E0EE8525CC9876BD573C9CDCC");
//		SET_SKEY2("967B9E42EC8F3E5F3D2DE0A5003F9129");
//		SET_SKEY1("0F402D78266379087F7C1E0A02AD1D6D");

	SMS_Init();
	Check_USMS();
	if (SP == 1) {
		if (PHnum != NULL) {
			char SmBuff[] = { "FOTA Update Complete" };
			SMS_FSend(PHnum, SmBuff);
			writeF_CP((uint64_t) 1, (uint64_t) 0, (uint64_t) 0);
		}
	}
	TCP_init();
	initCan();
	L86_setup(&huart5);

	printc(MinDelay, "\nTML Set Up Done.....\n\r");
}
float internal = 0;
void loop() {

//	Every 1 minute will check for gsmParameter and Go to sleep when ignition is off
	if (HAL_GetTick() % 10000 == 0) {
		if (count >= 6) {
			printc(MinDelay, "Came inside for gsm update ");
			gsmParameterUpdate();
			count = 0;

			//will
		}

		count++;
	}
	if (sleepcount > 6) {
		GO_TO_SLEEP();
		sleepcount = 0;
	}
	if (FLAG_IGN == 1) {
		generateData("IN");
		sendPDDataFlag = true;
		task2();
		FLAG_IGN = 0;
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
//		char SBf[]={"FOTA_Updated"};
//		SMS_Send(PHnum,SBf );
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
		sendMsg: char *number = strdup(tmpSplit[1]);

		clearBuffer(tmpSplit, sizeof(tmpSplit));
		splitString(number, ",", tmpSplit);
		SMS_Send(&tmpSplit[1][4], CMD_Buf);
		if (tmpSplit[1][2] != NULL) {
			memset(PHnum, 0, sizeof(PHnum));
			memcpy(PHnum, &tmpSplit[1][4], 10);
		}
		printc(MinDelay, "\n phone No is: %s\n", tmpSplit[1]);

	} else {
		printc(MinDelay, "UNABLE TO Parse Message");
	}
	setMessageStatus(false);
	if (SRESET_FLAG == true) {
		SET_RESET();
	}

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
//	ledHendler.pwrLedState=OFF;
//	ledHendler.gsmLedState=OFF;
//	ledHendler.gpsLedState=OFF;
	char tmpVal[15] = { };
		char tmpVal1[15] = { };
		sprintf(tmpVal1, "%.6f", latHitory);
		sprintf(tmpVal, "%.6f", lonHistory);
		SET_LAT_LON(tmpVal1,tmpVal);
	FeedAllConfig();

	task2();
//	GNSS_OFF();
//	sendPDDataFlag = true;
//	setSleppStatus(true);

}
void Handle_LEDS() {
	ledHendler.pwrLedState = SOLID;
	ledHendler.gsmLedState = LOW;
	ledHendler.gpsLedState = LOW;
}
//void wakeUpRoutine() {
//	ledHendler.pwrLedState=SOLID;
//	ledHendler.gsmLedState=LOW;
//	ledHendler.gpsLedState=LOW;
//	setSleppStatus(false);
//	if (FLAG_IGN == 1) {
//		generateData("IN");
//		sendPDDataFlag = true;
//		task2();
//		FLAG_IGN = 0;
//	}
//
//}
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
			&Network[1],    //"Network",
			getIgnition(),    //ignition
			vehicalBatteryStatus, inputVoltage,    //inputVoltage
			internalVoltage,    //"Battery Voltage",
			temper,    //"Alert",
			signalStrength,    //"SignalStrength",
			MCC,    //"MCC",
			MNC,    //"MNC",
			LAC,    //"LAC",
			Celid,    //"Cell ID",
			"1000",	//"Digital Input",
			"10",	//"Digital Output",
			"0.3",	//"Analog Input 1",
			"0.3,"	//"Analog Input 2,"
			);
}

void addGPSData(GPSData *data, char *tmp_str, char *tmpTime, char *tmpDate) {
	if (data->latitude != 0 && data->longitude != 0) {
		latHitory = data->latitude;
		lonHistory = data->longitude;
	}
	else{
		printc(MinDelay,"geting lat 0 long 0 at time %s\n",tmpTime);
	}
	sprintf(tmp_str, ",%d,%s,%s,%.6f,%c,%.6f,%c,%.2f,%d,%d,%d,%.2f,%.2f,",
			data->gps_fix, // Convert bool to int
			tmpDate, tmpTime, latHitory, data->lat_dir, lonHistory,
			data->lon_dir, data->speed, data->heading, data->sat_num, data->alt,
			data->PDOP, data->HDOP);
}

void GPSDataCallback(GPSData *gpsData) {
	clearBuffer(&L86Data, sizeof(GPSData));
	memcpy(&L86Data, gpsData, sizeof(GPSData));
}
void task2() {

	int Flen = strlen(Firmware_Version);
	if (sendPDDataFlag) {
		clearBuffer(finalData, sizeof(finalData));
		if (dataQueue.top >= 0) {
			pop(&dataQueue, finalData);

			//send the rest of the data in the dataqueue to history queue
			while (dataQueue.top >= 0) {
				clearBuffer(historyData, sizeof(historyData));
				pop(&dataQueue, historyData);
				historyData[20 + Flen] = 'H';
				push(&packetHistoryQueue, historyData, strlen(historyData));
				//writeL86Data(finalData, strlen(finalData));

			}
			if (!TCP_Send(finalData)) {
				finalData[20 + Flen] = 'H';
				push(&packetHistoryQueue, finalData, strlen(finalData));
				//writeL86Data(finalData, strlen(finalData));

			}

			sendPDDataFlag = false;
		}
	}

	if (interflag) {

		if (packetHistoryQueue.top >= 0) {
			clearBuffer(finalData, sizeof(finalData));
			pop(&packetHistoryQueue, finalData);
			if (!TCP_Send(finalData)) {
				finalData[20 + Flen] = 'H';
				push(&packetHistoryQueue, finalData, strlen(finalData));

				//check for historyqueue overflow

			}

			interflag = false;
		}

	}

}

void generateData(char *packetType) {
	memset(mqttString, 0, sizeof(mqttString));
	makeMessageHeader(mqttString, packetType);
	char tmpTime[7] = { 0 };
	char tmpDate[9] = { 0 };
	getDateTime(tmpDate, tmpTime);
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
		nsam_seconds++;
	}

	blinkLed(seconds);
	if (getSleepStatus())
		return;
//	Setting Up sampling frequency if we have latitude and longitude
	if ((seconds / (100 * SAMPLIMG_FREQUENCY)) > 0) {
		//generateData("NR");
		if (!getUpdateStatus()) {
			ledHendler.gpsLedState = HEIGH;
			generateData("NR");
		} else {
			ledHendler.gpsLedState = LOW;

		}
		seconds = 0;
		if (!getIgnition()) {
			if (sleepcount == 0 && SendIFDataFlag) {
				generateData("IF");
				SendIFDataFlag == false;
			}
			sleepcount++;
		} else {
			sleepcount = 0;
			SendIFDataFlag == true;
		}
	}
	//gen data greater than, right now considering same sampling freq
	if ((nsam_seconds / (100 * SAMPLIMG_FREQUENCY)) > 0) {
		sendPDDataFlag = true;
		interflag = false;
		nsam_seconds = 0;
	}
	if (!((nsam_seconds > ((SAMPLIMG_FREQUENCY - 3) * 100)) || (sendPDDataFlag))) {
		interflag = true;
	} else {
		interflag = false;
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

void Check_USMS() {
	char CP_Data[8] = { 0 };
	char CD_Data[8] = { 0 };
	char Sp_Data[8] = { 0 };
	FLASH_Read_Buff(Current_App_LOC, CP_Data, 8);
	FLASH_Read_Buff(Current_App_LOC + 16, CD_Data, 8);
	FLASH_Read_Buff(Current_App_LOC + 32, Sp_Data, 8);
	CP = CP_Data[0];
	CD = CD_Data[0];
	SP = Sp_Data[0];
}

#endif /* INC_TML_C_ */
