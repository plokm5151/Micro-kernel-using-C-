
#pragma once
#include <stdint.h>
#define UART0_BASE   0x09000000UL
#define UARTDR       0x00
#define UARTFR       0x18
#define UARTIBRD     0x24
#define UARTFBRD     0x28
#define UARTLCR_H    0x2C
#define UARTCR       0x30
#define UARTIMSC     0x38
static inline void mmio_write(uint64_t addr, uint32_t val){ *(volatile uint32_t*)(addr)=val; }
static inline uint32_t mmio_read(uint64_t addr){ return *(volatile uint32_t*)(addr); }
void uart_init();
void uart_putc(char c);
void uart_puts(const char* s);
void uart_print_u64(unsigned long long v);
