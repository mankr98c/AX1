

#include"EC200Utils.h"

UART_HandleTypeDef *ec200MqttUart;
uint8_t rx_data[2];
uint8_t EC200_ResponseBuff[RESBUFF_SIZE];
int responseIndex=0;
bool enable_Idle = true;
bool FLAG_EC200RESONSE=false;
char tmpbuff[100]={};

bool wait_for_response(char *resBuff,int timeout){
	bool status=false;
	FLAG_EC200RESONSE=false;
	int count=0;
	while(!FLAG_EC200RESONSE&&(HAL_GetTick()% timeout)!=0)
	     {
		   count++;
//	    	 printc(MinDelay,"Waiting for response %d\n",HAL_GetTick()% EC200MAXRESONSETIME);
	     }
	     if(FLAG_EC200RESONSE){
	    	 memcpy(resBuff,EC200_ResponseBuff,strlen(EC200_ResponseBuff));
	    	 memset(EC200_ResponseBuff,0,RESBUFF_SIZE);
	    	 status=true;
	     }
	 printc(MinDelay,"wait Response count %d  flag %d status %d\n",count,FLAG_EC200RESONSE,status);
	 FLAG_EC200RESONSE=false;
	 return status;
}
bool EC200_Transmit(const char *command,char *resBuff,int timeout){
     HAL_UART_Transmit(ec200MqttUart, (uint8_t*)command,strlen(command), 2000);
     if(resBuff==NULL)
    	 return false;
     return wait_for_response(resBuff,timeout);
 }
bool EC200_Init(void) {
	int EC200_counter=0;
    //Transmit AT commands for EC20 initialization
	uint32_t resbuff[100]={};
    if(EC200_Transmit("AT\r\n",resbuff,EC200MINRESONSETIME))
    {
    	printc(MinDelay,"Res For At is %s\n",resbuff);
    }
    memset(resbuff,0,100);
    if(EC200_Transmit("AT+QURCCFG=\"urcport\",\"uart1\"\r\n",resbuff,EC200MINRESONSETIME))
    {
    	printc(MinDelay,"Res For AT+QURCCFG=\"urcport\",\"uart1\"\r\n is %s\n",resbuff);
    }
    memset(resbuff,0,100);
    if(EC200_Transmit("AT+IPR=115200\r\n",resbuff,EC200MINRESONSETIME))
	{
		printc(MinDelay,"Res For AT+IPR=115200 is %s\n",resbuff);
	}
    memset(resbuff,0,100);
    if(EC200_Transmit("ATV1\r\n",resbuff,EC200MINRESONSETIME))
   {
    		printc(MinDelay,"Res For ATV1 is %s\n",resbuff);
    }
    memset(resbuff,0,100);
    if(EC200_Transmit("ATE0\r\n",resbuff,EC200MINRESONSETIME))
   {
    		printc(MinDelay,"Res For ATE1 is %s\n",resbuff);
    }
    memset(resbuff,0,100);
    if(EC200_Transmit("AT+CMEE=2\r\n",resbuff,EC200MINRESONSETIME))
     {
      		printc(MinDelay,"Res For AT+CMEE=2 is %s\n",resbuff);
      }
    memset(resbuff,0,100);
    if(EC200_Transmit("ATI\r\n",resbuff,EC200MINRESONSETIME))
      {
          		printc(MinDelay,"Res For ATI is %s\n",resbuff);
        }
    memset(resbuff,0,100);
    if (EC200_Transmit("AT+GSN\r\n", resbuff,EC200MINRESONSETIME)) {
        printc(MinDelay, "Res For GSN is %s\n", resbuff);
    }

    memset(resbuff,0,sizeof(resbuff));
    CPIN:
	EC200_counter=0;
    if (EC200_Transmit("AT+CPIN?\r\n", resbuff,EC200MINRESONSETIME)) {
        printc(MinDelay, "Res For CPIN is %s\n", resbuff);
        if(strstr(resbuff,"+CPIN: READY")==NULL){
        	if(EC200_counter<3){
        		EC200_counter++;
        		goto CPIN;
        	}
        	else{
        		printc(MinDelay,"Sim Not Detected");
        		return false;
        	}
        }
    }
    memset(resbuff,0,sizeof(resbuff));
    network:
	EC200_counter=0;
    if (EC200_Transmit("AT+CSQ\r\n", resbuff,2000)) {
        printc(MinDelay, "Res For CSQ is %s\n", resbuff);
        if(strstr(resbuff,"+CSQ: 99,99")!=NULL){
        	if(EC200_counter<3){
        		EC200_counter++;
        		goto network;

        	}
			else{
				printc(MinDelay,"Signal Not found");
				return false;
			}
        }


    }
    memset(resbuff,0,100);

    if (EC200_Transmit("AT+CREG?\r\n", resbuff,EC200MINRESONSETIME)) {
        printc(MinDelay, "Res For CREG is %s\n", resbuff);
    }
    memset(resbuff,0,100);

    if (EC200_Transmit("AT+CGREG?\r\n", resbuff,EC200MINRESONSETIME)) {
        printc(MinDelay, "Res For CGREG is %s\n", resbuff);
    }
    memset(resbuff,0,100);
    if (EC200_Transmit("AT+COPS?\r\n", resbuff,EC200MINRESONSETIME)) {
        printc(MinDelay, "Res For COPS is %s\n", resbuff);
    }
    memset(resbuff,0,100);
    if (EC200_Transmit("AT+QCCID\r\n", resbuff,EC200MINRESONSETIME)) {
        printc(MinDelay, "Res For QCCID is %s\n", resbuff);
    }
    memset(resbuff,0,100);
    if (EC200_Transmit("AT+CIMI\r\n", resbuff,EC200MINRESONSETIME)) {
        printc(MinDelay, "Res For CIMI is %s\n", resbuff);
    }

}
//will fill the opbuffer if value available else return false
bool getIMEI(char *opbuffer){
	char tmpIMEI[15]={0};
	if(EC200_Transmit("AT+GSN\r\n",opbuffer,EC200MINRESONSETIME))
	 {
//			i have to check why string split function is not working
			int length=(int)(strstr(&opbuffer[2],"\r\n")-(char*)&opbuffer[2]);
			memcpy(tmpIMEI,&opbuffer[2],length);
			clearBuffer(tmpbuff,sizeof(tmpbuff));
			strcpy(opbuffer,tmpIMEI);
			printc(MinDelay,"Res For AT+GSN %s\n",opbuffer);
	    	return true;
	 }
	return false;
}
int getSignal(void){
	uint8_t resbuff[100]={};
	uint8_t signal=-1;
	memset(resbuff,0,sizeof(resbuff));
	if(EC200_Transmit("AT+CSQ\r\n",resbuff,EC200MINRESONSETIME))
	{
		sscanf(resbuff,"\r\n+CSQ: %d",&signal);
		printc(MinDelay,"RSSI For AT+CSQ %s\n",resbuff);
		return signal;
	}
		return -1;

}

bool getOperatorName(char *opbuffer){
	uint8_t split[5][100]={};
	uint8_t resbuff[100]={};
	memset(resbuff,0,sizeof(resbuff));
	if(EC200_Transmit("AT+COPS?\r\n",resbuff,2000))
	  {
			clearBuffer(split, sizeof(split));
			splitString(resbuff,",",split);
			memcpy(opbuffer,split[2],strlen(split[2]));
			printc(MinDelay,"Res For AT+COPS? %s\n",resbuff);
			return true;
		}
		return false;
}

bool get_MCC_MNC_CellID(char *opMMC,char *opMNC,char *opCELL,char *CellID){
	    uint8_t split[19][100]={};
	    uint8_t resbuff[100]={};
		memset(resbuff,0,sizeof(resbuff));
		if(EC200_Transmit("AT+QENG=\"servingcell\"\r\n",resbuff,EC200MINRESONSETIME))
		{
			splitString(resbuff,",",split);
			strcpy(opMMC,split[4]);
			strcpy(opMNC,split[5]);
			strcpy(opCELL,split[6]);
			strcpy(CellID,split[7]);
//			printc(MinDelay,"Res For AT+QENG? %s\n",resbuff);
			return true;
		}
		return false;
}

bool setupEc200(UART_HandleTypeDef *uart){
	  ec200MqttUart=uart;
	  GNSS_ON();
	  GPIO_LTE_ON();
	  GPIO_3V3_ON();
	  GPS_LIGHT_ON();
	  enableIdle();
	  ec200start:
	  if(EC200_Transmit("AT\r\n",tmpbuff,EC200MINRESONSETIME)){
		  printc(MinDelay,"%s",tmpbuff);
	  }
	  else{
		 goto ec200start;
	  }
	  HAL_Delay(5000);
	  EC200_Init();
	  gsmParameterUpdate();
	  clearBuffer(tmpbuff,sizeof(tmpbuff));
	  if(getIMEI(tmpbuff)){
		   setIMEI(tmpbuff);
	  }
	  else{
		  printc(MinDelay,"Don't get IMEI\n");
		  return false;
	  }
	  return true;

}


 void E200_RxHAlfCpltCallback(void){
	 printc(MinDelay,"Half Callback\n %s \n",EC200_ResponseBuff);
 }

 void Ec200_RxEventCallback(uint16_t size){
	 printc(MinDelay,"Event Callback %d %s\n",size,EC200_ResponseBuff);
	 FLAG_EC200RESONSE=true;
 }
 void E200_RXCpltCallback(void){
	 printc(MinDelay,"Complete Callback %s \n",EC200_ResponseBuff);
 }
void switchIdle()
{
	if(enable_Idle)
		enable_Idle = false;
	else
		enable_Idle = true;


}
void enableIdle(){
	if(enable_Idle)
    	HAL_UARTEx_ReceiveToIdle_IT(ec200MqttUart,EC200_ResponseBuff,RESBUFF_SIZE);
}
