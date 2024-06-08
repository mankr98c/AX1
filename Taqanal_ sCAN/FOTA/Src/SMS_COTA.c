/*
 * SMS_COTA.c
 *
 *  Created on: May 6, 2024
 *      Author: Manky
 */

#include "main.h"
#include "SMS_COTA.h"
// Functions for handling specific GET commands

extern uint32_t L1, L2, L3;
extern int TOTAL_PACKETS;
extern char ComKeys[8][33];
extern char updateVersion[10];
extern int FLAG_RESET_On_Sleep;
RConfiguration Rconfig;
bool updateStatus = false;
char RB[100] = { 0 };
extern char *PHnum;
bool SRESET_FLAG = false;
bool getUpdateStatus() {
	return updateStatus;
}
void setUpdateStaus(bool upStatus) {
	updateStatus = upStatus;
}

void clear_RConfiguration() {
	clearBuffer(&Rconfig, sizeof(Rconfig));
}

bool SMS_COTA(char *SMSBuff, char *opBuffer) {
	uint8_t splitBuffer[5][100] = { 0 };
	splitString((uint8_t*) SMSBuff, " ", splitBuffer);
	if (splitBuffer[0] == NULL) {
		strcpy(opBuffer, "Didn't Get Proper command");
		return false;
	}
	char *command = (char*) strdup(splitBuffer[0]);
	if (splitBuffer[1] == NULL) {
		strcpy(opBuffer, "Didn't Got Any parameter1");
		return false;
	}
	char *parameter = strdup(splitBuffer[1]);
	char *parameter2 = NULL;
	if (splitBuffer[2] != NULL) {
		parameter2 = strdup(splitBuffer[2]);
	}
	clearBuffer(splitBuffer, sizeof(splitBuffer));
	splitString(parameter, ":", splitBuffer);
	parameter = strdup(splitBuffer[0]);
	char *value = strdup(splitBuffer[1]);
	if (strcmp(command, "GET") == 0) {
		if (parameter != NULL) {
			// Handle GET command with parameter
			if (strcmp(parameter, "MIP") == 0) {
				SGET_MIP(opBuffer);
			} else if (strcmp(parameter, "PORT1") == 0) {
				SGET_PORT1(opBuffer);
			} else if (strcmp(parameter, "MUSER") == 0) {
				SGET_MUSER(opBuffer);
			} else if (strcmp(parameter, "MPASS") == 0) {
				SGET_MPASS(opBuffer);
			} else if (strcmp(parameter, "CERT") == 0) {
				SGET_CERT(opBuffer);
			} else if (strcmp(parameter, "KEY") == 0) {
				SGET_KEY(opBuffer);
			} else if (strcmp(parameter, "CA") == 0) {
				SGET_CA(opBuffer);
			} else if (strcmp(parameter, "IP2") == 0) {
				SGET_IP2(opBuffer);
			} else if (strcmp(parameter, "PORT2") == 0) {
				SGET_PORT2(opBuffer);
			} else if (strcmp(parameter, "SKEY1") == 0) {
				SGET_SKEY1(opBuffer);
			} else if (strcmp(parameter, "SKEY2") == 0) {
				SGET_SKEY2(opBuffer);
			} else if (strcmp(parameter, "SKEY3") == 0) {
				SGET_SKEY3(opBuffer);
			} else if (strcmp(parameter, "APN") == 0) {
				SGET_APN(opBuffer);
			} else if (strcmp(parameter, "ION") == 0) {
				SGET_ION(opBuffer);
			} else if (strcmp(parameter, "NSAM") == 0) {
				SGET_NSAM(opBuffer);
			} else if (strcmp(parameter, "HA") == 0) {
				SGET_HA(opBuffer);
			} else if (strcmp(parameter, "HB") == 0) {
				SGET_HB(opBuffer);
			} else if (strcmp(parameter, "HC") == 0) {
				SGET_HC(opBuffer);
			} else if (strcmp(parameter, "ACCOFFSET") == 0) {
				SGET_ACCOFFSET(opBuffer);
			} else if (strcmp(parameter, "USER") == 0) {
				SGET_USER(opBuffer);
			} else if (strcmp(parameter, "PASSWORD") == 0) {
				SGET_PASSWORD(opBuffer);
			} else if (strcmp(parameter, "VIN") == 0) {
				SGET_VIN(opBuffer);
			} else {
				// Handle unknown parameter
				printc(MinDelay, "Unknown parameter: %s\n", parameter);
				return false;
			}
			char *tmpcheck = strdup(opBuffer);
			sprintf(opBuffer, "%s:%s", parameter, tmpcheck);
		} else {
			// Handle GET command without parameter
			printc(MinDelay, "No parameter specified for GET command.\n");
			return false;
		}
	} else if (strcmp(command, "SET") == 0) {

		if (parameter != NULL) {
			if (value != NULL) {
				// Handle SET command with parameter and value
				if (strcmp(parameter, "MIP") == 0) {
					SET_MIP(value);
					setTcpServerIP(value);
				} else if (strcmp(parameter, "PORT1") == 0) {
					SET_PORT1(value);
				} else if (strcmp(parameter, "MUSER") == 0) {
					SET_MUSER(value);
				} else if (strcmp(parameter, "MPASS") == 0) {
					SET_MPASS(value);
				} else if (strcmp(parameter, "CERT") == 0) {
					SET_CERT(value);
				} else if (strcmp(parameter, "KEY") == 0) {
					SET_KEY(value);
				} else if (strcmp(parameter, "CA") == 0) {
					SET_CA(value);
				} else if (strcmp(parameter, "IP2") == 0) {
					SET_IP2(value);
				} else if (strcmp(parameter, "PORT2") == 0) {
					SET_PORT2(value);
				} else if (strcmp(parameter, "SKEY1") == 0) {
					SET_SKEY1(value);
				} else if (strcmp(parameter, "SKEY2") == 0) {
					SET_SKEY2(value);
				} else if (strcmp(parameter, "SKEY3") == 0) {
					SET_SKEY3(value);
				} else if (strcmp(parameter, "APN") == 0) {
					SET_APN(value);
				} else if (strcmp(parameter, "ION") == 0) {
					int intValue = atoi(value);
					if (intValue > 3) {
						SET_ION(value);
						setSampling(intValue);
					} else
						return false;
				} else if (strcmp(parameter, "NSAM") == 0) {
					SET_NSAM(value);
				} else if (strcmp(parameter, "RESET") == 0) {
					SRESET_FLAG = true;
				} else if (strcmp(parameter, "HA") == 0) {
					SET_HA(value);
				} else if (strcmp(parameter, "HB") == 0) {
					SET_HB(value);
				} else if (strcmp(parameter, "HC") == 0) {
					SET_HC(value);
				} else if (strcmp(parameter, "ACCOFFSET") == 0) {
					SET_ACCOFFSET(value);
				} else if (strcmp(parameter, "USER") == 0) {
					SET_USER(value);
				} else if (strcmp(parameter, "PASSWORD") == 0) {
					SET_PASSWORD(value);
				} else if (strcmp(parameter, "VIN") == 0) {
					SET_VIN(value);
				} else if (strcmp(parameter, "UPFW") == 0) {
					clearBuffer(splitBuffer, sizeof(splitBuffer));
					splitString(parameter2, ":", splitBuffer);
					if (strcmp(splitBuffer[0], "TML") == 0) {
						//						checking password
						if (!strcmp(splitBuffer[1], "1234")) {
							SET_FOTA_UPDATE(value);
							setUpdateStaus(true);
						} else {
							strcpy(opBuffer, "Password Didn't Match\n");
							return false;
						}
					} else {
						strcpy(opBuffer, "UPFW Format Not Matched\n");
						return false;
					}
				} else {
					// Handle unknown parameter
//					sprintf(opBuffer, "%s Unknown parameter:",parameter);
					printc(MinDelay, "Unknown parameter: %s\n", parameter);
					return false;
				}
				if (strcmp(parameter, "UPFW") == 0) {
					sprintf(opBuffer, "%s Updating", parameter);
				} else if (SRESET_FLAG == true) {
					sprintf(opBuffer, "%s OK", parameter);
				} else
					sprintf(opBuffer, "%s Updated", parameter);
			} else {
				// Handle SET command without value
				printc(MinDelay, "Value not specified for parameter: %s\n",
						parameter);
				return false;
			}
		} else {
			// Handle SET command without parameter and value
			printc(MinDelay,
					"parameter and value not specified for SET or GET command.\n");
			return false;
		}
	} else {
		// Handle unknown commands
		printc(MinDelay, "Unknown command: %s\n", command);
		return false;
	}
	return true;
}

void SGET_MIP(char *opBuffer) {
	strcpy(opBuffer, Rconfig.MIP);
}

void SGET_PORT1(char *opBuffer) {
	if (Rconfig.MPORT1 != NULL)
		strcpy(opBuffer, Rconfig.MPORT1);
}

void SGET_MUSER(char *opBuffer) {
	strcpy(opBuffer, Rconfig.MUSER);
}

void SGET_MPASS(char *opBuffer) {
	strcpy(opBuffer, Rconfig.MPASS);
}

void SGET_CERT(char *opBuffer) {
	strcpy(opBuffer, Rconfig.CERT);
}

void SGET_KEY(char *opBuffer) {
	strcpy(opBuffer, Rconfig.KEY);
}

void SGET_CA(char *opBuffer) {
	strcpy(opBuffer, Rconfig.CA);
}

void SGET_IP2(char *opBuffer) {
	strcpy(opBuffer, Rconfig.IP2);
}

void SGET_LAT(double *lat){
	sscanf(Rconfig.LAT, "%lf", lat);
}
void SGET_LON(double *lon){
	sscanf(Rconfig.LON, "%lf", lon);
}
void SGET_PORT2(char *opBuffer) {
	strcpy(opBuffer, Rconfig.IPPORT2);
}

void SGET_SKEY1(char *opBuffer) {
	strcpy(opBuffer, Rconfig.SKEY1);
}

void SGET_SKEY2(char *opBuffer) {
	strcpy(opBuffer, Rconfig.SKEY2);
}

void SGET_SKEY3(char *opBuffer) {
	strcpy(opBuffer, Rconfig.SKEY3);
}

void SGET_APN(char *opBuffer) {
	strcpy(opBuffer, Rconfig.DAPN);
}

void SGET_ION(char *opBuffer) {
	strcpy(opBuffer, Rconfig.ION);
}

void SGET_NSAM(char *opBuffer) {
	strcpy(opBuffer, Rconfig.NSAM);
}

void SGET_HA(char *opBuffer) {
	strcpy(opBuffer, Rconfig.HA);
}

void SGET_HB(char *opBuffer) {
	strcpy(opBuffer, Rconfig.HB);
}

void SGET_HC(char *opBuffer) {
	strcpy(opBuffer, Rconfig.HC);
}

void SGET_ACCOFFSET(char *opBuffer) {
	strcpy(opBuffer, Rconfig.ACCOFFSET);
}

void SGET_USER(char *opBuffer) {
	strcpy(opBuffer, Rconfig.USER);
}

void SGET_PASSWORD(char *opBuffer) {
	strcpy(opBuffer, Rconfig.PSWD);
}

void SGET_VIN(char *opBuffer) {
	strcpy(opBuffer, Rconfig.VIN);
}

void SGET_WAkeupTime(char *opBuffer) {
	if (Rconfig.Rtime[0] != 0xff)
		strcpy(opBuffer, Rconfig.Rtime);
}

// Implement SET command functions similarly
bool SET_MIP(char *ip_address) {
	if (Rconfig.MIP != NULL)
		clearBuffer(Rconfig.MIP, strlen(Rconfig.MIP));
	Rconfig.MIP = ip_address;
}

bool SET_PORT1(char *port) {
	if (Rconfig.MPORT1 != NULL)
		clearBuffer(Rconfig.MPORT1, strlen(Rconfig.MPORT1));
	Rconfig.MPORT1 = port;
}

bool SET_MUSER(char *username) {
	if (Rconfig.MUSER != NULL)
		clearBuffer(Rconfig.MPORT1, strlen(Rconfig.MPORT1));
	Rconfig.MUSER = username;

}

bool SET_MPASS(char *password) {
	if (Rconfig.MPASS != NULL)
		clearBuffer(Rconfig.MPASS, strlen(Rconfig.MPASS));
	Rconfig.MPASS = password;

}

bool SET_CERT(char *filename) {
	if (Rconfig.CERT != NULL)
		clearBuffer(Rconfig.CERT, strlen(Rconfig.CERT));
	Rconfig.CERT = filename;

}

bool SET_KEY(char *filename) {
	if (Rconfig.KEY != NULL)
		clearBuffer(Rconfig.KEY, strlen(Rconfig.KEY));
	Rconfig.KEY = filename;

}

bool SET_CA(char *filename) {
	if (Rconfig.CA != NULL)
		clearBuffer(Rconfig.CA, strlen(Rconfig.CA));
	Rconfig.CA = filename;

}

bool SET_IP2(char *address) {
	if (Rconfig.IP2 != NULL)
		clearBuffer(Rconfig.IP2, strlen(Rconfig.IP2));
	Rconfig.IP2 = address;

}

bool SET_PORT2(char *value) {
	if (Rconfig.IPPORT2 != NULL)
		clearBuffer(Rconfig.IPPORT2, strlen(Rconfig.IPPORT2));
	Rconfig.IPPORT2 = value;

}

bool SET_SKEY1(char *keyValue) {
	if (Rconfig.SKEY1 != NULL)
		clearBuffer(Rconfig.SKEY1, strlen(Rconfig.SKEY1));
	Rconfig.SKEY1 = keyValue;

}

bool SET_SKEY2(char *keyValue) {
	if (Rconfig.SKEY2 != NULL)
		clearBuffer(Rconfig.SKEY2, strlen(Rconfig.SKEY2));
	Rconfig.SKEY2 = keyValue;

}

bool SET_SKEY3(char *keyValue) {
	if (Rconfig.SKEY3 != NULL)
		clearBuffer(Rconfig.SKEY3, strlen(Rconfig.SKEY3));
	Rconfig.SKEY3 = keyValue;

}

bool SET_APN(char *number) {
	if (Rconfig.DAPN != NULL)
		clearBuffer(Rconfig.DAPN, strlen(Rconfig.DAPN));
	Rconfig.DAPN = number;

}

bool SET_ION(char *number) {
	if (Rconfig.ION != NULL)
		clearBuffer(Rconfig.ION, strlen(Rconfig.ION));
	Rconfig.ION = number;
}

bool SET_NSAM(char *number) {
	if (Rconfig.NSAM != NULL)
		clearBuffer(Rconfig.NSAM, strlen(Rconfig.NSAM));
	Rconfig.NSAM = number;

}

bool SET_HA(char *number) {
	if (Rconfig.HA != NULL)
		clearBuffer(Rconfig.HA, strlen(Rconfig.HA));
	Rconfig.HA = number;

}

bool SET_HB(char *number) {
	if (Rconfig.HB != NULL)
		clearBuffer(Rconfig.HB, strlen(Rconfig.HB));
	Rconfig.HB = number;

}

bool SET_HC(char *number) {
	if (Rconfig.HC != NULL)
		clearBuffer(Rconfig.HC, strlen(Rconfig.HC));
	Rconfig.HC = number;

}

bool SET_ACCOFFSET(char *number) {
	if (Rconfig.ACCOFFSET != NULL)
		clearBuffer(Rconfig.ACCOFFSET, strlen(Rconfig.ACCOFFSET));
	Rconfig.ACCOFFSET = number;

}

bool SET_USER(char *idVal) {
	if (Rconfig.USER != NULL)
		clearBuffer(Rconfig.USER, strlen(Rconfig.USER));
	Rconfig.USER = idVal;

}

bool SET_PASSWORD(char *passwordVal) {
	if (Rconfig.PSWD != NULL)
		clearBuffer(Rconfig.PSWD, strlen(Rconfig.PSWD));
	Rconfig.PSWD = passwordVal;

}

bool SET_VIN(char *number) {
	if (Rconfig.VIN != NULL)
		clearBuffer(Rconfig.VIN, strlen(Rconfig.VIN));
	Rconfig.VIN = number;

}
bool SET_LAT_LON(char *lat,char *lon) {
	if (Rconfig.LAT != NULL)
		clearBuffer(Rconfig.LAT, strlen(Rconfig.LAT));
	Rconfig.LAT = lat;
	if (Rconfig.LON != NULL)
		clearBuffer(Rconfig.LON, strlen(Rconfig.LON));
	Rconfig.LON = lon;
}
bool SET_PHNUM(char *PHnum1) {
	if (Rconfig.PHNUM != NULL)
		clearBuffer(Rconfig.PHNUM, strlen(Rconfig.PHNUM));
	Rconfig.PHNUM = PHnum1;
}

bool SET_RTCTimer(char *RTime1) {
	if (Rconfig.Rtime != NULL)
		clearBuffer(Rconfig.Rtime, strlen(Rconfig.Rtime));
	Rconfig.Rtime = RTime1;
}

void SET_RESET() {
	FeedAllConfig();
	HAL_NVIC_SystemReset();
}
void UPDATE_COMKeys() {
	if (Rconfig.CKEY1 != NULL)
		clearBuffer(Rconfig.CKEY1, strlen(Rconfig.CKEY1));
	if (Rconfig.CKEY2 != NULL)
		clearBuffer(Rconfig.CKEY2, strlen(Rconfig.CKEY2));

	if (Rconfig.CKEY3 != NULL)
		clearBuffer(Rconfig.CKEY3, strlen(Rconfig.CKEY3));

	if (Rconfig.CKEY4 != NULL)
		clearBuffer(Rconfig.CKEY4, strlen(Rconfig.CKEY4));

	if (Rconfig.CKEY5 != NULL)
		clearBuffer(Rconfig.CKEY5, strlen(Rconfig.CKEY5));

	if (Rconfig.CKEY6 != NULL)
		clearBuffer(Rconfig.CKEY6, strlen(Rconfig.CKEY6));

	if (Rconfig.CKEY7 != NULL)
		clearBuffer(Rconfig.CKEY7, strlen(Rconfig.CKEY7));
	if (Rconfig.CKEY8 != NULL)
		clearBuffer(Rconfig.CKEY8, strlen(Rconfig.CKEY8));

	// Ensure Rconfig.CKEY1 to Rconfig.CKEY8 are of type char* and ComKeys is an array of pointers to char.
	Rconfig.CKEY1 = strdup(ComKeys[0]);
	Rconfig.CKEY2 = strdup(ComKeys[1]);
	Rconfig.CKEY3 = strdup(ComKeys[2]);
	Rconfig.CKEY4 = strdup(ComKeys[3]);
	Rconfig.CKEY5 = strdup(ComKeys[4]);
	Rconfig.CKEY6 = strdup(ComKeys[5]);
	Rconfig.CKEY7 = strdup(ComKeys[6]);
	Rconfig.CKEY8 = strdup(ComKeys[7]);
}

bool SET_FOTA_SIZE(char *SZE) {
	int size = atoi(SZE);
	TOTAL_PACKETS = ((size * PACKET_SIZE) / PACKET_SIZE);
	if (size < 128) {
		L1 = L1_v1;
		L2 = L2_v2;
		L3 = L3_v2;

	} else if (size > 128 && size < 256) {
		L1 = L1_v1;
		L2 = L2_v2;
		L3 = L3_v2;

	} else {
		printc(MinDelay, "File size is large");
		return false; // Indicate failure
	}
	return true; // Indicate failure
}

bool SET_FOTA_UPDATE(char *Version) {
	if (Rconfig.FOTAVER != NULL)
		clearBuffer(Rconfig.FOTAVER, strlen(Rconfig.FOTAVER));
	Rconfig.FOTAVER = Version;
	strcpy(updateVersion, Rconfig.FOTAVER);

}

void Write_CFGFlash(uint32_t WriteLoc, uint64_t *DATA) {
	uint32_t KAddr = WriteLoc;
	HAL_FLASH_Unlock();
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_QUADWORD, KAddr, DATA); // Corrected to pass address of WDT
	HAL_FLASH_Lock();
}

void getAllConfig() {
	clear_RConfiguration();
	char tmpConfig[200] = { };
	GET_MIP(tmpConfig);
	GET_PORT1(tmpConfig);
	GET_MUSER(tmpConfig);
	GET_MPASS(tmpConfig);
	GET_CERT(tmpConfig);
	GET_KEY(tmpConfig);
	GET_CA(tmpConfig);
	GET_IP2(tmpConfig);
	GET_PORT2(tmpConfig);
	GET_SKEY1(tmpConfig);
	GET_SKEY2(tmpConfig);
	GET_SKEY3(tmpConfig);
	GET_APN(tmpConfig);
	GET_ION(tmpConfig);
	GET_NSAM(tmpConfig);
	GET_HA(tmpConfig);
	GET_HB(tmpConfig);
	GET_HC(tmpConfig);
	GET_ACCOFFSET(tmpConfig);
	GET_USER(tmpConfig);
	GET_PASSWORD(tmpConfig);
	GET_VIN(tmpConfig);
	GET_Ckey();
	GET_LAT();
	GET_LON();
	GET_PHNUM();
	GET_RTCTimer();
	setStaticKeys(Rconfig.SKEY1, Rconfig.SKEY2, Rconfig.SKEY3);
	setComKeys(Rconfig.CKEY1, Rconfig.CKEY2, Rconfig.CKEY3, Rconfig.CKEY4,
			Rconfig.CKEY5, Rconfig.CKEY6, Rconfig.CKEY7, Rconfig.CKEY8);
//	GET_ALOC(tmpConfig);
}

void FeedAllConfig() {

	HAL_FLASH_ERASE(SMS_COTA_SEC);
	// Write each field of the struct to its corresponding location in flash memory
	Write_CFGFlash(MQTT_IP, (uint64_t*) Rconfig.MIP);
	Write_CFGFlash(MQTT_PORT, (uint64_t*) Rconfig.MPORT1);
	Write_CFGFlash(MQTT_USER, (uint64_t*) Rconfig.MUSER);
	Write_CFGFlash(MQTT_PSW, (uint64_t*) Rconfig.MPASS);
	Write_CFGFlash(MQTT_CERTIFICATE, (uint64_t*) Rconfig.CERT);
	Write_CFGFlash(MQTT_KEY, (uint64_t*) Rconfig.KEY);
	Write_CFGFlash(MQTT_CERTIFICATE_AUTHORITY, (uint64_t*) Rconfig.CA);
	Write_CFGFlash(FOTA_IP2, (uint64_t*) Rconfig.IP2);
	Write_CFGFlash(FOTA_PORT2, (uint64_t*) Rconfig.IPPORT2);
	HAL_FLASH_Write_Buff(FOTA_Static_Key1, (uint64_t*) Rconfig.SKEY1,
			strlen(Rconfig.SKEY1));
	HAL_FLASH_Write_Buff(FOTA_Static_Key2, (uint64_t*) Rconfig.SKEY2,
			strlen(Rconfig.SKEY2));
	HAL_FLASH_Write_Buff(FOTA_Static_Key3, (uint64_t*) Rconfig.SKEY3,
			strlen(Rconfig.SKEY3));
	Write_CFGFlash(APN, (uint64_t*) Rconfig.DAPN);
	Write_CFGFlash(ION_Sampling, (uint64_t*) Rconfig.ION);
	Write_CFGFlash(NO_OF_Sampling, (uint64_t*) Rconfig.NSAM);
	Write_CFGFlash(HARSH_ACC_TH, (uint64_t*) Rconfig.HA);
	Write_CFGFlash(HARSH_DEACC_TH, (uint64_t*) Rconfig.HB);
	Write_CFGFlash(HARSH_CORNERING_TH, (uint64_t*) Rconfig.HC);
	Write_CFGFlash(SENSOR_OFFSET_CAL, (uint64_t*) Rconfig.ACCOFFSET);
	Write_CFGFlash(USERNAME, (uint64_t*) Rconfig.USER);
	Write_CFGFlash(PASSWORD, (uint64_t*) Rconfig.PSWD);
	Write_CFGFlash(VECHILE_ID_NUM, (uint64_t*) Rconfig.VIN);
	Write_CFGFlash(LAT_ADDR, (uint64_t*) Rconfig.LAT);
	Write_CFGFlash(LON_ADDR, (uint64_t*) Rconfig.LON);
	Write_CFGFlash(SMS_NUM_Addr, (uint64_t*) Rconfig.PHNUM);
	Write_CFGFlash(RTC_timer_addr, (uint64_t*) Rconfig.Rtime);
	HAL_FLASH_Write_Buff(CKEY1_ADDR, (uint64_t*) Rconfig.CKEY1,
			strlen(Rconfig.CKEY1));
	HAL_FLASH_Write_Buff(CKEY2_ADDR, (uint64_t*) Rconfig.CKEY2,
			strlen(Rconfig.CKEY2));
	HAL_FLASH_Write_Buff(CKEY3_ADDR, (uint64_t*) Rconfig.CKEY3,
			strlen(Rconfig.CKEY3));
	HAL_FLASH_Write_Buff(CKEY4_ADDR, (uint64_t*) Rconfig.CKEY4,
			strlen(Rconfig.CKEY4));
	HAL_FLASH_Write_Buff(CKEY5_ADDR, (uint64_t*) Rconfig.CKEY5,
			strlen(Rconfig.CKEY5));
	HAL_FLASH_Write_Buff(CKEY6_ADDR, (uint64_t*) Rconfig.CKEY6,
			strlen(Rconfig.CKEY6));
	HAL_FLASH_Write_Buff(CKEY7_ADDR, (uint64_t*) Rconfig.CKEY7,
			strlen(Rconfig.CKEY7));
	HAL_FLASH_Write_Buff(CKEY8_ADDR, (uint64_t*) Rconfig.CKEY8,
			strlen(Rconfig.CKEY8));
	 Write_CFGFlash(FL_On_Sl_Addr, (uint64_t*)&FLAG_RESET_On_Sleep);
	// Write other fields similarly
//	Write_CFGFlash(ADDR_L1, L1);
//	Write_CFGFlash(ADDR_L2, L2);
//	Write_CFGFlash(ADDR_L3, L3);

}
// Implementations of getter functions
bool GET_MIP() {
	memset(RB, 0, sizeof(RB));
	if (FLASH_Read_Buff(MQTT_IP, RB, 16) == HAL_OK) {
		Rconfig.MIP = (char*) malloc(strlen(RB) + 1); // Allocate memory for string
		if (Rconfig.MIP != NULL) {
			strcpy(Rconfig.MIP, RB);
			if (RB[0] == 255) {
				strcpy(Rconfig.MIP, "182.71.127.241");
			}
			setTcpServerIP(Rconfig.MIP);
			printc(MinDelay, "MIP: %s\n", Rconfig.MIP);
			return true;
		} else {
			printc(MinDelay, "Memory allocation failed for MIP!\n");
		}
	} else {
		printc(MinDelay, "Failed to read MIP from flash!\n");
	}
	return false;
}

bool GET_PORT1() {
	memset(RB, 0, sizeof(RB));
	if (FLASH_Read_Buff(MQTT_PORT, RB, 16) == HAL_OK) {
		Rconfig.MPORT1 = (char*) malloc(strlen(RB) + 1);
		if (Rconfig.MPORT1 != NULL) {
			strcpy(Rconfig.MPORT1, RB);
			if (RB[0] == 255) {
				strcpy(Rconfig.MPORT1, "4034");
			}
			setTcpPort(atoi(Rconfig.MPORT1));
			printc(MinDelay, "PORT1: %s\n", Rconfig.MPORT1);
			return true;
		}
	} else {
		printc(MinDelay, "Failed to read PORT1 from flash!\n");
	}
	return false;
}

bool GET_MUSER(char *opBuffer) {
	memset(RB, 0, sizeof(RB));
	if (FLASH_Read_Buff(MQTT_USER, RB, 16) == HAL_OK) {
		Rconfig.MUSER = (char*) malloc(strlen(RB) + 1); // Allocate memory for string
		if (Rconfig.MUSER != NULL) {
			strcpy(Rconfig.MUSER, RB);
			strcpy(opBuffer, RB);
			printc(MinDelay, "MUSER: %s\n", Rconfig.MUSER);
			return true;
		} else {
			printc(MinDelay, "Memory allocation failed for MUSER!\n");
		}
	} else {
		printc(MinDelay, "Failed to read MUSER from flash!\n");
	}
	return false;
}

bool GET_MPASS(char *opBuffer) {
	memset(RB, 0, sizeof(RB));
	if (FLASH_Read_Buff(MQTT_PSW, RB, 16) == HAL_OK) {
		Rconfig.MPASS = (char*) malloc(strlen(RB) + 1); // Allocate memory for string
		if (Rconfig.MPASS != NULL) {
			strcpy(Rconfig.MPASS, RB);
			printc(MinDelay, "MPASS: %s\n", Rconfig.MPASS);
		} else {
			printc(MinDelay, "Memory allocation failed for MPASS!\n");
		}
	} else {
		printc(MinDelay, "Failed to read MPASS from flash!\n");
	}
}

bool GET_CERT(char *opBuffer) {
	memset(RB, 0, sizeof(RB));
	if (FLASH_Read_Buff(MQTT_CERTIFICATE, RB, 16) == HAL_OK) {
		Rconfig.CERT = (char*) malloc(strlen(RB) + 1); // Allocate memory for string
		if (Rconfig.CERT != NULL) {
			strcpy(Rconfig.CERT, RB);
			printc(MinDelay, "CERT: %s\n", Rconfig.CERT);
		} else {
			printc(MinDelay, "Memory allocation failed for CERT!\n");
		}
	} else {
		printc(MinDelay, "Failed to read CERT from flash!\n");
	}
}

bool GET_KEY(char *opBuffer) {
	memset(RB, 0, sizeof(RB));
	if (FLASH_Read_Buff(MQTT_KEY, RB, 16) == HAL_OK) {
		Rconfig.KEY = (char*) malloc(strlen(RB) + 1); // Allocate memory for string
		if (Rconfig.KEY != NULL) {
			strcpy(Rconfig.KEY, RB);
			printc(MinDelay, "KEY: %s\n", Rconfig.KEY);
		} else {
			printc(MinDelay, "Memory allocation failed for KEY!\n");
		}
	} else {
		printc(MinDelay, "Failed to read KEY from flash!\n");
	}
}

bool GET_CA(char *opBuffer) {
	memset(RB, 0, sizeof(RB));
	if (FLASH_Read_Buff(MQTT_CERTIFICATE_AUTHORITY, RB, 16) == HAL_OK) {
		Rconfig.CA = (char*) malloc(strlen(RB) + 1); // Allocate memory for string
		if (Rconfig.CA != NULL) {
			strcpy(Rconfig.CA, RB);
			printc(MinDelay, "CA: %s\n", Rconfig.CA);
		} else {
			printc(MinDelay, "Memory allocation failed for CA!\n");
		}
	} else {
		printc(MinDelay, "Failed to read CA from flash!\n");
	}
}

bool GET_IP2(char *opBuffer) {
	memset(RB, 0, sizeof(RB));
	if (FLASH_Read_Buff(FOTA_IP2, RB, 16) == HAL_OK) {
		Rconfig.IP2 = (char*) malloc(strlen(RB) + 1); // Allocate memory for string
		if (Rconfig.IP2 != NULL) {
			strcpy(Rconfig.IP2, RB);
			printc(MinDelay, "IP2: %s\n", Rconfig.IP2);
		} else {
			printc(MinDelay, "Memory allocation failed for IP2!\n");
		}
	} else {
		printc(MinDelay, "Failed to read IP2 from flash!\n");
	}
}

bool GET_PORT2(char *opBuffer) {
	memset(RB, 0, sizeof(RB));
	if (FLASH_Read_Buff(FOTA_PORT2, RB, 16) == HAL_OK) {
		Rconfig.IPPORT2 = (char*) malloc(strlen(RB) + 1);
		strcpy(Rconfig.IPPORT2, RB);
		printc(MinDelay, "PORT2: %d\n", Rconfig.IPPORT2);
	} else {
		printc(MinDelay, "Failed to read PORT2 from flash!\n");
	}
}

bool GET_SKEY1(char *opBuffer) {
	memset(RB, 0, sizeof(RB));
	if (FLASH_Read_Buff(FOTA_Static_Key1, RB, 32) == HAL_OK) {
		Rconfig.SKEY1 = (char*) malloc(strlen(RB) + 1); // Allocate memory for string
		if (Rconfig.SKEY1 != NULL) {
			strcpy(Rconfig.SKEY1, RB);
			printc(MinDelay, "SKEY1: %s\n", Rconfig.SKEY1);
		} else {
			printc(MinDelay, "Memory allocation failed for SKEY1!\n");
		}
	} else {
		printc(MinDelay, "Failed to read SKEY1 from flash!\n");
	}
}

bool GET_SKEY2(char *opBuffer) {
	memset(RB, 0, sizeof(RB));
	if (FLASH_Read_Buff(FOTA_Static_Key2, RB, 32) == HAL_OK) {
		Rconfig.SKEY2 = (char*) malloc(strlen(RB) + 1); // Allocate memory for string
		if (Rconfig.SKEY2 != NULL) {
			strcpy(Rconfig.SKEY2, RB);
			printc(MinDelay, "SKEY2: %s\n", Rconfig.SKEY2);
		} else {
			printc(MinDelay, "Memory allocation failed for SKEY2!\n");
		}
	} else {
		printc(MinDelay, "Failed to read SKEY2 from flash!\n");
	}
}

bool GET_SKEY3(char *opBuffer) {
	memset(RB, 0, sizeof(RB));
	if (FLASH_Read_Buff(FOTA_Static_Key3, RB, 32) == HAL_OK) {
		Rconfig.SKEY3 = (char*) malloc(strlen(RB) + 1); // Allocate memory for string
		if (Rconfig.SKEY3 != NULL) {
			strcpy(Rconfig.SKEY3, RB);
			printc(MinDelay, "SKEY3: %s\n", Rconfig.SKEY3);
		} else {
			printc(MinDelay, "Memory allocation failed for SKEY3!\n");
		}
	} else {
		printc(MinDelay, "Failed to read SKEY3 from flash!\n");
	}
}

bool GET_APN(char *opBuffer) {
	memset(RB, 0, sizeof(RB));
	if (FLASH_Read_Buff(APN, RB, 16) == HAL_OK) {
		Rconfig.DAPN = (char*) malloc(strlen(RB) + 1); // Allocate memory for string
		if (Rconfig.DAPN != NULL) {
			strcpy(Rconfig.DAPN, RB);
			printc(MinDelay, "APN: %s\n", Rconfig.DAPN);
		} else {
			printc(MinDelay, "Memory allocation failed for APN!\n");
		}
	} else {
		printc(MinDelay, "Failed to read APN from flash!\n");
	}
}

// Implement the remaining GET functions in a similar manner

bool GET_ION(char *opBuffer) {
	memset(RB, 0, sizeof(RB));
	if (FLASH_Read_Buff(ION_Sampling, RB, 16) == HAL_OK) {
		Rconfig.ION = (char*) malloc(strlen(RB) + 1); // Allocate memory for string
		if (Rconfig.ION != NULL) {
			strcpy(Rconfig.ION, RB);
			if (RB[0] == 255) {
				strcpy(Rconfig.ION, "08");
			}
			setSampling(atoi(Rconfig.ION));
			printc(MinDelay, "ION: %s\n", Rconfig.ION);
		} else {
			printc(MinDelay, "Memory allocation failed for ION!\n");
		}
	} else {
		printc(MinDelay, "Failed to read ION from flash!\n");
	}
}

bool GET_NSAM(char *opBuffer) {
	memset(RB, 0, sizeof(RB));
	if (FLASH_Read_Buff(NO_OF_Sampling, RB, 16) == HAL_OK) {
		Rconfig.NSAM = (char*) malloc(strlen(RB) + 1); // Allocate memory for string
		if (Rconfig.NSAM != NULL) {
			strcpy(Rconfig.NSAM, RB);
			printc(MinDelay, "NSAM: %s\n", Rconfig.NSAM);
		} else {
			printc(MinDelay, "Memory allocation failed for NSAM!\n");
		}
	} else {
		printc(MinDelay, "Failed to read NSAM from flash!\n");
	}
}

bool GET_HA(char *opBuffer) {
	memset(RB, 0, sizeof(RB));
	if (FLASH_Read_Buff(HARSH_ACC_TH, RB, 16) == HAL_OK) {
		Rconfig.HA = (char*) malloc(strlen(RB) + 1); // Allocate memory for string
		if (Rconfig.HA != NULL) {
			strcpy(Rconfig.HA, RB);
			printc(MinDelay, "HA: %s\n", Rconfig.HA);
		} else {
			printc(MinDelay, "Memory allocation failed for HA!\n");
		}
	} else {
		printc(MinDelay, "Failed to read HA from flash!\n");
	}
}

bool GET_HB(char *opBuffer) {
	memset(RB, 0, sizeof(RB));
	if (FLASH_Read_Buff(HARSH_DEACC_TH, RB, 16) == HAL_OK) {
		Rconfig.HB = (char*) malloc(strlen(RB) + 1); // Allocate memory for string
		if (Rconfig.HB != NULL) {
			strcpy(Rconfig.HB, RB);
			printc(MinDelay, "HB: %s\n", Rconfig.HB);
		} else {
			printc(MinDelay, "Memory allocation failed for HB!\n");
		}
	} else {
		printc(MinDelay, "Failed to read HB from flash!\n");
	}
}

bool GET_HC(char *opBuffer) {
	memset(RB, 0, sizeof(RB));
	if (FLASH_Read_Buff(HARSH_CORNERING_TH, RB, 16) == HAL_OK) {
		Rconfig.HC = (char*) malloc(strlen(RB) + 1); // Allocate memory for string
		if (Rconfig.HC != NULL) {
			strcpy(Rconfig.HC, RB);
			printc(MinDelay, "HC: %s\n", Rconfig.HC);
		} else {
			printc(MinDelay, "Memory allocation failed for HC!\n");
		}
	} else {
		printc(MinDelay, "Failed to read HC from flash!\n");
	}
}

bool GET_ACCOFFSET(char *opBuffer) {
	memset(RB, 0, sizeof(RB));
	if (FLASH_Read_Buff(SENSOR_OFFSET_CAL, RB, 16) == HAL_OK) {
		Rconfig.ACCOFFSET = (char*) malloc(strlen(RB) + 1); // Allocate memory for string
		if (Rconfig.ACCOFFSET != NULL) {
			strcpy(Rconfig.ACCOFFSET, RB);
			printc(MinDelay, "ACCOFFSET: %s\n", Rconfig.ACCOFFSET);
		} else {
			printc(MinDelay, "Memory allocation failed for ACCOFFSET!\n");
		}
	} else {
		printc(MinDelay, "Failed to read ACCOFFSET from flash!\n");
	}
}

bool GET_USER(char *opBuffer) {
	memset(RB, 0, sizeof(RB));
	if (FLASH_Read_Buff(USERNAME, RB, 16) == HAL_OK) {
		Rconfig.USER = (char*) malloc(strlen(RB) + 1); // Allocate memory for string
		if (Rconfig.USER != NULL) {
			strcpy(Rconfig.USER, RB);
			printc(MinDelay, "USER: %s\n", Rconfig.USER);
		} else {
			printc(MinDelay, "Memory allocation failed for USER!\n");
		}
	} else {
		printc(MinDelay, "Failed to read USER from flash!\n");
	}
}

bool GET_PASSWORD(char *opBuffer) {
	memset(RB, 0, sizeof(RB));
	if (FLASH_Read_Buff(PASSWORD, RB, 16) == HAL_OK) {
		Rconfig.PSWD = (char*) malloc(strlen(RB) + 1); // Allocate memory for string
		if (Rconfig.PSWD != NULL) {
			strcpy(Rconfig.PSWD, RB);
			printc(MinDelay, "PASSWORD: %s\n", Rconfig.PSWD);
		} else {
			printc(MinDelay, "Memory allocation failed for PASSWORD!\n");
		}
	} else {
		printc(MinDelay, "Failed to read PASSWORD from flash!\n");
	}
}

bool GET_VIN(char *opBuffer) {
	memset(RB, 0, sizeof(RB));
	if (FLASH_Read_Buff(VECHILE_ID_NUM, RB, 16) == HAL_OK) {
		Rconfig.VIN = (char*) malloc(strlen(RB) + 1); // Allocate memory for string
		if (Rconfig.VIN != NULL) {
			strcpy(Rconfig.VIN, RB);
			printc(MinDelay, "VIN: %s\n", Rconfig.VIN);
		} else {
			printc(MinDelay, "Memory allocation failed for VIN!\n");
		}
	} else {
		printc(MinDelay, "Failed to read VIN from flash!\n");
	}
}

bool GET_LAT() {
	memset(RB, 0, sizeof(RB));
	if (FLASH_Read_Buff(LAT_ADDR, RB, 16) == HAL_OK) {
		Rconfig.LAT = (char*) malloc(strlen(RB) + 1); // Allocate memory for string
		if (Rconfig.LAT != NULL) {
			if (RB[0] != 255)
				strcpy(Rconfig.LAT, RB);
			else {
				strcpy(Rconfig.LAT, "0");
			}
			printc(MinDelay, "LAT: %s\n", Rconfig.LAT);
		} else {
			printc(MinDelay, "Memory allocation failed for LAT!\n");
		}
	} else {
		printc(MinDelay, "Failed to read LAT from flash!\n");
	}
	return 0;
}

bool GET_LON() {
	memset(RB, 0, sizeof(RB));
	if (FLASH_Read_Buff(LON_ADDR, RB, 16) == HAL_OK) {
		Rconfig.LON = (char*) malloc(strlen(RB) + 1); // Allocate memory for string
		if (Rconfig.LON != NULL) {
			if (RB[0] != 255)
				strcpy(Rconfig.LON, RB);
			else {
				strcpy(Rconfig.LON, "0");
			}
			printc(MinDelay, "LON: %s\n", Rconfig.LON);
		} else {
			printc(MinDelay, "Memory allocation failed for LON!\n");
		}
	} else {
		printc(MinDelay, "Failed to read LON from flash!\n");
	}
	return true;
}

bool GET_PHNUM() {
	memset(RB, 0, sizeof(RB));
	if (FLASH_Read_Buff(SMS_NUM_Addr, RB, 16) == HAL_OK) {
		Rconfig.PHNUM = (char*) malloc(strlen(RB) + 1); // Allocate memory for string
		if (Rconfig.PHNUM != NULL) {
			strcpy(Rconfig.PHNUM, RB);
			memcpy(&PHnum ,Rconfig.PHNUM,10);
			printc(MinDelay, "PHNUM: %s\n", Rconfig.PHNUM);
		} else {
			printc(MinDelay, "Memory allocation failed for PHNUM!\n");
		}
	} else {
		printc(MinDelay, "Failed to read PHNUM from flash!\n");
	}
	return true;
}
bool GET_RTCTimer() {
	memset(RB, 0, sizeof(RB));
	if (FLASH_Read_Buff(RTC_timer_addr, RB, 16) == HAL_OK) {
		Rconfig.Rtime = (char*) malloc(strlen(RB) + 1); // Allocate memory for string
		if (Rconfig.Rtime != NULL) {
			strcpy(Rconfig.Rtime, RB);
			printc(MinDelay, "RTCWaketime: %s\n", Rconfig.Rtime);
		} else {
			printc(MinDelay, "Memory allocation failed for Rtime!\n");
		}
	} else {
		printc(MinDelay, "Failed to read Rtime from flash!\n");
	}
	return true;
}

bool GET_ALOC() {
	char LRB[16]; // Assuming max size of data is 16 bytes

	memset(LRB, 0, sizeof(LRB));
	if (FLASH_Read_Buff(ADDR_L1, LRB, sizeof(LRB)) == HAL_OK) {
		L1 = atoi(LRB); // Convert string to uint32_t
		printf("L1: %lu\n", L1); // Print L1 (if needed)
	} else {
		printf("Failed to read L1 from flash!\n");
		return false;
	}

	memset(LRB, 0, sizeof(LRB));
	if (FLASH_Read_Buff(ADDR_L2, LRB, sizeof(LRB)) == HAL_OK) {
		L2 = atoi(LRB); // Convert string to uint32_t
		printf("L2: %lu\n", L2); // Print L2 (if needed)
	} else {
		printf("Failed to read L2 from flash!\n");
		return false;
	}

	memset(LRB, 0, sizeof(LRB));
	if (FLASH_Read_Buff(ADDR_L3, LRB, sizeof(LRB)) == HAL_OK) {
		L3 = atoi(LRB); // Convert string to uint32_t
		printf("L3: %lu\n", L3); // Print L3 (if needed)
	} else {
		printf("Failed to read L3 from flash!\n");
		return false;
	}
//	sprintf(opBuffer, "%lu %lu %lu", L1, L2, L3);
	return true;
}
bool GET_Ckey() {
	memset(RB, 0, sizeof(RB));
	if (FLASH_Read_Buff(CKEY1_ADDR, RB, 32) == HAL_OK) {
		Rconfig.CKEY1 = (char*) malloc(strlen(RB) + 1); // Allocate memory for string
		if (Rconfig.CKEY1 != NULL) {
			strcpy(Rconfig.CKEY1, RB);
			printc(MinDelay, "CKEY1: %s\n", Rconfig.CKEY1);
		} else {
			printc(MinDelay, "Memory allocation failed for Ckey1!\n");
		}
	} else {
		printc(MinDelay, "Failed to read CKEY1 from flash!\n");
	}
	memset(RB, 0, sizeof(RB));
	if (FLASH_Read_Buff(CKEY2_ADDR, RB, 32) == HAL_OK) {
		Rconfig.CKEY2 = (char*) malloc(strlen(RB) + 1); // Allocate memory for string
		if (Rconfig.CKEY2 != NULL) {
			strcpy(Rconfig.CKEY2, RB);
			printc(MinDelay, "CKEY2: %s\n", Rconfig.CKEY2);
		} else {
			printc(MinDelay, "Memory allocation failed for CKEY2!\n");
		}
	} else {
		printc(MinDelay, "Failed to read CKEY2 from flash!\n");
	}
	memset(RB, 0, sizeof(RB));
	if (FLASH_Read_Buff(CKEY3_ADDR, RB, 32) == HAL_OK) {
		Rconfig.CKEY3 = (char*) malloc(strlen(RB) + 1); // Allocate memory for string
		if (Rconfig.CKEY3 != NULL) {
			strcpy(Rconfig.CKEY3, RB);
			printc(MinDelay, "CKEY3: %s\n", Rconfig.CKEY3);
		} else {
			printc(MinDelay, "Memory allocation failed for CKEY3!\n");
		}
	} else {
		printc(MinDelay, "Failed to read CKEY3 from flash!\n");
	}
	memset(RB, 0, sizeof(RB));
	if (FLASH_Read_Buff(CKEY4_ADDR, RB, 32) == HAL_OK) {
		Rconfig.CKEY4 = (char*) malloc(strlen(RB) + 1); // Allocate memory for string
		if (Rconfig.CKEY4 != NULL) {
			strcpy(Rconfig.CKEY4, RB);
			printc(MinDelay, "CKEY4: %s\n", Rconfig.CKEY4);
		} else {
			printc(MinDelay, "Memory allocation failed for CKEY4!\n");
		}
	} else {
		printc(MinDelay, "Failed to read CKEY4 from flash!\n");
	}
	memset(RB, 0, sizeof(RB));
	if (FLASH_Read_Buff(CKEY5_ADDR, RB, 32) == HAL_OK) {
		Rconfig.CKEY5 = (char*) malloc(strlen(RB) + 1); // Allocate memory for string
		if (Rconfig.CKEY5 != NULL) {
			strcpy(Rconfig.CKEY5, RB);
			printc(MinDelay, "CKEY5: %s\n", Rconfig.CKEY5);
		} else {
			printc(MinDelay, "Memory allocation failed for CKEY5!\n");
		}
	} else {
		printc(MinDelay, "Failed to read CKEY5 from flash!\n");
	}
	memset(RB, 0, sizeof(RB));
	if (FLASH_Read_Buff(CKEY6_ADDR, RB, 32) == HAL_OK) {
		Rconfig.CKEY6 = (char*) malloc(strlen(RB) + 1); // Allocate memory for string
		if (Rconfig.CKEY6 != NULL) {
			strcpy(Rconfig.CKEY6, RB);
			printc(MinDelay, "CKEY6: %s\n", Rconfig.CKEY6);
		} else {
			printc(MinDelay, "Memory allocation failed for CKEY6!\n");
		}
	} else {
		printc(MinDelay, "Failed to read CKEY6 from flash!\n");
	}
	memset(RB, 0, sizeof(RB));
	if (FLASH_Read_Buff(CKEY7_ADDR, RB, 32) == HAL_OK) {
		Rconfig.CKEY7 = (char*) malloc(strlen(RB) + 1); // Allocate memory for string
		if (Rconfig.CKEY7 != NULL) {
			strcpy(Rconfig.CKEY7, RB);
			printc(MinDelay, "CKEY7: %s\n", Rconfig.CKEY7);
		} else {
			printc(MinDelay, "Memory allocation failed for CKEY7!\n");
		}
	} else {
		printc(MinDelay, "Failed to read CKEY7 from flash!\n");
	}
	memset(RB, 0, sizeof(RB));
	if (FLASH_Read_Buff(CKEY8_ADDR, RB, 32) == HAL_OK) {
		Rconfig.CKEY8 = (char*) malloc(strlen(RB) + 1); // Allocate memory for string
		if (Rconfig.CKEY8 != NULL) {
			strcpy(Rconfig.CKEY8, RB);
			printc(MinDelay, "CKEY8: %s\n", Rconfig.CKEY8);
		} else {
			printc(MinDelay, "Memory allocation failed for CKEY8!\n");
		}
	} else {
		printc(MinDelay, "Failed to read CKEY8 from flash!\n");
	}

}

bool SGET_ALOC() {
	L1 = L1_v1;
	L2 = L2_v2;
	L3 = L3_v2;
}

