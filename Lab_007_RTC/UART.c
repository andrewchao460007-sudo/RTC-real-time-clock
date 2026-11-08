#include "UART.h"
void UART2_GPIO_Init(void) {
    // 1. Enable GPIOA peripheral clock
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

    // 2. Set PA2 (TX) and PA3 (RX) to Alternate Function mode (10)
    GPIOA->MODER &= ~(GPIO_MODER_MODE2 | GPIO_MODER_MODE3);
    GPIOA->MODER |= (GPIO_MODER_MODE2_1 | GPIO_MODER_MODE3_1);

    // 3. Configure AF7 (USART2) for PA2 and PA3 (Shift 8 for Pin 2, 12 for Pin 3)
    GPIOA->AFR[0] &= ~(GPIO_AFRL_AFSEL2 | GPIO_AFRL_AFSEL3);
    GPIOA->AFR[0] |= (7U << (2 * 4)) | (7U << (3 * 4));

    // 4. Configure Output Speed and Pull-up resistors
    GPIOA->OSPEEDR |= (GPIO_OSPEEDR_OSPEED2 | GPIO_OSPEEDR_OSPEED3);
    GPIOA->PUPDR   &= ~(GPIO_PUPDR_PUPD2 | GPIO_PUPDR_PUPD3);
    GPIOA->PUPDR   |= (GPIO_PUPDR_PUPD2_0 | GPIO_PUPDR_PUPD3_0);
}

void UART2_Init(void) {
    // Enable GPIO configuration for UART2
    UART2_GPIO_Init();

    // Enable USART2 peripheral clock
    RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;

    // Disable USART before configuring
    USART2->CR1 &= ~USART_CR1_UE;

    // Configure Baud Rate (80 MHz sysclk / 115200 baud = 694)
    USART2->BRR = 694;

    // Enable Transmit (TE) and Receive (RE)
    USART2->CR1 |= (USART_CR1_TE | USART_CR1_RE);

    // Enable USART peripheral
    USART2->CR1 |= USART_CR1_UE;
}

void USART_Write(USART_TypeDef *USARTx, uint8_t *buffer, uint32_t nBytes) {
    for (uint32_t i = 0; i < nBytes; i++) {
        // Wait until transmit data register is empty
        while (!(USARTx->ISR & USART_ISR_TXE));
        // Write data byte to transmit data register
        USARTx->TDR = buffer[i];
    }
    // Wait until transmission complete
    while (!(USARTx->ISR & USART_ISR_TC));
}

uint8_t USART_Read(USART_TypeDef *USARTx) {
    // Wait until receive data register is full
    while (!(USARTx->ISR & USART_ISR_RXNE));
    // Read and return character from receive data register
    return (uint8_t)(USARTx->RDR & 0xFF);
}