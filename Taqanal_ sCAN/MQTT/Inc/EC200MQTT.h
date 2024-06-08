/*
 * EC200MQTT.h
 *
 *  Created on: Mar 15, 2024
 *      Author: 91944
 */

#ifndef INC_EC200MQTT_H_
#define INC_EC200MQTT_H_
#include "EC200Utils.h"
/******************************************* Defination ********************************************/
#define MQTT_MAX_RESONSETIME 10000
#define CLIENT_IDX 0
#define STRINGIFY_(x) #x
#define STRINGIFY(x) STRINGIFY_(x)
#define HOST_NAME "forecite.in"
#define PORT 8883
#define CLIENT_ID "867409070312751"
#define USERNAME  "867409070312751"
#define PASSWORD "123"
#define TOPIC "/device/867409070312751/MQTTJSON/EC_TEST"

void MQTT_init(void);

void MQTT_Config(void);
bool MQTT_Open(void);
bool MQTT_Connect(void);
void MQTT_Subscribe(void);
void MQTT_Close_ID(void);
bool MQTT_Publish(uint8_t *data);
void MQTT_Disconnect(void);


#endif /* INC_EC200MQTT_H_ */
