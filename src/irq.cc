#include <stdint.h>
#include "drivers/uart_pl011.h"
#include "arch/gicv3.h"
#include "arch/timer.h"

extern "C" void irq_handler_el1(void) {
  uint32_t iar = gic_ack();
  uint32_t intid = iar & 0xFFFFFFu;
  if (intid == 27u) {           // virtual timer PPI
    timer_irq();
  } else {
    uart_puts("IRQ?\n");
  }
  gic_eoi(iar);
}
