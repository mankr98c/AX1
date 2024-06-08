#include"main.h"
#include"EC200Utils.h"
#define BS 1024
extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart6;
extern char *Imei;
extern uint64_t CP;
extern char ComKeys[8][33];
extern bool enable_Idle;
char Decrypt_d[1024] = { 0 };
char resBuff[2048] = { };
extern uint32_t L1, L2, L3;
extern int TOTAL_PACKETS;
int redoCounter = 0;
/*-----------------------------------HTTP-SECTION-------------------------------------------*/

void clearResBuff() {
	clearBuffer(resBuff, sizeof(resBuff));
}
bool EC200_HTTP_POST(int type) {
	// Configure PDP context ID
	clearResBuff();
	Config_Update("IMEI", Imei);
	clearResBuff();
	if (EC200_Transmit("AT+QIACT?\r\n", resBuff, EC200MINRESONSETIME)) {
		printc(100, "AT+QIACT?\r\n RES->%s", resBuff);
	}
	return processMessage(type);

}

/*-----------------------------------HTTP-SECTION-END----------------------------------------*/

bool EC200_Data_Fetch(const char *command) {

	clearResBuff();
	redoCounter = 0;
	readURL: if (!EC200_Transmit(command, resBuff, 5000)) {
		if (redoCounter >= 3) {
			return false;
		}
		redoCounter++;
		goto readURL;
	}
	int KeyID = 0; // Corrected to int
	int result = Response_Check(resBuff);
	if (result == -1) {
		printc(MinDelay, "Wrong data received\n");
		return false; // it needed to be handled all return under FOTA
	}
	char Rchecksum[2] = { 0 }; //
	char En_Data[1024] = { 0 }; // Remove const qualifier
	const char *extracted_Data[6] = { 0 };
	do {
		FOTA_Seprate_String(resBuff, result, 10, extracted_Data, En_Data);
		if (extracted_Data[5] == NULL) {
			printc(MinDelay, "No Checksum\n");
			return false;
		}
		memcpy(Rchecksum, extracted_Data[5], 2);
//		strncpy(En_Data, extracted_Data[4], strlen(extracted_Data[4]));

	} while (extracted_Data[5] == NULL);

	KeyID = atoi(extracted_Data[3]); // Convert string to integer

	char CSUM = Calculate_CRC8(En_Data, strlen(En_Data));
	char Cksum[3] = { 0 }; // Since CSUM is one byte, the buffer should be able to hold two characters plus the null terminator

	sprintf(Cksum, "%0X", CSUM); // Convert CSUM to hexadecimal and store it in Cksum
	clearBuffer(Decrypt_d, sizeof(Decrypt_d));
	if (Rchecksum[0] != Cksum[0] && Rchecksum[1] != Cksum[1]) { // Compare Cksum with Rchecksum
		printc(MinDelay, "Check sum not matched\n");
		return false;
	}
	Decrypt(En_Data, Decrypt_d, KeyID);

	return true;
}
void EC200_File_save(const char *command) {
	uint8_t packet_index = 0;
	uint32_t Addr = 0;
	uint8_t rx_Code_buffer[TOTAL_PACKETS][PACKET_SIZE];
	uint8_t H_Buff[11] = { 0 };

//	if(EC200_Transmit(command, H_Buff, 20000)){
//		printc(1000,"%s",H_Buff);
//		wait_for_response(H_Buff, 20000);
//	}
	enable_Idle = false;
	memset(rx_Code_buffer, 0, sizeof(rx_Code_buffer));
	HAL_UART_Transmit(&huart1, (uint8_t*) command, strlen(command), 2000);

	HAL_UART_Receive(&huart1, H_Buff, sizeof(H_Buff), 500); // Receive data into H_Buff

	for (packet_index = 0; packet_index < TOTAL_PACKETS; packet_index++) {

		HAL_UART_Receive(&huart1, rx_Code_buffer[packet_index], PACKET_SIZE,
				1000);
	}

	writeF_CP((uint64_t)1, (uint64_t)1,(uint64_t)1);
	Addr = L3;
	Erase_sectors(L3);
	for (int i = 0; i < TOTAL_PACKETS; i++) {
		HAL_Delay(100);
		HAL_FLASH_Write_Buff(Addr, &rx_Code_buffer[i], PACKET_SIZE);
		Addr += 1024;
	}

	enable_Idle = true;
	return;
}

void EC200_Key_Update(const char *command) {

	clearResBuff();
	redoCounter = 0;
	readURL: if (!EC200_Transmit(command, resBuff, 5000)) {
		if (redoCounter >= 3) {
			return false;
		}
		redoCounter++;
		goto readURL;
	}
	int KeyID = 0; // Corrected to int
	int result = Response_Check(resBuff);
	if (result == -1) {
		printc(MinDelay, "Wrong data received\n");
		return false; // it needed to be handled all return under FOTA
	}
	char Rchecksum[2] = { 0 }; //
	char En_Data[1024] = { 0 }; // Remove const qualifier
	const char *extracted_Data[6] = { 0 };
	do {
		FOTA_Seprate_String(resBuff, result, 10, extracted_Data, En_Data);
		if (extracted_Data[5] == NULL) {
			printc(MinDelay, "No Checksum\n");
			return false;
		}
		memcpy(Rchecksum, extracted_Data[5], 2);
//		strncpy(En_Data, extracted_Data[4], strlen(extracted_Data[4]));

	} while (extracted_Data[5] == NULL);

	KeyID = atoi(extracted_Data[3]); // Convert string to integer

	char CSUM = Calculate_CRC8(En_Data, strlen(En_Data));
	char Cksum[3] = { 0 }; // Since CSUM is one byte, the buffer should be able to hold two characters plus the null terminator

	sprintf(Cksum, "%0X", CSUM); // Convert CSUM to hexadecimal and store it in Cksum
	clearBuffer(Decrypt_d, sizeof(Decrypt_d));
	if (Rchecksum[0] == Cksum[0] && Rchecksum[1] == Cksum[1]) { // Compare Cksum with Rchecksum
		k_Decrypt(En_Data, Decrypt_d, KeyID);
	} else {
		printc(MinDelay, "CheckSum Don't Matched\n");
//			Decrypt(En_Data, Decrypt_d, KeyID);
	}
//		memset(rx_buffer, 0, sizeof(rx_buffer)); // Clear rx_buffer
	clearBuffer(ComKeys, sizeof(ComKeys));
	extractKeys(Decrypt_d, ComKeys);
	UPDATE_COMKeys();
	// Print Function For Keys
	for (int i = 0; i < 8; i++) {
		printc(MinDelay, "Key %d: %s\n\r", i + 1, ComKeys[i]);
	}

}


bool EC200_Config_Update(const char *command) {

clearResBuff();
redoCounter = 0;
readURL: if (!EC200_Transmit(command, resBuff, 5000)) {
	if (redoCounter >= 3) {
		return false;
	}
	redoCounter++;
	goto readURL;
}
int KeyID = 0; // Corrected to int
int result = Response_Check(resBuff);
if (result == -1) {
	printc(MinDelay, "Wrong data received\n");
	return false; // it needed to be handled all return under FOTA
}
char Rchecksum[2] = { 0 }; //
char En_Data[1024] = { 0 }; // Remove const qualifier
const char *extracted_Data[6] = { 0 };
do {
	FOTA_Seprate_String(resBuff, result, 10, extracted_Data, En_Data);
	if (extracted_Data[5] == NULL) {
		printc(MinDelay, "No Checksum\n");
		return false;
	}
	memcpy(Rchecksum, extracted_Data[5], 2);
	//		strncpy(En_Data, extracted_Data[4], strlen(extracted_Data[4]));

} while (extracted_Data[5] == NULL);

KeyID = atoi(extracted_Data[3]); // Convert string to integer

char CSUM = Calculate_CRC8(En_Data, strlen(En_Data));
char Cksum[3] = { 0 }; // Since CSUM is one byte, the buffer should be able to hold two characters plus the null terminator

sprintf(Cksum, "%0X", CSUM); // Convert CSUM to hexadecimal and store it in Cksum
clearBuffer(Decrypt_d, sizeof(Decrypt_d));
if (Rchecksum[0] != Cksum[0] && Rchecksum[1] != Cksum[1]) { // Compare Cksum with Rchecksum
	printc(MinDelay, "Check sum not matched\n");
	return false;
}
Decrypt(En_Data, Decrypt_d, KeyID);

return true;
}
