#include "drivers/uart_pl011.h"
#include "arch/timer.h"

static inline unsigned long long cntfrq() {
  unsigned long long f=0; asm volatile("mrs %0, CNTFRQ_EL0":"=r"(f)); return f;
}
static inline void cntv_set_tval(uint32_t v){ asm volatile("msr CNTV_TVAL_EL0, %0"::"r"((uint64_t)v)); }
static inline void cntv_enable(int en){
  uint64_t ctl = en ? 1ull /*EN*/ : 0ull;
  // bit0 EN=1, bit1 IMASK=0
  asm volatile("msr CNTV_CTL_EL0, %0"::"r"(ctl));
}

void timer_init_hz(uint32_t hz){
  unsigned long long f = cntfrq();
  uint32_t tval = (uint32_t)(f / hz);
  cntv_set_tval(tval);
  cntv_enable(1);
}

void timer_irq(){
  // re-arm
  unsigned long long f = cntfrq();
  uint32_t tval = (uint32_t)(f / 1000u);
  cntv_set_tval(tval);
  uart_puts(".");
}
