typedef struct {
  uint32_t volatile ACR;     /* Offset: 0x00 Access Control Register */
  uint32_t volatile KEYR;    /* Offset: 0x04 Key Register */
  uint32_t volatile OPTKEYR; /* Offset: 0x08 Option Key Register */
  uint32_t volatile SR;      /* Offset: 0x0C Status Register */
  uint32_t volatile CR;      /* Offset: 0x10 Control Register */
  uint32_t volatile OPTCR;   /* Offset: 0x14 Option Control Register */
} FLASH_t;

#define FLASH ((FLASH_t *)0x40023C00)

#define FLASH_ACR_ICEN 1 << 8
#define FLASH_ACR_DCEN 1 << 9
#define FLASH_ACR_PRFTEN 1 << 10
#define FLASH_ACR_LATENCY_5WS 5 << 0
