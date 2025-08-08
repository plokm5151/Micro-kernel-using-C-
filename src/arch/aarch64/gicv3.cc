#include <stdint.h>
#include "arch/gicv3.h"

static inline void enable_sre_el1() {
  uint64_t v = 0;
  asm volatile("mrs %0, ICC_SRE_EL1" : "=r"(v));
  v |= 1ull; // SRE enable
  asm volatile("msr ICC_SRE_EL1, %0" :: "r"(v));
  asm volatile("isb");
}

void gic_init() {
  // Optional: wake Redistributor (many QEMU setups are already awake)
  // Clear ProcessorSleep bit in GICR_WAKER (offset 0x0014), wait ChildrenAsleep clear.
  // uint32_t w = mmio_r32(GICR_BASE + 0x0014);
  // w &= ~(1u << 1); // ProcessorSleep=0
  // mmio_w32(GICR_BASE + 0x0014, w);
  // while (mmio_r32(GICR_BASE + 0x0014) & (1u << 2)) { }

  // ---- SGI/PPI registers are in the SGI_base frame (GICR_BASE + 0x10000) ----
  // Group all PPIs to Non-secure Group1
  mmio_w32(GICR_SGI_BASE + 0x0080, 0xFFFFFFFFu); // GICR_IGROUPR0
  mmio_w32(GICR_SGI_BASE + 0x0D00, 0x00000000u); // GICR_IGRPMODR0: NS Group1

  // Level-triggered for PPIs: GICR_ICFGR1 (INTIDs 16..31)
  mmio_w32(GICR_SGI_BASE + 0x00C4, 0x00000000u);

  // Enable Virtual Timer PPI #27
  mmio_w32(GICR_SGI_BASE + 0x0100, (1u << 27)); // GICR_ISENABLER0

  // Priority for INTID 27 (one byte per INTID from 0..31)
  volatile uint8_t* prio = (volatile uint8_t*)(GICR_SGI_BASE + 0x0400);
  prio[27] = 0x80;

  // Distributor: enable Group1NS
  mmio_w32(GICD_BASE + 0x0000, (1u << 1)); // GICD_CTLR.EnableGrp1NS

  // CPU interface: sysregs path
  enable_sre_el1();
  asm volatile("msr ICC_PMR_EL1, %0" :: "r"(0xFFull)); // unmask all priorities
  asm volatile("msr ICC_BPR1_EL1, %0" :: "r"(0ull));   // no binning
  asm volatile("msr ICC_IGRPEN1_EL1, %0" :: "r"(1ull));// enable Group1
  asm volatile("isb");
}

uint32_t gic_ack() {
  uint64_t iar = 0;
  asm volatile("mrs %0, ICC_IAR1_EL1" : "=r"(iar));
  return (uint32_t)iar;
}

void gic_eoi(uint32_t iar) {
  asm volatile("msr ICC_EOIR1_EL1, %0" :: "r"((uint64_t)iar));
  asm volatile("msr ICC_DIR_EL1, %0"   :: "r"((uint64_t)iar));
}
