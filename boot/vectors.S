  .align 11
  .global __vec_base
__vec_base:

// EL1t
  .balign 0x80
  b .

  .balign 0x80
  b irq_el1

  .balign 0x80
  b .

  .balign 0x80
  b .

// EL1h
  .balign 0x80
  b .

  .balign 0x80
  b irq_el1

  .balign 0x80
  b .

  .balign 0x80
  b .

// EL0_64
  .balign 0x80
  b .

  .balign 0x80
  b irq_el1

  .balign 0x80
  b .

  .balign 0x80
  b .

  .global irq_el1
irq_el1:
  stp x0, x1, [sp, #-16]!
  stp x2, x3, [sp, #-16]!
  bl irq_handler_el1
  ldp x2, x3, [sp], #16
  ldp x0, x1, [sp], #16
  eret
