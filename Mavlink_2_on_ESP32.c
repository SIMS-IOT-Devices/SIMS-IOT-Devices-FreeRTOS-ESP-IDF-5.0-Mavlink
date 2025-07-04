// TCP Server with WiFi connection communication via Socket by Mavlink 2.0

#include <stdio.h>
#include <string.h>

// FreeRTOS
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// System and WiFi
#include "my_data.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_check.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_system.h"
#include "esp_err.h"

// TCP Sockets
#include "lwip/inet.h"
#include "lwip/netdb.h"
#include "lwip/sockets.h"
#include "lwip/ip_addr.h"

#define PORT 14550

static const char *TAG = "TCP_SOCKET";

static void tcp_server_task(void *pvParameters)
{
    // Variables definitions
    char addr_str[128];
    char rx_buffer[128];
    // int tx_buffer_length = 17; // For Heartbeat message
    // char tx_buffer[tx_buffer_length];
    int keepAlive = 1;
    int keepIdle = 5;
    int keepInterval = 5;
    int keepCount = 3;
    struct sockaddr_storage dest_addr;

    // int counter = 0;

    // server address definition
    struct sockaddr_in *dest_addr_ip4 = (struct sockaddr_in *)&dest_addr;
    dest_addr_ip4->sin_addr.s_addr = htonl(INADDR_ANY);
    dest_addr_ip4->sin_family = AF_INET;
    dest_addr_ip4->sin_port = htons(PORT);

    // Open socket
    int listen_sock = socket(AF_INET, SOCK_STREAM, 0); // 0 for TCP Protocol
    int opt = 1;
    setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    ESP_LOGI(TAG, "Socket created");

    bind(listen_sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    ESP_LOGI(TAG, "Socket bound, port %d", PORT);

    // Listen to the socket
    listen(listen_sock, 1);

    while (1)
    {
        ESP_LOGI(TAG, "Socket listening");

        struct sockaddr_storage source_addr; // Large enough for both IPv4 or IPv6
        socklen_t addr_len = sizeof(source_addr);

        // Accept socket
        int sock = accept(listen_sock, (struct sockaddr *)&source_addr, &addr_len);
        if (sock < 0)
        {
            ESP_LOGE(TAG, "Unable to accept connection: errno %d", errno);
            break;
        }

        // Set tcp keepalive option
        setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &keepAlive, sizeof(int));
        setsockopt(sock, IPPROTO_TCP, TCP_KEEPIDLE, &keepIdle, sizeof(int));
        setsockopt(sock, IPPROTO_TCP, TCP_KEEPINTVL, &keepInterval, sizeof(int));
        setsockopt(sock, IPPROTO_TCP, TCP_KEEPCNT, &keepCount, sizeof(int));

        // Convert ip address to string
        if (source_addr.ss_family == PF_INET)
        {
            inet_ntoa_r(((struct sockaddr_in *)&source_addr)->sin_addr, addr_str, sizeof(addr_str) - 1);
        }

        ESP_LOGI(TAG, "Socket accepted ip address: %s", addr_str);

        // Mavlink TCP data reception
        ssize_t bytes_received = recv(sock, rx_buffer, sizeof(rx_buffer), 0);
        printf("\nESP32 RECEIVED: %d\n", bytes_received);
        printf("Mavlink message from PC to ESP32:\n");
        printf("\nHeader 10 bytes:\n");
        printf("0 - Start-of-frame:                     %02x\n", rx_buffer[0]);  // MAVLink 1.0 0xFE, MAVLink 2.0 0xFD
        printf("1 - Payload length:                     %02x\n", rx_buffer[1]);  // HEARTBEAT has a payload length of 9 bytes
        printf("2 - Incompatibility flags:              %02x\n", rx_buffer[2]);  // Features requiring negotiation
        printf("3 - Compatibility flags:                %02x\n", rx_buffer[3]);  // Features that can be ignored if unsupported
        printf("4 - Packet sequence (0 first message):  %02x\n", rx_buffer[4]);  // Packet sequence number incremented per message
        printf("5 - System ID       (We defined):       %02x\n", rx_buffer[5]);  // Who is sending
        printf("6 - Component ID    (We defined):       %02x\n", rx_buffer[6]);  // Which component is sending
        printf("7 - Message ID1     (0 for Heartbeat):  %02x\n", rx_buffer[7]);  // 0 for Heartbeat
        printf("8 - Message ID2     (0 for Heartbeat):  %02x\n", rx_buffer[8]);  // 0 for Heartbeat
        printf("9 - Message ID3     (0 for Heartbeat):  %02x\n", rx_buffer[9]);  // 0 for Heartbeat

        printf("\nPayload 9 bytes:\n");
        printf("0 - custom_mode:                        %02x\n", rx_buffer[10]); 
        printf("1 - custom_mode:                        %02x\n", rx_buffer[11]); 
        printf("2 - custom_mode:                        %02x\n", rx_buffer[12]); 
        printf("3 - custom_mode:                        %02x\n", rx_buffer[13]);  
        printf("4 - type:                               %02x\n", rx_buffer[14]); // 6 = MAV_TYPE_FIXED_WING
        printf("5 - autopilot:                          %02x\n", rx_buffer[15]); // 0 = MAV_AUTOPILOT_GENERIC
        printf("6 - base_mode:                          %02x\n", rx_buffer[16]); // 0 = No mode flags active
        printf("7 - system_status:                      %02x\n", rx_buffer[17]); // 4 = MAV_STATE_ACTIVE
        printf("8 - mavlink_version:                    %02x\n", rx_buffer[18]); // 3 = Required for MAVLink 2

        printf("\nCRC 2 bytes:\n");
        printf("1 - CRC:                                %02x\n", rx_buffer[19]); // LSB = 0x98
        printf("2 - CRC:                                %02x\n", rx_buffer[20]); // MSB = 0x2F

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    close(listen_sock);
    vTaskDelete(NULL);
}

static void wifi_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    switch (event_id)
    {
    case WIFI_EVENT_STA_START:
        printf("WiFi connecting WIFI_EVENT_STA_START ... \n");
        break;
    case WIFI_EVENT_STA_CONNECTED:
        printf("WiFi connected WIFI_EVENT_STA_CONNECTED ... \n");
        break;
    case WIFI_EVENT_STA_DISCONNECTED:
        printf("WiFi lost connection WIFI_EVENT_STA_DISCONNECTED ... \n");
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
    nvs_flash_init();
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t wifi_initiation = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_initiation);
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, wifi_event_handler, NULL);
    wifi_config_t wifi_configuration = {
        .sta = {
            .ssid = SSID,
            .password = PASS}};
    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_configuration);
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_start();
    esp_wifi_connect();
}

void app_main(void)
{
    wifi_connection();
    vTaskDelay(5000 / portTICK_PERIOD_MS);
    xTaskCreate(tcp_server_task, "tcp_server", 4096, (void *)AF_INET, 5, NULL);
}
