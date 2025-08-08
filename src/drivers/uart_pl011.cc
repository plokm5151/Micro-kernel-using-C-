#include "drivers/uart_pl011.h"

void uart_init() {
  // disable before config
  mmio_write(UART0_BASE + UARTCR, 0x0);
  mmio_write(UART0_BASE + UARTIMSC, 0x0);

  // 24MHz / (16 * 115200) ≈ 13.02 -> IBRD=13, FBRD≈27
  mmio_write(UART0_BASE + UARTIBRD, 13);
  mmio_write(UART0_BASE + UARTFBRD, 27);

  // FIFO enable, 8 data bits, 1 stop, no parity
  mmio_write(UART0_BASE + UARTLCR_H, (1u<<4) | (3u<<5));

  // enable UART, TX, RX
  mmio_write(UART0_BASE + UARTCR, (1u<<0) | (1u<<8) | (1u<<9));
}

void uart_putc(char c) {
  // wait while TX FIFO full
  while (mmio_read(UART0_BASE + UARTFR) & (1u<<5)) { }
  mmio_write(UART0_BASE + UARTDR, (uint32_t)c);
}

void uart_puts(const char* s) {
  while (*s) {
    if (*s == '\n') uart_putc('\r');
    uart_putc(*s++);
  }
}

void uart_print_u64(unsigned long long v) {
  char buf[32];
  int i = 0;
  if (v == 0) { uart_putc('0'); return; }
  while (v > 0 && i < 31) { buf[i++] = (char)('0' + (v % 10)); v /= 10; }
  while (i--) uart_putc(buf[i]);
}
