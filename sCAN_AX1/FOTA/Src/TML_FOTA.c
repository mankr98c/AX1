/*
 * TML_FOTA.c
 *
 *  Created on: Apr 23, 2024
 *      Author: 91944
 */
#include "TML_FOTA.h"
#include "main.h"
#include "ledblink.h"
#include "sms.h"
extern char PHnum[10];

void fotaReqestKey() {
	WriteF_SKey();
	EC200_HTTP_POST(1);

	WriteF_CKey();
}
bool fotaUpdate() {
	return EC200_HTTP_POST(6);
}

void COTA_Update() {
	EC200_HTTP_POST(5);
}

bool update() {

	EC200_Init();
	FUpdate();
	fotaReqestKey();

//	COTA_Update();
	if (!fotaUpdate()) {
		char SBf[] = { "FOTA_Download Failed" };
		SMS_FSend(PHnum, SBf);
		return false;
	}
	FupdateC();

	char SBf[] = { "FOTA_Download Complete" };
	SMS_FSend(PHnum, SBf);
	SET_PHNUM(PHnum);
	SET_RESET();

	return true;

}

void ReadF_Loc(int KeyType, int KeyNumber)
// KeyType Can be 1 or 2 ,  #KeyNumber Should be 1-3 for KeyType-1, #KeyNumber Should be 1-8 for KeyType-2
{
	switch (KeyType) {
	case 1:
		ReadF_sKey(KeyNumber);
		break;
	case 2:
		ReadF_cKey(KeyNumber);
	default:
		break;
	}

}

