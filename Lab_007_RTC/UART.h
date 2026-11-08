#ifndef __STM32L476G_UART_H
#define __STM32L476G_UART_H

#include "stm32l476xx.h"
#include <stdint.h>
#include <stddef.h>

#define BufferSize 32

/* Initialization Functions */
void UART2_Init(void);
void UART2_GPIO_Init(void);
void USART_Init(USART_TypeDef *USARTx);

/* Data Transmit & Receive Functions */
void USART_Write(USART_TypeDef *USARTx, uint8_t *buffer, uint32_t nBytes);
uint8_t USART_Read(USART_TypeDef *USARTx);

/* Utility & Timing Functions */
void USART_Delay(uint32_t us);

/* Interrupt Handlers (if interrupt-driven UART is used) */
void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void USART_IRQHandler(USART_TypeDef *USARTx, uint8_t *buffer, uint32_t *pRx_counter);

#endif /* __STM32L476G_UART_H */