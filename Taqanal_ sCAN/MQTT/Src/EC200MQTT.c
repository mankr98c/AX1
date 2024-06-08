#include "EC200MQTT.h"
/*-------------------------TEST_DEFINE--------------------------*/
uint8_t data1[100] = "Hello, MQTT!\r\n";
void MQTT_init(void){
	 MQTT_Config();
	 MQTT_Publish(data1);
}

void MQTT_Config(void) {
    // MQTT Configuration
	uint32_t resbuff[100]={};
	if (EC200_Transmit("AT\r\n", resbuff,EC200MINRESONSETIME)) {
	    printc(MinDelay, "Response for AT is %s\n", resbuff);
	}
	memset(resbuff,0,100);
	if (EC200_Transmit("\r\n", resbuff,EC200MINRESONSETIME)) {
	    printc(MinDelay, "Response for new line is %s\n", resbuff);
	}
	memset(resbuff,0,100);
	if (EC200_Transmit("AT+QMTCFG=\"version\"," STRINGIFY(CLIENT_IDX) ",3\r\n", resbuff,EC200MINRESONSETIME)) {
	    printc(MinDelay, "Response for QMTCFG version is %s\n", resbuff);
	}
	memset(resbuff,0,100);
	if (EC200_Transmit("AT\r\n", resbuff,EC200MINRESONSETIME)) {
	    printc(MinDelay, "Response for AT is %s\n", resbuff);
	}
	memset(resbuff,0,100);
	if (EC200_Transmit("AT+QMTCFG=\"send/mode\"," STRINGIFY(CLIENT_IDX) ",0\r\n", resbuff,EC200MINRESONSETIME)) {
	    printc(MinDelay, "Response for QMTCFG send/mode is %s\n", resbuff);
	}
	memset(resbuff,0,100);
	if (EC200_Transmit("AT+QMTCFG=\"SSL\"," STRINGIFY(CLIENT_IDX) ",1,0\r\n", resbuff,EC200MINRESONSETIME)) {
	    printc(MinDelay, "Response for QMTCFG SSL is %s\n", resbuff);
	}
	memset(resbuff,0,100);
	if (EC200_Transmit("AT+QSSLCFG=\"cacert\"," STRINGIFY(CLIENT_IDX) ",\"UFS:cacert.pem\"\r\n", resbuff,EC200MINRESONSETIME)) {
	    printc(MinDelay, "Response for QSSLCFG cacert is %s\n", resbuff);
	}
	memset(resbuff,0,100);
	if (EC200_Transmit("AT+QSSLCFG=\"clientcert\"," STRINGIFY(CLIENT_IDX) ",\"UFS:client.pem\"\r\n", resbuff,EC200MINRESONSETIME)) {
	    printc(MinDelay, "Response for QSSLCFG clientcert is %s\n", resbuff);
	}
	memset(resbuff,0,100);
	if (EC200_Transmit("AT+QSSLCFG=\"clientkey\"," STRINGIFY(CLIENT_IDX) ",\"UFS:user_key.pem\"\r\n", resbuff,EC200MINRESONSETIME)) {
	    printc(MinDelay, "Response for QSSLCFG clientkey is %s\n", resbuff);
	}
	memset(resbuff,0,100);
	if (EC200_Transmit("AT+QSSLCFG=\"seclevel\"," STRINGIFY(CLIENT_IDX) ",2\r\n", resbuff,EC200MINRESONSETIME)) {
	    printc(MinDelay, "Response for QSSLCFG seclevel is %s\n", resbuff);
	}
	memset(resbuff,0,100);
	if (EC200_Transmit("AT+QSSLCFG=\"sslversion\"," STRINGIFY(CLIENT_IDX) ",4\r\n", resbuff,EC200MINRESONSETIME)) {
	    printc(MinDelay, "Response for QSSLCFG sslversion is %s\n", resbuff);
	}
	memset(resbuff,0,100);
	if (EC200_Transmit("AT+QSSLCFG=\"ciphersuite\"," STRINGIFY(CLIENT_IDX) ",0xFFFF\r\n", resbuff,EC200MINRESONSETIME)) {
	    printc(MinDelay, "Response for QSSLCFG ciphersuite is %s\n", resbuff);
	}
	memset(resbuff,0,100);
	if (EC200_Transmit("AT+QSSLCFG=\"ignorelocaltime\"," STRINGIFY(CLIENT_IDX) ",1\r\n", resbuff,EC200MINRESONSETIME)) {
	    printc(MinDelay, "Response for QSSLCFG ignorelocaltime is %s\n", resbuff);
	}
	free(resbuff);

}
//
bool MQTT_Open(void) {
    char command[100]={};
    char rx_buffer[60]={};
    memset(rx_buffer, 0, sizeof(rx_buffer));
    int count=0;
    open:
    sprintf(command, "AT+QMTOPEN=" STRINGIFY(CLIENT_IDX) ",\"" HOST_NAME "\"," STRINGIFY(PORT) "\r\n");
    if(EC200_Transmit(command, rx_buffer,MQTT_MAX_RESONSETIME)) {
           printc(MinDelay, "Response for AT+QMTOPEN is %s\n", rx_buffer);
           if(wait_for_response(rx_buffer,MQTT_MAX_RESONSETIME)){
        	   printc(MinDelay,"%s\n",rx_buffer,60);
        	          int connrsp=-1;
        	          if ((strstr(rx_buffer, "+QMTOPEN: 0,0") != NULL) ||(strstr(rx_buffer, "+QMTOPEN: 0,2") != NULL)) { // Check each line for the response
        	                   return true;
        	           }
        	          else{
        	        	 if(count<3){
        	        		 count++;
        	        		 goto open;
        	        	 }
        	          }
        	      }

           }
    return false;

       }


bool MQTT_Connect(void) {
    // MQTT Connect
	char rx_buffer[100]={};
    memset(rx_buffer,0,sizeof(rx_buffer));
    if(EC200_Transmit("AT+QMTCONN="STRINGIFY(CLIENT_IDX)",\""CLIENT_ID"\",\""USERNAME"\",\""PASSWORD"\"\r\n",rx_buffer,MQTT_MAX_RESONSETIME)){
    		printc(MinDelay, "Response for AT+QMTCONN is %s\n", rx_buffer);
    		memset(rx_buffer,0,sizeof(rx_buffer));
    		if(wait_for_response(rx_buffer,MQTT_MAX_RESONSETIME)){
    			if(strlen(rx_buffer)!=0){
    				 printc(MinDelay, "first response Response for AT+QMTCONN is %s\n", rx_buffer);
    				goto conn;
    			}

    		 }
    		if(wait_for_response(rx_buffer,MQTT_MAX_RESONSETIME)){
    		   printc(MinDelay, "Second response Response for AT+QMTCONN is %s\n", rx_buffer);
			}
			conn:
			if(strstr(rx_buffer,"+QMTCONN: 0,0,0")!=NULL||strstr(rx_buffer,"+QMTCONN: 0,1,0")!=NULL){
									 return true;
			 }
    }
    return false;
}
//
bool MQTT_Publish(uint8_t *message) {

//	we have to modify response from ec200
	char rx_buffer[200]={};
	char command[100]={};
	int len=strlen(message);
	char data[len];
	memcpy(data,message,len);
	if(EC200_Transmit("AT+QMTCONN?\r\n",rx_buffer,MQTT_MAX_RESONSETIME)){
		  if(strstr(rx_buffer,"+QMTCONN: 0,3")==NULL){
			   printc(MinDelay, "AT+QMTCONN? %s,%d \n", rx_buffer,strlen(rx_buffer));
			   if(MQTT_Open()){
			   		printc(1000,(char*)"connection Open Done\n");
			   		if(MQTT_Connect()){
			   			printc(MinDelay,"Connection done \n");
			   		}
			   		else{
			   			printc(MinDelay,"Error in Connecting \n");
			   			return false;
			   		}
			   	}
			   	else{
			   		printc(MinDelay,"Error in opening \n");
			   		return false;
			   	}
		   }
		   else{
			   printc(MinDelay,"all Ready Connected %s\n",rx_buffer);
		   }
		   sprintf(command, "AT+QMTPUBEX="STRINGIFY(CLIENT_IDX)",1,1,0,\"%s\",%d\r\n", TOPIC, len);
		   printc(1000,"command is %s %d\n",command,strlen(command));
			if(EC200_Transmit(command,rx_buffer,MQTT_MAX_RESONSETIME)){
			   printc(MinDelay, "Response for AT+QMTPUBEX is %s\n", rx_buffer);
		   }
			// Send the MQTT data
			memset(rx_buffer,0,sizeof(rx_buffer));
			if( EC200_Transmit(data,rx_buffer,MQTT_MAX_RESONSETIME)){
			   print(MinDelay,rx_buffer,sizeof(rx_buffer)-1);
			   memset(rx_buffer,0,sizeof(rx_buffer));
			   if(wait_for_response(rx_buffer,MQTT_MAX_RESONSETIME)){
				   print(MinDelay,"response of publish is %s \n",sizeof(rx_buffer)-1);
			   }
			 }

	}
	return true;


}

void MQTT_Subscribe(void) {
    // MQTT Subscribe
	EC200_Transmit("AT+QMTSUB="STRINGIFY(CLIENT_IDX)",1,\""TOPIC"\",1\r\n",NULL,0);
}

void MQTT_Disconnect(void) {
    // MQTT Disconnect
	EC200_Transmit("AT+QMTDISC="STRINGIFY(CLIENT_IDX)"\r\n",NULL,0);
}

void MQTT_Close_ID(void) {
    // MQTT Closed Current connection
	EC200_Transmit("AT+QMTCLOSE="STRINGIFY(CLIENT_IDX)"\r\n",NULL,0);
}

/*-------------------------------------------------------------------------------*/




