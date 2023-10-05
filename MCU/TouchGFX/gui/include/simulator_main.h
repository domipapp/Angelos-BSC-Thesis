
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif



/* USER CODE BEGIN Private defines */
#define IP_ADDRESS "192.168.1.249"	// Server IP Address
#define PORT "9000"					// Server Port number
// AT command for connecting to server
#define WIFI_SSID "Telekom-072404"	// Local WIFI SSID
#define WIFI_PASS "atc7habf4xt6"// Local WIFI password
#define DATA_FREQUENCY "5000"		// 5000ms data frequency

extern char server_ip[50];
extern char server_port[50];
extern char wifi_ssid[50];
extern char wifi_pass[50];
extern uint16_t data_frequency;

extern osEventFlagsId_t eventConfigurationsLoadedHandle;
// Event flag
#define EVENT_FLAG1 0x00000001U
#define EVENT_FLAG_ESP_ERROR 0x00000001U
#define EVENT_FLAG_ESP_RESPONSE_TIMEOUT 0x00000010U
// Wait for event flags for 5000 ticks
#define EVENT_FLAG_WAIT 5000


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
