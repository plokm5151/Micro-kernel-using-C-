#pragma once
#include <stdint.h>

#define GICD_BASE    0x08000000UL
#define GICR_BASE    0x080A0000UL
#define GICR_SGI_BASE (GICR_BASE + 0x10000UL)  // SGI/PPI registers live here

static inline void mmio_w32(uint64_t a, uint32_t v){ *(volatile uint32_t*)a=v; }
static inline uint32_t mmio_r32(uint64_t a){ return *(volatile uint32_t*)a; }
static inline void mmio_w64(uint64_t a, uint64_t v){ *(volatile uint64_t*)a=v; }
static inline uint64_t mmio_r64(uint64_t a){ return *(volatile uint64_t*)a; }

void gic_init(void);
uint32_t gic_ack(void);
void gic_eoi(uint32_t i);
