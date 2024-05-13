#ifndef USART_H_
#define USART_H_
#include "types.h"

void usart3_init(u32 baud_rate);
char usart3_read_byte(void);
void usart3_write_byte(int ch);

void usart3_read(void* buff, u32 len);
void usart3_write(void* buff, u32 len);

void usart3_read_msg(void* buff, u32 len);
void usart3_write_msg(void* buff, u32 len);

enum
{
    LOGGER_DEBUG = 0,
    LOGGER_INFO,
    LOGGER_WARNING,
    LOGGER_ERROR,
    LOGGER_FATAL,
    LOGGER_TRACE,
};

void _log(u8 type, const char* file, u32 line, const char* fmt, ...);

#define LOG_DEBUG(...)      _log(LOGGER_DEBUG, __FILE__, __LINE__, __VA_ARGS__)

#endif /* USART_H_ */