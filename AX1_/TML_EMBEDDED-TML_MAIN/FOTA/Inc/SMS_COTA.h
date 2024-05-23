/*
 * SMS_COTA.h
 *
 *  Created on: May 7, 2024
 *      Author: Manky
 */

#include "main.h"
#include "Flash.h"

#ifndef INC_SMS_COTA_H_
#define INC_SMS_COTA_H_

/************************************************SMS_COTA_STORAGE_SECTOR-181****************************************************/

#define SMS_COTA_SEC								((uint32_t)0x0816A000)
#define MQTT_IP 									((uint32_t)0x0816A010)
#define MQTT_PORT 									((uint32_t)0x0816A030)
#define MQTT_USER									((uint32_t)0x0816A050)
#define MQTT_PSW 									((uint32_t)0x0816A070)
#define MQTT_CERTIFICATE 							((uint32_t)0x0816A090)
#define MQTT_KEY 									((uint32_t)0x0816A0B0)
#define MQTT_CERTIFICATE_AUTHORITY 					((uint32_t)0x0816A0E0)
#define FOTA_IP2 									((uint32_t)0x0816A110)
#define FOTA_PORT2 									((uint32_t)0x0816A130)
#define FOTA_Static_Key1 							((uint32_t)0x0816A150)
#define FOTA_Static_Key2							((uint32_t)0x0816A1A0)
#define FOTA_Static_Key3							((uint32_t)0x0816A1E0)
#define APN 										((uint32_t)0x0816A220)
#define ION_Sampling								((uint32_t)0x0816A250)
#define NO_OF_Sampling 								((uint32_t)0x0816A270)
#define HARSH_ACC_TH 								((uint32_t)0x0816A290)
#define HARSH_DEACC_TH	 							((uint32_t)0x0816A2B0)
#define HARSH_CORNERING_TH							((uint32_t)0x0816A2D0)
#define SENSOR_OFFSET_CAL 							((uint32_t)0x0816A2F0)
#define USERNAME 									((uint32_t)0x0816A310)
#define PASSWORD		 							((uint32_t)0x0816A330)
#define VECHILE_ID_NUM	 							((uint32_t)0x0816A350)
#define ECU_Part_Number 							((uint32_t)0x0816A370)
#define ECU_Assembly_Number							((uint32_t)0x0816A390)
#define UPDATE_FW_CMD								((uint32_t)0x0816A3B0)

#define ADDR_L1 									((uint32_t)0x0816A3D0)
#define ADDR_L2										((uint32_t)0x0816A3F0)
#define ADDR_L3 									((uint32_t)0x0816A410)

#define CKEY1_ADDR									((uint32_t)0x0816A430)
#define CKEY2_ADDR									((uint32_t)0x0816A450)
#define CKEY3_ADDR									((uint32_t)0x0816A470)
#define CKEY4_ADDR									((uint32_t)0x0816A490)
#define CKEY5_ADDR									((uint32_t)0x0816A4B0)
#define CKEY6_ADDR									((uint32_t)0x0816A4D0)
#define CKEY7_ADDR									((uint32_t)0x0816A4F0)
#define CKEY8_ADDR									((uint32_t)0x0816A520)
/***********************************************************END_OF_ALLOCATION*****************************************************/

//
//typedef struct {
//	char *MIP;
//	int MPORT1;
//	char *MUSER;
//	char *MPASS;
//	char *CERT;
//	char *KEY;
//	char *CA;
//	char *IP2;
//	int IPPORT2;
//	char *SKEY1;
//	char *SKEY2;
//	char *SKEY3;
//	char *DAPN;
//	char *ION;
//	char *NSAM;
//	char *HA;
//	char *HB;
//	char *HC;
//	char *ACCOFFSET;
//	char *USER;
//	char *PSWD;
//	char *VIN;
//} Configuration;

typedef struct {
	char *MIP;
	char *MPORT1;
	char *MUSER;
	char *MPASS;
	char *CERT;
	char *KEY;
	char *CA;
	char *IP2;
	char *IPPORT2;
	char *SKEY1;
	char *SKEY2;
	char *SKEY3;
	char *DAPN;
	char *ION;
	char *NSAM;
	char *HA;
	char *HB;
	char *HC;
	char *ACCOFFSET;
	char *USER;
	char *PSWD;
	char *VIN;
	char *FOTAVER;
	char *CKEY1;
	char *CKEY2;
	char *CKEY3;
	char *CKEY4;
	char *CKEY5;
	char *CKEY6;
	char *CKEY7;
	char *CKEY8;

} RConfiguration;

bool SET_MIP(char *ip_address);
bool SET_PORT1(char *port);
bool SET_MUSER(char *username);
bool SET_MPASS(char *password);
bool SET_CERT(char *filename);
bool SET_KEY(char *filename);
bool SET_CA(char *filename);
bool SET_IP2(char *address);
bool SET_PORT2(char *value);
bool SET_SKEY1(char *keyValue);
bool SET_SKEY2(char *keyValue);
bool SET_SKEY3(char *keyValue);
bool SET_APN(char *number);
bool SET_ION(char *number);
bool SET_NSAM(char *number);
bool SET_HA(char *number);
bool SET_HB(char *number);
bool SET_HC(char *number);
bool SET_ACCOFFSET(char *number);
bool SET_USER(char *idVal);
bool SET_PASSWORD(char *passwordVal);
bool SET_VIN(char *number);

bool SET_FOTA_UPDATE(char *Version);

bool GET_MIP();
bool GET_PORT1();
bool GET_MUSER(char*);
bool GET_MPASS(char*);
bool GET_CERT(char*);
bool GET_KEY(char*);
bool GET_CA(char*);
bool GET_IP2(char*);
bool GET_PORT2(char*);
bool GET_SKEY1(char*);
bool GET_SKEY2(char*);
bool GET_SKEY3(char*);
bool GET_APN(char*);
bool GET_ION(char*);
bool GET_NSAM(char*);
bool GET_HA(char*);
bool GET_HB(char*);
bool GET_HC(char*);
bool GET_ACCOFFSET(char*);
bool GET_USER(char*);
bool GET_PASSWORD(char*);
bool GET_VIN(char*);
bool SMS_COTA(char *SMSBuff,char *opBuffer);
void getAllConfig();
void Erase_SMSCOTA_SEC();
void Write_CFGFlash (uint32_t WriteLoc, uint64_t *DATA);
bool getUpdateStatus();
void setUpdateStaus(bool upStatus);
bool GET_ALOC();
bool SET_FOTA_SIZE(char *SZE);
bool SGET_ALOC ();
void UPDATE_COMKeys();
bool GET_Ckey();
#endif /* INC_SMS_COTA_H_ */
