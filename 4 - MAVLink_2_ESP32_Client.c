// TCP Client to send a MAVLink 2.0 Heartbeat message to another ESP32 with an open socket and known IP

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
#include "my_data.h"

static const char *TAG = "TCP CLIENT";

// Target TCP Server
#define TARGET_IP "192.168.1.152"
#define TARGET_PORT 14550

// MAVLink 2.0 Heartbeat message
const uint8_t HEX_DATA_TO_SEND[] = {0xFD, 0x09, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x04, 0x03,
                                    0x98, 0x2F};
const size_t HEX_DATA_LEN = sizeof(HEX_DATA_TO_SEND);

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

void raw_tcp_client_task(void *pvParameters)
{
    struct sockaddr_in dest_addr;                     // Structure to hold the server's address
    memset(&dest_addr, 0, sizeof(dest_addr));         // Clear the structure
    dest_addr.sin_addr.s_addr = inet_addr(TARGET_IP); // Set target IP
    dest_addr.sin_family = AF_INET;                   // Set address family to IPv4
    dest_addr.sin_port = htons(TARGET_PORT);          // Set target port (host to network short)

    int sock = -1; // Socket descriptor
    int err = 0;   // Error code holder

    // 1. Create a socket
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (sock < 0)
    {
        ESP_LOGE(TAG, "Failed to create socket: %d", errno);
        vTaskDelay(pdMS_TO_TICKS(5000)); // Wait before retrying
    }

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

    // 3. Send MAVLink message
    int bytes_sent = send(sock, HEX_DATA_TO_SEND, HEX_DATA_LEN, 0);
    if (bytes_sent < 0)
    {
        ESP_LOGE(TAG, "Error sending data: %d (%s)", errno, strerror(errno));
    }
    else
    {
        ESP_LOGI(TAG, "Sent %d bytes of data.", bytes_sent);
    }

    // 4. Close the socket
    shutdown(sock, 0); // Shut down the sending side
    close(sock);       // Close the socket
    ESP_LOGI(TAG, "Socket closed.");

    vTaskDelete(NULL);
}

void app_main(void)
{
    nvs_flash_init();
    wifi_connection();

    vTaskDelay(2000 / portTICK_PERIOD_MS);
    printf("WIFI was initiated ...........\n\n");

    // Create the TCP client task
    xTaskCreate(&raw_tcp_client_task, "tcp_client_task", 4096, NULL, 5, NULL);
}
