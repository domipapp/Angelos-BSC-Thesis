
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

char server_ip[50];
char server_port[50];
char wifi_ssid[50];
char wifi_pass[50];
uint16_t data_frequency;


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
