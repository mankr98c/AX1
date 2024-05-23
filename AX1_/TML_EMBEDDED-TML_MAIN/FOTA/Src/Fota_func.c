// all FoTA Functionality
/*________________Written by Manky___________________________*/
#include"FOTA.h"

char Imei[16];
uint8_t intuchrRedo = 0;
void setImei() {

}
void getImei() {

}
extern char Decrypt_d[1024];
extern char resBuff[2024];
// Comment after testing
//char StatKeys[3][33] = { "28C282CC6A4C2033303DE699F1ABF11E",
//		"36F4E12F4EFB7730E9749727B34DBC30", "35AFA9AB4CE596418A19AC758597384D" };
// unComment after Testing
char StatKeys[3][33] = { 0 };
char ComKeys[8][33] = { 0 };

bool setStaticKeys(char *key1, char *key2, char *key3) {
	strcpy(StatKeys[0], key1);
	strcpy(StatKeys[1], key2);
	strcpy(StatKeys[2], key3);
}
bool setComKeys(char *key1, char *key2, char *key3, char *key4, char *key5,
		char *key6, char *key7, char *key8) {
	strcpy(ComKeys[0], key1);
	strcpy(ComKeys[1], key1);
	strcpy(ComKeys[2], key3);
	strcpy(ComKeys[3], key4);
	strcpy(ComKeys[4], key5);
	strcpy(ComKeys[5], key6);
	strcpy(ComKeys[6], key7);
	strcpy(ComKeys[7], key8);
}
//   D8713789989BC8D6A275A16394ACC251	17510D819CC56E1A524CE1B4A390DBC0	29F67DBF9252188A559D8DB138F0A9A8

// Now you can access the keys by index (1, 2, 3)
char *key1 = StatKeys[0]; // Index 0 corresponds to the first key
char *key2 = StatKeys[1]; // Index 1 corresponds to the second key
char *key3 = StatKeys[2]; // Index 2 corresponds to the third key

// Now you can access the keys by index (1, 2, 3)
char *ckey1 = ComKeys[0]; // Index 0 corresponds to the first key
char *ckey2 = ComKeys[1]; // Index 1 corresponds to the second key
char *ckey3 = ComKeys[2]; // Index 2 corresponds to the third key
char *ckey4 = ComKeys[3]; // Index 0 corresponds to the first key
char *ckey5 = ComKeys[4]; // Index 1 corresponds to the second key
char *ckey6 = ComKeys[5]; // Index 2 corresponds to the third key
char *ckey7 = ComKeys[6]; // Index 0 corresponds to the first key
char *ckey8 = ComKeys[7]; // Index 1 corresponds to the second key

int TOTAL_PACKETS;
uint8_t key_resBuffer[200] = { };
void reset_keyresBuffer() {
	clearBuffer(key_resBuffer, sizeof(key_resBuffer));
}
// Define callback functions for each message type
void handleKeysRequest() {

	uint8_t str[100] = { 0 };
	char command[250] = { 0 };
	char dataBody[300] = { 0 };
	char str1[300] = { 0 };
	memset(dataBody, 0, sizeof(dataBody));
//	snprintf(dataBody, sizeof(dataBody), "{\"imei\":\"%s\"}", &Imei);
	sprintf(dataBody, "{\"imei\":\"%s\"}", &Imei, sizeof(dataBody));
	printc(100, "String =%s \r\n", dataBody);
	sKey_update(1);
	encrypt(dataBody, str);

	int array_length = strlen(str);

	char hex_string[array_length];

	for (int i = 0; i < array_length; i++) {
		sprintf(hex_string + (i * 2), "%02X", str[i]);
	}

	memset(dataBody, '\0', sizeof(dataBody));
	memcpy(dataBody, hex_string, strlen(hex_string));
	memset(hex_string, 0, sizeof(hex_string));

// Calculate the checksum
	uint8_t checksum = Calculate_CRC8((uint8_t*) dataBody, strlen(dataBody));
	memset(str1, 0, sizeof(str1));

// Construct the string
	snprintf((char*) str1, sizeof(str1), "hexKey=%s%s,%d,%s,%d,%s,%0X%c", key2,
	header, 1, &Imei, 2, dataBody, checksum, Delimiter);

	printc(100, "CommandURL -  %s \n\r", str1);

	memset(dataBody, 0, sizeof(dataBody));
// Create the AT command string with the length of dataBody inserted
	sprintf(command, "AT+QHTTPURL=%d,%d\r\n", strlen(URL), EC_TOUT);
	reset_keyresBuffer();
	if (EC200_Transmit(command, key_resBuffer, EC200MINRESONSETIME)) {
		printc(100, "AT+QHTTPURL -  %s \n\r", key_resBuffer);
	}
	memset(command, 0, 250);
// Send HTTP URL
	reset_keyresBuffer();
	if (EC200_Transmit(URL, key_resBuffer, EC200MINRESONSETIME)) {
		printc(100, "URL -  %s \n\r", key_resBuffer);
	}
	int F_Data = strlen(str1);

// Construct the AT command for HTTP POST
	sprintf(command, "AT+QHTTPPOST=%d\r\n", F_Data);
// Transmit the command string
//	EC200_Data_Fetch(command);
	reset_keyresBuffer();
	if (EC200_Transmit(command, key_resBuffer, 10000)) {
		printc(100, "AT+QHTTPPOST -  %s \n\r", key_resBuffer);
	}
	memset(command, 0, sizeof(command));
// Transmit the dataBody
	if (EC200_Transmit(str1, key_resBuffer, EC200MINRESONSETIME)) {
		printc(100, "AT+QHTTPPOST -  %s \n\r", key_resBuffer);
	}
	memset(str1, 0, sizeof(str1));
	HAL_Delay(3000);
	sprintf(command, "AT+QHTTPREAD= %d \r\n", EC_TOUT);
	EC200_Key_Update(command);
	;
}

void handleKeysUpdate() {
	printc(100, "Received Keys Update\n");
// Add your implementation here
}

void handleFotaDetails() {
	printc(100, "Received Fota Version and Cota Setting Details\n");
// Add your implementation here
}

void handleVersionStatus() {
	printc(100, "Received Version Status\n");
// Add your implementation here
}

void handleCotaData() {
	uint8_t str[100] = { 0 };
	char command[250] = { 0 };
	char dataBody[300] = { 0 };
	char str1[300] = { 0 };
	char CVer[30] = { "Test_1" }; // Fetched FRom Flash
	redoIntouchConnect:
	memset(dataBody, 0, sizeof(dataBody));
	//	snprintf(dataBody, sizeof(dataBody), "{\"imei\":\"%s\"}", &Imei);
	sprintf(dataBody, "{\"imei\":\"%s\",\"configVersion\":\"%s\"}", &Imei, &CVer,
			sizeof(dataBody));
	printc(100, "String =%s \r\n", dataBody);
	cKey_update(0);
	int array_length = encrypt(dataBody, str);

	char hex_string[array_length];

	memset(hex_string,0,sizeof(hex_string));
	for (int i = 0; i < array_length; i++) {
		sprintf(hex_string + (i * 2), "%02X", str[i]);
	}

	memset(dataBody, '\0', sizeof(dataBody));
	memcpy(dataBody, hex_string, strlen(hex_string));
	memset(hex_string, 0, sizeof(hex_string));
	// Calculate the checksum
	uint8_t checksum = Calculate_CRC8((uint8_t*) dataBody, strlen(dataBody));
	memset(str1, 0, sizeof(str1));

	// Construct the string
	snprintf((char*) str1, sizeof(str1), "hexKey=%s%s,%d,%s,%d,%s,%0X%c", ckey1,
	header, 5, &Imei, 1, dataBody, checksum, Delimiter);

	printc(100, "CommandURL -  %s \n\r", str1);

	sprintf(command, "AT+QHTTPURL=%d,%d\r\n", strlen(URL), EC_TOUT);
	clearBuffer(resBuff, sizeof(resBuff));
	if (EC200_Transmit(command, resBuff, 10000)) {
		printc(100, "AT+QHTTPURL -  %s \n\r", key_resBuffer);
	}
	memset(command, 0, 250);

	// Send HTTP URL
	clearBuffer(resBuff, sizeof(resBuff));
	HAL_Delay(100);
	if (EC200_Transmit(URL, resBuff, 10000)) {
		printc(100, "%s \n\r", resBuff);
	}
	int F_Data = strlen(str1);
	// Construct the AT command for HTTP POST
	sprintf(command, "AT+QHTTPPOST=%d\r\n", F_Data);

	// Transmit the command string
	clearBuffer(resBuff, sizeof(resBuff));
	HAL_Delay(100);
	if (EC200_Transmit(command, resBuff, 10000)) {
//		printc(100, "POST - %s \n\r", resBuff);
	}
	memset(command, 0, 250);
	// Transmit the dataBody
	clearBuffer(resBuff, sizeof(resBuff));
	HAL_Delay(500);
	if (EC200_Transmit(str1, resBuff, 10000)) {
		printc(100, "%s \n\r", resBuff);
	}
	memset(str1, 0, sizeof(str1));
	HAL_Delay(500);
	sprintf(command, "AT+QHTTPREAD= %d\r\n", EC_TOUT);
	EC200_Config_Update(command);

}

void handleErrorResponse() {
	printc(100, "Received Error Response\n");
// Add your implementation here
}

bool handleFotaData() {
//	char version[] ={ "1.0.01" };
	uint8_t str[100] = { 0 };
	char dataBody[300] = { 0 };
	char str1[300] = { 0 };
	char command[250] = { 0 };

	redoIntouchConnect: snprintf(dataBody, sizeof(dataBody),
			"{\"version\":\"%s\"}", getUpdateVersion());
//	snprintf(dataBody, sizeof(dataBody), "{\"version\":\"%s\"}",
//			version);
	printc(100, "String = %s \r\n", dataBody);
//	ReadF_Loc(2, 1);
	cKey_update(0);
	int array_length = encrypt(dataBody, str);

	char hex_string[array_length];

	for (int i = 0; i < array_length; i++) {
		sprintf(hex_string + (i * 2), "%02X", str[i]);
	}

	memset(dataBody, '\0', sizeof(dataBody));
	memcpy(dataBody, hex_string, strlen(hex_string));
	memset(hex_string, 0, sizeof(hex_string));

// Calculate the checksum

	uint8_t checksum = Calculate_CRC8((uint8_t*) dataBody, strlen(dataBody));
	memset(str1, 0, sizeof(str1));

	snprintf(str1, sizeof(str1), "hexKey=%s%s,%d,%s,%d,%s,%0X%c",
			ComKeys[0], header, 6, &Imei, 1, dataBody, checksum, Delimiter);

	printc(100, "Command -  %s \n\r", str1);

	sprintf(command, "AT+QHTTPURL=%d,%d\r\n", strlen(URL), EC_TOUT);
	clearBuffer(resBuff, sizeof(resBuff));
	if (EC200_Transmit(command, resBuff, 10000)) {
		printc(100, "AT+QHTTPURL -  %s \n\r", key_resBuffer);
	}
	memset(command, 0, 250);

	// Send HTTP URL
	clearBuffer(resBuff, sizeof(resBuff));
	HAL_Delay(100);
	if (EC200_Transmit(URL, resBuff, 10000)) {
		printc(100, "%s \n\r", resBuff);
	}
	int F_Data = strlen(str1);
	// Construct the AT command for HTTP POST
	sprintf(command, "AT+QHTTPPOST=%d\r\n", F_Data);

	// Transmit the command string
	clearBuffer(resBuff, sizeof(resBuff));
	HAL_Delay(100);
	if (EC200_Transmit(command, resBuff, 10000)) {
		printc(100, "POST - %s \n\r", resBuff);
	}
	memset(command, 0, 250);
	// Transmit the dataBody
	clearBuffer(resBuff, sizeof(resBuff));
	HAL_Delay(500);
	if (EC200_Transmit(str1, resBuff, 10000)) {
		printc(100, "%s \n\r", resBuff);
	}

	memset(str1, 0, sizeof(str1));
	HAL_Delay(500);
	sprintf(command, "AT+QHTTPREAD= %d\r\n", EC_TOUT);

	if (!EC200_Data_Fetch(command)) {
		if (intuchrRedo >= 3)
			return false;
		intuchrRedo++;
		goto redoIntouchConnect;
	}
	F_Data = 0;
	F_Data = strlen(Decrypt_d);
	char DURL[F_Data];
	memset(DURL, 0, sizeof(DURL));
	uint8_t paddingVal = Decrypt_d[F_Data - 1];
	printc(1000, "Download-URL = %s \r\n", Decrypt_d);
	memcpy(DURL, Decrypt_d, F_Data - paddingVal);
	print(1000, DURL, strlen(DURL));
	memset(command, 0, 250);

	sprintf(command, "AT+QHTTPURL=%d,%d\r\n", strlen(DURL), 30);
	clearBuffer(resBuff, sizeof(resBuff));
	HAL_Delay(100);
	if (EC200_Transmit(command, resBuff, 10000)) {
		printc(100, "%s \n\r", resBuff);
	}
	clearBuffer(resBuff, sizeof(resBuff));
	if (EC200_Transmit(DURL, resBuff, 20000)) {
		printc(100, "URL%s \n\r", resBuff);
	}

	memset(command, 0, 250);
	// Send HTTP GET request
	clearBuffer(resBuff, sizeof(resBuff));
	if (EC200_Transmit("AT+QHTTPGET\r\n", resBuff, 20000)) {
		printc(100, "%s \n\r", resBuff);
		clearBuffer(resBuff, sizeof(resBuff));
		switchIdle();
		wait_for_response(resBuff, 20000);
	}
	// Read the Response
	EC200_File_save("AT+QHTTPREAD=80\r\n");

	return true;
}

// Main function to process incoming messages
bool processMessage(int messageType) {
	switch (messageType) {
	case 1:
		handleKeysRequest();  // Finished
		break;
	case 2:
		handleKeysUpdate();
		break;
	case 3:
		handleFotaDetails();
		break;
	case 4:
		handleVersionStatus();
		break;
	case 5:
		handleCotaData();
		break;
	case 6:
		return handleFotaData();           // in-progress
		break;
	case 9:
		handleErrorResponse();
		break;
	default:
		printc(100, "Invalid Message Type\n");
		break;
	}
	return false;
}

void Config_Update(char *cmnds, char *Data) {
	if (strcmp(cmnds, "IMEI") == 0) {
		char RspBuff[200] = { 0 };
		char ExBuff[20] = { 0 };
		char *Result;
		int size, retryCount;
		int EL = 0, SL = 0;

//		HAL_UART_Transmit(&huart1, (uint8_t*) "AT+GSN\r\n",
//				strlen("AT+GSN\r\n"), 2000);
//		HAL_UART_Receive(&huart1, (uint8_t*) RspBuff, sizeof(RspBuff), 5000);
		if (EC200_Transmit("AT+GSN\r\n", RspBuff, EC200MINRESONSETIME)) {
			printc(100, "AT+GSN\r\n res -> %s", RspBuff);
			Result = Response_Check((char*) RspBuff);
			EL = Result - 5;
			SL = EL - 14;
//		if (Result == NULL && retryCount <= 3) {
//			printc(100,"Substring 'OK' not found, retrying (attempt %d).\n",
//					retryCount);
//			goto retry;
//			retryCount++;
//		}

			Extract_Data_from_Buffer(SL, EL, RspBuff, ExBuff, &size);

//		Seprate_String(RspBuff, 5, ExBuff);
			// need to add FlasH Write to Config Sector location
			memcpy(Imei, ExBuff, strlen(ExBuff));
			memset(RspBuff, 0, sizeof(RspBuff));
			HAL_Delay(100);
		}
	}
}

void Extract_Data_from_Buffer(int Start_loc, int End_loc, char Buff[],
		char *extractedData, int *size) {
	*size = End_loc - Start_loc + 1;
	strncpy(extractedData, &Buff[Start_loc], *size);
	extractedData[*size] = '\0'; // Null-terminate the string
//	printc(100,"Extracted data: %s\n", extractedData);
}

// Function to Check the Response Of the At commands
int Response_Check(char Buff[]) {
	char *result_OK = strstr(Buff, "OK");
	char *result_error = strstr(Buff, "\"errorResponse\":\"succesful\"");
	char *Result_MMI = strstr(Buff, "$MMI");

	if (Result_MMI != NULL) {

		printc(100, "Substring '$MMI' found at index %d\n", Result_MMI - Buff);
		return Result_MMI - Buff; // Return index of 'errorResponse':'succesful'
	} else if (result_OK != NULL) {
		printc(100, "Substring 'OK' found at index %d\n", result_OK - Buff);
		return result_OK - Buff; // Return index of 'OK'
	} else if (result_error != NULL) {
		printc(100, "Substring 'errorResponse':'succesful' found at index %d\n",
				result_error - Buff);
		return -1; // Return index of 'errorResponse':'succesful'
	} else {
		printc(100, "Neither substring found\n");
		return -1; // Indicate that neither substring was found
	}
	return -1;
}
int Hex_Check(char Buff[], int hexValue) {
	char target_char[3]; // To hold the hexadecimal representation of hexValue
	sprintf(target_char, "%02X", hexValue); // Convert hexValue to hexadecimal string

	char *result_OK = strstr(Buff, target_char); // Search for the hexadecimal string

	if (result_OK != NULL) {
		printc("Hexadecimal value '%s' found at index %ld\n", target_char,
				result_OK - Buff);
		return result_OK - Buff; // Return index of the found hexadecimal value
	} else {
		printc(100, "Hexadecimal value '%s' not found\n", target_char);
		return -1; // Indicate that the hexadecimal value was not found
	}
}
// extract keys

// Function to extract keys
void extractKeys(const char *inputString, char (*CoKeys)[33]) {
	const char *start = strstr(inputString, "{\"comMap\":{"); // Find the start of the comMap section
	if (start == NULL) {
		printc(100, "comMap section not found in the input string.\n");
		return;
	}

	start += strlen("{\"comMap\":{"); // Move start pointer to the beginning of the comMap data

// Loop to extract values
	for (int i = 0; i < 8; i++) {
		// Find the start of the value
		const char *valueStart = strstr(start, ":\""); // Find the start of the value
		if (valueStart == NULL) {
			printc(100, "Value start not found.\n");
			return;
		}
		valueStart += 2; // Move past the ":"

		// Find the end of the value
		const char *valueEnd = strstr(valueStart, "\"");
		if (valueEnd == NULL) {
			printc(100, "Value end not found.\n");
			return;
		}

		// Copy the value to the ComKeys buffer
		strncpy(CoKeys[i], valueStart, valueEnd - valueStart);
		CoKeys[i][valueEnd - valueStart] = '\0'; // Null-terminate the value

		// Move start pointer to the next value
		start = valueEnd + 1;
	}
}

void Seprate_String(char Buff[], int Start_Location, int index, char *data[]) {
	char *line;
	int len = strlen(Buff) - Start_Location;
	char tmpBuffer[len + 1]; // Allocate enough space for the string

	memcpy(tmpBuffer, &Buff[Start_Location], len); // Copy Buff to tmpBuffer
	tmpBuffer[len] = '\0'; // Null-terminate the string

	int currentLine = 0;

	line = strtok(tmpBuffer, ",");
	while (line != NULL && currentLine <= index) {
		data[currentLine++] = line;

		if (currentLine == index) {
			printc(100, "Line %d: %s\n", currentLine, line);
			break; // Stop parsing once the desired line is found
		}
		printc(100, "Line %d: %s\n", currentLine, line);
		line = strtok(NULL, ",");
	}

	if (line == NULL) {
		printc(100, "Line %d does not exist.\n", index);
	}
}

void FOTA_Seprate_String(char Buff[], int Start_Location, int index,
		char *data[], char Encpt_Data[]) {
	char *line;
	int len = strlen(Buff) - Start_Location;
	char tmpBuffer[len + 1]; // Allocate enough space for the string

	memcpy(tmpBuffer, &Buff[Start_Location], len); // Copy Buff to tmpBuffer
	tmpBuffer[len] = '\0'; // Null-terminate the string

	int currentLine = 0;

	line = strtok(tmpBuffer, ",");
	while (line != NULL && currentLine <= index) {
		data[currentLine++] = line;
		if (currentLine == 5)
			memcpy(Encpt_Data, line, strlen(line));
		if (currentLine == index) {
			printc(100, "Line %d: %s\n", currentLine, line);
			break; // Stop parsing once the desired line is found
		}
		printc(100, "Line %d: %s\n", currentLine, line);
		line = strtok(NULL, ",");
	}

	if (line == NULL) {
		printc(100, "Line %d does not exist.\n", index);
	}
}

uint8_t Calculate_CRC8(uint8_t *data, uint32_t size) {
	uint8_t arr[size];
	memset(arr, 0, sizeof(arr));
	memcpy(arr, data, size);
	return HAL_CRC_Calculate(&hcrc, arr, size);
}

