/*
 * sms.c
 *
 *  Created on: May 2, 2024
 *      Author: saurabh
 */
#include "sms.h"
#include "main.h"

char sms_resp_buff[250]; // Adjust buffer size if needed
bool messageStatus = false;

void clear_sms_resp_buff() {
	clearBuffer(sms_resp_buff, sizeof(sms_resp_buff));
}

bool SMS_Init() {
	clear_sms_resp_buff();
	// 1. SMS Text Mode
//    if (!EC200_Transmit("ATD9857821331;\r\n", sms_resp_buff, 2000)) { return false; }
	if (!EC200_Transmit("AT+CMGF=1\r\n", sms_resp_buff, 2000)) {
		return false;
	}
	if (strstr(sms_resp_buff, "OK") == NULL) {
		return false;
	}
	// 2. SMS Notifications
	if (!EC200_Transmit("AT+CNMI=2,1,0,0,0\r\n", sms_resp_buff, 2000)) {
		return false;
	}
	if (strstr(sms_resp_buff, "OK") == NULL) {
		return false;
	}

	// 3. RI Pulse for Incoming SMS
	if (!EC200_Transmit("AT+QCFG=\"urc/ri/smsincoming\",\"pulse\",1200,1\r\n",
			sms_resp_buff, 2000)) {
		return false;
	}
	if (strstr(sms_resp_buff, "OK") == NULL) {
		return false;
	}

	// 4. Set Character Set to GSM
	if (!EC200_Transmit("AT+CSCS=\"GSM\"\r\n", sms_resp_buff, 2000)) {
		return false;
	}
	if (strstr(sms_resp_buff, "OK") == NULL) {
		return false;
	}
//    SMS_Send("9467220879","How are you");
	return true;
}
bool SMS_Read(int index, uint8_t *message_buffer) {
	char command[50] = { };
	sprintf(command, "AT+CMGR=%d\r\n", index);
	if (!EC200_Transmit(command, message_buffer, 2000)) {
		return false;
	}
	return SMS_Delete(0); // Replace with actual parsing success/failure logic
}
bool SMS_Send(char *number, char *message) {
	char cmd[100] = { };
	number[10] = '\0';
	sprintf(cmd, "AT+CMGS=\"%s\"\r\n", number);
	if (!EC200_Transmit(cmd, sms_resp_buff, 2000)) {
		return false;
	}
	uint8_t ctrlz[10] = { };
	sprintf(ctrlz, "%s\r\n%c", message, 26);
	if (!EC200_Transmit(ctrlz, sms_resp_buff, 2000)) {
		return false;
	}
	return true;
}
void onMessage() {
	setMessageStatus(true);
}
bool SMS_Delete(int index) {
	clear_sms_resp_buff();
	char command[50] = { };
	sprintf(command, "AT+CMGD=%d,4\r\n", index);
	if (!EC200_Transmit(command, sms_resp_buff, 2000)) {
		return false;
	}
	if (strstr(sms_resp_buff, "OK") == NULL) {
		return false;
	}
	return true;
}
bool getMessageStatus() {
	return messageStatus;
}
void setMessageStatus(bool msgStatus) {
	messageStatus = msgStatus;
}

bool SMS_FSend(char *number, char *message) {
	char cmd[100] = { };
	number[10] = '\0';
	HAL_Delay(1000);
	sprintf(cmd, "AT+CMGS=\"%s\"\r\n", number);
	if (EC200_Transmit(cmd, sms_resp_buff, 2000)) {

	}
	uint8_t ctrlz[10] = { };
	sprintf(ctrlz, "%s\r\n%c", message, 26);
	if (EC200_Transmit(ctrlz, sms_resp_buff, 2000)) {
		return true;
	}
	return false;
}

