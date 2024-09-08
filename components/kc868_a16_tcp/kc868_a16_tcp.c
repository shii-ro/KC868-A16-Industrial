
#include "kc868_a16_tcp.h"

#define TAG "TCP_SERVER"

// Static variable to store the queue handle
static QueueHandle_t input_state_queue = NULL;

// Função da tarefa para o servidor TCP
static void tcp_server_task(void *pvParameters)
{
    uint32_t port = (uint32_t)pvParameters;

	struct sockaddr_in clientAddress;
	struct sockaddr_in serverAddress;
	
	// Create a socket that we will listen upon.
	int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock < 0) {
		ESP_LOGE(TAG, "socket: %d %s", sock, strerror(errno));
		goto END;
	}

	// Bind our server socket to a port.
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddress.sin_port = htons(port);

	int rc  = bind(sock, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
	if (rc < 0) {
		ESP_LOGE(TAG, "bind: %d %s", rc, strerror(errno));
		goto END;
	}

	// Flag the socket as listening for new connections.
	rc = listen(sock, 5);
	if (rc < 0) {
		ESP_LOGE(TAG, "listen: %d %s", rc, strerror(errno));
		goto END;
	}

	while (1) 
	{
        ESP_LOGD(TAG, "Waiting for new client connection");
        socklen_t clientAddressLength = sizeof(clientAddress);
        int clientSock = accept(sock, (struct sockaddr *)&clientAddress, &clientAddressLength);
        if (clientSock < 0) {
            ESP_LOGE(TAG, "accept: %d %s", clientSock, strerror(errno));
            goto END;
        }

        while (1) {
            uint16_t input_state;
            
            // Wait to receive an input state from the queue (blocks until a message is received)
            if (xQueueReceive(input_state_queue, &input_state, portMAX_DELAY) == pdPASS) {
                char text[80];
                sprintf(text, "Input 0 state: %02d\nInput 1 state: %02d\n", (input_state >> 8), (input_state & 0xFF));

                // Send the input state to the client
                if (send(clientSock, text, strlen(text), 0) < 0) {
                    ESP_LOGE(TAG, "send failed");
                    break;  // Exit the loop on send failure
                }
            }
        }

        closesocket(clientSock);
	}

	END:
	vTaskDelete(NULL);
}

void kc868_a16_tcp_server_send_message(uint16_t data)
{
	if (xQueueSend(input_state_queue, &data, portMAX_DELAY) != pdPASS) 
	{
        ESP_LOGE(TAG, "Failed to send input state to queue");
    }
}


void kc868_a16_tcp_server_start_task(uint32_t port)
{
	// Create a queue to hold input states (16-bit values)
    input_state_queue = xQueueCreate(10, sizeof(uint16_t));
    // Criar uma tarefa para o servidor TCP, rodando em segundo plano
    xTaskCreate(tcp_server_task, "tcp_server_task", 4096, (void *)8001, 5, NULL);
}
