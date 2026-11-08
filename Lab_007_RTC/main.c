#include "stm32l476xx.h"
#include "SysClock.h"
#include "LED.h"
#include "RTC.h"
#include "UART.h"

#include <string.h>
#include <stdio.h>

#define __RTC_CONVERT_BIN2BCD(__VALUE__) (uint8_t)((((__VALUE__) / 10) << 4) | ((__VALUE__) % 10))
#define __RTC_CONVERT_BCD2BIN(__VALUE__) (uint8_t)(((uint8_t)((__VALUE__) & (uint8_t)0xF0) >> (uint8_t)0x4) * 10 + ((__VALUE__) & (uint8_t)0x0F))

// Global string buffers declared before function usage
char strTime[12] = {0};
char strDate[12] = {0};
char str[30];

char rx_buffer[50];
char reply[64];
uint8_t rx_index = 0;

uint32_t start_seconds_total = 0;

uint32_t Get_total_seconds(void) {
    uint32_t hours   = __RTC_CONVERT_BCD2BIN(RTC_TIME_GetHour());
    uint32_t minutes = __RTC_CONVERT_BCD2BIN(RTC_TIME_GetMinute());
    uint32_t seconds = __RTC_CONVERT_BCD2BIN(RTC_TIME_GetSecond());
    return (hours * 3600) + (minutes * 60) + seconds;
}

void Process_command(char *cmd) {
    if (strcmp(cmd, "elapsed") == 0) {
        uint32_t current_total = Get_total_seconds();
        uint32_t elapsed;

        // Handle midnight rollover (86400 seconds in a day)
        if (current_total >= start_seconds_total) {
            elapsed = current_total - start_seconds_total;
        } else {
            elapsed = (86400 + current_total) - start_seconds_total;
        }

        sprintf(reply, "\r\nElapsed: %u seconds (%u mins)\r\n", elapsed, elapsed / 60);
        USART_Write(USART2, (uint8_t *)reply, strlen(reply));
    } 
    else if (strcmp(cmd, "time") == 0) {
        Get_RTC_Calendar(strTime, strDate);
        sprintf(reply, "\r\nCurrent Time: %s\r\n", strTime);
        USART_Write(USART2, (uint8_t *)reply, strlen(reply));
    } 
    else {
        sprintf(reply, "\r\nUnknown command: %s. Commands: 'elapsed', 'time'\r\n", cmd);
        USART_Write(USART2, (uint8_t *)reply, strlen(reply));
    }
}

void Check_UART_Input(void) {
    // Check if a character has arrived without blocking main loop
    if (USART2->ISR & USART_ISR_RXNE) {
        char ch = (char)(USART2->RDR & 0xFF);

        // Echo character back to TeraTerm
        USART_Write(USART2, (uint8_t *)&ch, 1);

        // Process line ending
        if (ch == '\r' || ch == '\n') {
            rx_buffer[rx_index] = '\0'; // Null-terminate string
            if (rx_index > 0) {
                Process_command(rx_buffer);
                rx_index = 0; // Reset buffer index
            }
        } else if (rx_index < sizeof(rx_buffer) - 1) {
            rx_buffer[rx_index++] = ch;
        }
    }
}

int main(void) {
    System_Clock_Init(); // Switch System Clock = 80 MHz
    LED_Init();

    RTC_Init();
    UART2_Init();

    // Capture boot time for elapsed time calculations
    start_seconds_total = Get_total_seconds();

    static uint8_t last_second = 0xFF;

    while(1) {
        // Continuously poll UART for incoming keypresses from TeraTerm
        Check_UART_Input();

        // Check current RTC second
        uint8_t current_second = __RTC_CONVERT_BCD2BIN(RTC_TIME_GetSecond());

        if (current_second != last_second) {
            last_second = current_second;

            Get_RTC_Calendar(strTime, strDate);
            sprintf(str, "%s, %s\r\n", strTime, strDate); 
            USART_Write(USART2, (uint8_t *)str, strlen(str));
            LED_Toggle();
        }
    }
}

