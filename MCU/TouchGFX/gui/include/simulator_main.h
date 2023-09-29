
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif



/* USER CODE BEGIN Private defines */
#define IP_ADDRESS "192.168.1.249"	// Server IP Address
#define PORT "9000"					// Server Port number
// AT command for connecting to server
#define WIFI_SSID "Telekom-2D6325"	// Local WIFI SSID
#define WIFI_PASS "4njteenm6s7cx4cb"// Local WIFI password
#define DATA_FREQUENCY "5000"		// 5000ms data frequency

char server_connect_command [50];

char wifi_ssid[50];

char wifi_pass[50];

char wifi_connect_command [116];

uint16_t data_frequency = 0;


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
