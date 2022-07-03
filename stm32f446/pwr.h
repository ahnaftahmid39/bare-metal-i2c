typedef struct {
  uint32_t volatile CR;  /* Offset: 0x00 (R/W) Power control register */
  uint32_t volatile CSR; /* Offset: 0x04 (R/W) Power control status register */
} PWR_t;

#define PWR ((PWR_t *)0x40007000)

#define PWR_CR_VOS 3 << 14