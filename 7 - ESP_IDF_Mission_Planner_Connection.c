// UDP Client to send a MAVLink 2.0 Heartbeat and parameters message
// from ESP32 to Mission Planner PC with known IP and PORT

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_http_client.h"

#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "esp_event.h"
#include "esp_system.h"

#include "my_data.h"

static const char *TAG = "UDP CLIENT";

// Target UDP Server
#define TARGET_IP "192.168.1.37"
#define TARGET_PORT 14550

// MAVLink 2.0 Heartbeat message
uint8_t HEX_DATA_TO_SEND[] = {  0xFD, // MAVLink 2.0 identifier
                                0x09, // Length of payload = 9 bytes
                                0x00, // Incompatibility flags
                                0x00, // Compatibility flags
                                0x00, // Sequence number
                                0x01, // System ID
                                0x01, // Component ID
                                0x00, // HEARTBEAT
                                0x00, // HEARTBEAT
                                0x00, // HEARTBEAT
                                0x00, // Custom Mode = 0
                                0x00, // Custom Mode = 0
                                0x00, // Custom Mode = 0
                                0x00, // Custom Mode = 0
                                0x02, // MAV_TYPE_QUADROTOR
                                0x00, // MAV_AUTOPILOT_GENERIC
                                0x00, // Not armed
                                0x04, // User-defined
                                0x03, // MAVLink version = 3 (for MAVLink 2.0)
                                0x98, // CRC-16
                                0x2F  // CRC-16
                                };
size_t HEX_DATA_LEN = sizeof(HEX_DATA_TO_SEND);
// MAVLink 2.0 Parameters definition
uint8_t parm_msg[] = {  0xFD, // MAVLink 2.0
                        0x19, // Length of payload = 25 bytes
                        0x00, // Incompatibility flags
                        0x00, // Compatibility flags
                        0x00, // Sequence
                        0x01, // System ID
                        0x01, // Component ID
                        0x16, // PARAM_VALUE = 22
                        0x00, // PARAM_VALUE = 22
                        0x00, // PARAM_VALUE = 22
                        0x00, // param_value = 1.0
                        0x00, // param_value = 1.0
                        0x80, // param_value = 1.0
                        0x3f, // param_value = 1.0
                        0x00, // param_count = 0
                        0x00, // param_count = 0
                        0x00, // param_index = 0
                        0x00, // param_index = 0
                        0x50, // param_id = "PARAM_1"	
                        0x41, // param_id = "PARAM_1"
                        0x52, // param_id = "PARAM_1"
                        0x41, // param_id = "PARAM_1"
                        0x4D, // param_id = "PARAM_1"
                        0x5F, // param_id = "PARAM_1"
                        0x31, // param_id = "PARAM_1"
                        0x00, // param_id = "PARAM_1"
                        0x00, // padding for 16 chars
                        0x00, // padding for 16 chars
                        0x00, // padding for 16 chars
                        0x00, // padding for 16 chars
                        0x00, // padding for 16 chars
                        0x00, // padding for 16 chars
                        0x00, // padding for 16 chars
                        0x00, // padding for 16 chars
                        0x09, // MAV_PARAM_TYPE_REAL32
                        0x78, // CRC-16
                        0xB1  // CRC-16  
                        };
size_t parm_msg_len = sizeof(parm_msg);

static void wifi_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    switch (event_id)
    {
    case WIFI_EVENT_STA_START:
        printf("WiFi connecting ... \n");
        break;
    case WIFI_EVENT_STA_CONNECTED:
        printf("WiFi connected ... \n");
        break;
    case WIFI_EVENT_STA_DISCONNECTED:
        printf("WiFi lost connection ... \n");
        break;
    case IP_EVENT_STA_GOT_IP:
        printf("WiFi got IP ... \n\n");
        break;
    default:
        break;
    }
}

void wifi_connection()
{
    // 1 - Wi-Fi/LwIP Init Phase
    esp_netif_init();                    // TCP/IP initiation 					s1.1
    esp_event_loop_create_default();     // event loop 			                s1.2
    esp_netif_create_default_wifi_sta(); // WiFi station 	                    s1.3
    wifi_init_config_t wifi_initiation = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_initiation); // 					                    s1.4
    // 2 - Wi-Fi Configuration Phase
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, wifi_event_handler, NULL);
    wifi_config_t wifi_configuration = {
        .sta = {
            .ssid = SSID,
            .password = PASS}};
    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_configuration);
    // 3 - Wi-Fi Start Phase
    esp_wifi_start();
    // 4- Wi-Fi Connect Phase
    esp_wifi_connect();
}

void udp_client_task(void *pvParameters)
{
    int bytes_sent = 0;

    struct sockaddr_in dest_addr;                     // Structure to hold the server's address
    memset(&dest_addr, 0, sizeof(dest_addr));         // Clear the structure
    dest_addr.sin_addr.s_addr = inet_addr(TARGET_IP); // Set target IP
    dest_addr.sin_family = AF_INET;                   // Set address family to IPv4
    dest_addr.sin_port = htons(TARGET_PORT);          // Set target port (host to network short)

    int sock = -1; // Socket descriptor
    int err = 0;   // Error code holder

    // Receive information
    uint8_t rx_buffer[1500];
    struct sockaddr_in source_addr;
    socklen_t socklen = sizeof(source_addr);

    // 1. Create a socket
    // sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (sock < 0)
    {
        ESP_LOGE(TAG, "Failed to create socket: %d", errno);
        vTaskDelay(pdMS_TO_TICKS(5000)); // Wait before retrying
    }

    // 1.1 Increase send buffer size
    int send_buf_size = 8192;
    setsockopt(sock, SOL_SOCKET, SO_SNDBUF, &send_buf_size, sizeof(send_buf_size));

    // 2. Connect to the server
    ESP_LOGI(TAG, "Attempting to connect to %s:%d...", TARGET_IP, TARGET_PORT);
    err = connect(sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if (err != 0)
    {
        ESP_LOGE(TAG, "Socket connect failed: %d (%s)", errno, strerror(errno));
        close(sock); // Close the socket if connection fails
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
    ESP_LOGI(TAG, "Successfully connected to %s:%d", TARGET_IP, TARGET_PORT);

    for (size_t i = 0; i < 20; i++)
    {
        // 3. Update MAVLink message
        // Update counter
        if (i > 0)
            HEX_DATA_TO_SEND[4]++;

        // Calculate CRC
        uint16_t crc = 0xFFFF;
        uint8_t data;
        uint8_t crc_extra = 50; // example for HEARTBEAT

        // A. Accumulate header + payload (excluding start byte 0xFD)
        for (uint32_t ii = 1; ii < HEX_DATA_LEN - 2; ii++)
        {
            data = HEX_DATA_TO_SEND[ii];
            data ^= (uint8_t)(crc & 0xFF); // XOR with lower byte of CRC
            data ^= (data << 4);           // Shift data left by 4
            crc = (crc >> 8) ^ ((uint16_t)data << 8) ^ ((uint16_t)data << 3) ^ ((uint16_t)data >> 4);
        }

        // B. Accumulate CRC_EXTRA
        data = crc_extra;
        data ^= (uint8_t)(crc & 0xFF);
        data ^= (data << 4);
        crc = (crc >> 8) ^ ((uint16_t)data << 8) ^ ((uint16_t)data << 3) ^ ((uint16_t)data >> 4);

        HEX_DATA_TO_SEND[HEX_DATA_LEN - 2] = crc & 0xFF;
        HEX_DATA_TO_SEND[HEX_DATA_LEN - 1] = (crc >> 8) & 0xFF;

        // 4.1 Send Heartbeat message
        bytes_sent = send(sock, HEX_DATA_TO_SEND, HEX_DATA_LEN, 0);
        if (bytes_sent < 0)
        {
            ESP_LOGE(TAG, "Error sending HEARTBEAT data: %d (%s)", errno, strerror(errno));
        }
        else
        {
            ESP_LOGI(TAG, "Sent %d bytes of HEARTBEAT data.", bytes_sent);
        }

        // 4.2 Send PARAM message
        bytes_sent = send(sock, parm_msg, parm_msg_len, 0);
        if (bytes_sent < 0)
        {
            ESP_LOGE(TAG, "Error sending PARAM data: %d (%s)", errno, strerror(errno));
        }
        else
        {
            ESP_LOGI(TAG, "Sent %d bytes of PARAM data.", bytes_sent);
        }

        // Receive information
        int len = recvfrom(sock, rx_buffer, sizeof(rx_buffer) - 1, 0, (struct sockaddr *)&source_addr, &socklen);
        if (len < 0) {
            ESP_LOGE(TAG, "recvfrom failed: %d (%s)", errno, strerror(errno));
            break;
        } else {
            ESP_LOGI(TAG, "Received %d bytes from %s:%d", len,
                     inet_ntoa(source_addr.sin_addr), ntohs(source_addr.sin_port));
            // Print raw MAVLink message
            printf("Received from Mission Planner:\n");
            for (int i = 0; i < len; i++) {
                printf("%02X ", rx_buffer[i]);
            }
            printf("\n");
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    // 5. Close the socket
    shutdown(sock, 0); // Shut down the sending side
    close(sock);       // Close the socket
    ESP_LOGI(TAG, "Socket closed.");

    vTaskDelete(NULL);
}

void app_main(void)
{
    nvs_flash_init();
    wifi_connection();

    vTaskDelay(4000 / portTICK_PERIOD_MS);
    printf("WIFI was initiated ...........\n\n");

    // Create the UDP client task
    xTaskCreate(&udp_client_task, "udp_client_task", 4096, NULL, 5, NULL);
}
