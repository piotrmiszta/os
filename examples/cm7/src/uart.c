#include "uart.h"
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
//if defined function is used as logger
//if not defined is used as communicate interface
#define CONFIG_LOGGER

#define PERIPH_CLK      64000000
#define USART_RXNE      (1U << 5)
#define USART_TXE       (1U << 7)
#define USART_TE        (1U << 2)
#define USART_RE        (1U << 3)
#define USART_UE        (1U << 0)
#define USART_FIFOEN    (1U << 29)
#define GPIOD_EN        (1U << 3)
#define AFR_MASK        (0xFU)
#define RCC_USART3      (1U << 18)

static inline u32 compute_baud_rate(u32 baud_rate);

void usart3_init(u32 baud_rate)
{
    /* pins PD8 - tx PD9 - rx */
    RCC->AHB4ENR |= GPIOD_EN;
    /* alternate mode */
    GPIOD->MODER &= ~(1U << 16);
    GPIOD->MODER |= (1U << 17);
    GPIOD->MODER &= ~(1U << 18);
    GPIOD->MODER |= (1U << 19);
    GPIOD->AFR[1] &= ~ AFR_MASK;
    GPIOD->AFR[1] |= (7U << 0);
    GPIOD->AFR[1] |= (7U << 4);

    /* enable clock to usart3 */
    RCC->APB1LENR |= RCC_USART3;
    USART3->BRR = compute_baud_rate(baud_rate);
    USART3->CR1 |= USART_TE;
    USART3->CR1 |= USART_RE;
    USART3->CR1 |= USART_UE;
}

char usart3_read_byte(void)
{
    while(!(USART3->ISR & USART_RXNE)) {}
    return (char)USART3->RDR;
}
void usart3_write_byte(int ch)
{
    while(!(USART3->ISR & USART_TXE)) {}
    USART3->TDR = (ch & 0xFF);
}
#ifndef CONFIG_LOGGER
void usart3_read(void* buff, u32 len)
{
    //TODO: implement this funciton
}
#endif
void usart3_write(void* buff, u32 len)
{
    char* buffer = (char*)buff;
    for(u32 i = 0; i < len; i++)
    {
        usart3_write_byte(buffer[i]);
    }
}

void usart3_write_msg(void* buff, u32 len)
{
#ifndef CONFIG_LOGGER
    usart3_write(&len, sizeof(len));
    usart3_write_byte('=');
#endif
    usart3_write(buff, len);
}

void _log(u8 type, const char* file, u32 line, const char* fmt, ...)
{
    char buffer[1000] = {0};
    int n;
    switch (type)
    {
    case LOGGER_DEBUG:
        n = sprintf(buffer, "[DEBUG] ");
        break;
    case LOGGER_ERROR:
        n = sprintf(buffer, "[ERROR] ");
        break;
    case LOGGER_FATAL:
        n = sprintf(buffer, "[FATAL] ");
        break;
    case LOGGER_INFO:
        n = sprintf(buffer, "[INFO] ");
        break;
    case LOGGER_TRACE:
        n = sprintf(buffer, "[TRACE] ");
        break;
    case LOGGER_WARNING:
        n = sprintf(buffer, "[WARNING] ");
        break;
    }

    n += sprintf(&buffer[n], "%s:%lu \t", file, line );
    va_list list;
    va_start(list, fmt);
    n += vsprintf(&buffer[n], fmt, list);
    va_end(list);
    usart3_write_msg(buffer, (u32)(n+1));
}

static inline u32 compute_baud_rate(u32 baud_rate)
{
    return ((PERIPH_CLK + (baud_rate / 2U)) / baud_rate);
}
