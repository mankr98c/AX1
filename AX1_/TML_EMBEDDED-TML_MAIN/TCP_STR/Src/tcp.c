/*
 * tcp.c
 *
 *  Created on: April 23, 2024
 *      Author: saurabh
 */

#include "tcp.h"
char tcp_buff[250];
char SERVER_IP[16]= "182.71.127.241";
int SERVER_PORT= 4034;

void setTcpServerIP(char *ip){
	clearBuffer(SERVER_IP, sizeof(SERVER_IP));
	strcpy(SERVER_IP,ip);
}
void setTcpPort(int port){
	SERVER_PORT=port;
}
int getTcpPort(){
	char port[6]={};
	SGET_PORT1(port);
	return atoi(port);
}
void getTcpServerIP(char *opIP){
	SGET_MIP(opIP);
//	strcpy(opIP,SERVER_IP);
}
void clear_tcp_resp_buff()
{
	clearBuffer(tcp_buff,sizeof(tcp_buff));
}

bool TCP_init(void){
	if(TCP_config()){
		char data[]="Hello To TCP\r\n";
		if(TCP_Send(data) )
		{
			printc(MinDelay, "Data Send Success Fully\n");
		}
		else{
		printc(MinDelay, "Some error Occur during Sending\n");
		}
	}
	else{
		printc(MinDelay,"Configuration Failed \n");
	}
}

bool TCP_config() {

	clear_tcp_resp_buff();

    if(EC200_Transmit("AT+CPIN?\r\n",tcp_buff,2000))
    {
    // Check if the response contains the desired string
       if (strstr(tcp_buff, "+CPIN: READY") != NULL) {
           printc(MinDelay, "SIM Ready\n");
       } else {
           printc(MinDelay, "Not Ready\n");
           return false;
       }
    }

    if(EC200_Transmit("AT+CREG?\r\n",tcp_buff,2000))
   {
    	// Check if the response contains the desired string
       if (strstr(tcp_buff, "+CREG: 0,1") != NULL) {
           printc(MinDelay, "Registered at Home\n");
       } else {
           printc(MinDelay, "Not Registered\n");
           return false;
       }
   }

    if(EC200_Transmit("AT+QIDEACT=1\r\n",tcp_buff,2000))
    {
    // Check if the response contains "OK" or "+QIURC: \"closed\",0"
    if (strstr(tcp_buff, "OK") != NULL || strstr(tcp_buff, "+QIURC: \"closed\",0") != NULL) {
        printc(MinDelay, "Internet closed successfully\n");

    } else {
        printc(MinDelay, "Failed to close the Internet\n");
        return false;
    }
    }
    if(EC200_Transmit("AT+QICSGP=1,1,\"TAISYSNET\",\"\",\"\",0\r\n",tcp_buff,2000))
    {
        // Check if the response contains "OK"
        if (strstr(tcp_buff, "OK") != NULL) {
            printc(MinDelay, "APN SET SUCESSFULLY\n");

        } else {
            printc(MinDelay, "Failed to SET APN\n");
            return false;
        }

    }
    if(EC200_Transmit("AT+QIACT=1\r\n",tcp_buff,2000))
    {
    // Check if the response contains "OK"
           if (strstr(tcp_buff, "OK") != NULL) {
               printc(MinDelay, "DATA ENABLED\n");
               return true;
           } else {
               printc(MinDelay, "Failed to DATA ENABLE\n");
               return false;
           }
    }
    return true;
}

bool TCP_Open() {
	getTcpServerIP(SERVER_IP);
	SERVER_PORT=getTcpPort();
    char command[100];
    sprintf(command, "AT+QIOPEN=1,0,\"TCP\",\"%s\",%d,0,1\r\n",SERVER_IP, SERVER_PORT);
    if(EC200_Transmit(command,tcp_buff,5000))
    {

    	if(wait_for_response(tcp_buff,30000))
    	{

    	  // Check if the response contains "OK" and "+QIOPEN: 0,0"
    	        if ((strstr(tcp_buff, "+QIOPEN: 0,0") != NULL)||(strstr(tcp_buff, "+QIOPEN: 0,563") != NULL) ) {
    	            printc(MinDelay, "TCP connection opened successfully\n");
    	        } else {
    	            printc(MinDelay, "Failed to open TCP connection\n");
    	            return false;
    	        }
    	}
    	else
    	{
    		printc(MinDelay, "TCP timeout\n");
    		return false;
    	}

    }
return true;
}

bool TCP_Send(const char *data) {
	uint8_t command[100];
	if(TCP_Open()){
		sprintf(command,"AT+QISEND=0,%d\r\n",strlen(data));
		EC200_Transmit(command,tcp_buff,2000);
		if(EC200_Transmit(data,tcp_buff,2000))
		{
		// Check if the response contains "SEND OK"
			if (strstr(tcp_buff, "SEND OK") != NULL) {
				printc(MinDelay, "Data sent successfully\n");
				return true;
			} else {
				printc(MinDelay, "Failed to send data\n");
				return false;
			}
		}
		else{
			printc(MinDelay, "Don't get response for AT+QISEND=0 \n");
			return false;
		}
	}
	else{
		printc(MinDelay, "Open Failed\n");
		return false;

	}
}
