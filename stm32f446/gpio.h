typedef struct {
  uint32_t volatile MODER;   /* Offset: 0x00 (R/W) Mode Register */
  uint32_t volatile OTYPER;  /* Offset: 0x04 (R/W) Output Type Register */
  uint32_t volatile OSPEEDR; /* Offset: 0x08 (R/W) Output Speed Register */
  uint32_t volatile PUPDR;   /* Offset: 0x0C (R/W) Pull-up/Pull-down Register */
  uint32_t volatile IDR;     /* Offset: 0x10 (R/W) Input Data Register */
  uint32_t volatile ODR;     /* Offset: 0x14 (R/W) Output Data Register */
  uint32_t volatile BSRR;    /* Offset: 0x18 (R/W) Bit Set/Reset Register */
  uint32_t volatile LCKR;    /* Offset: 0x1C (R/W) Configuration Lock Register */
  uint32_t volatile AFR[2];    /* Offset: 0x20 (R/W) Alternate Function Low Register */
} GPIO_t;

#define GPIOA ((GPIO_t *)0x40020000)
#define GPIOB ((GPIO_t *)0x40020400)
#define GPIOC ((GPIO_t *)0x40020800)
#define GPIOD ((GPIO_t *)0x40020C00)
#define GPIOE ((GPIO_t *)0x40021000)
#define GPIOF ((GPIO_t *)0x40021400)
#define GPIOG ((GPIO_t *)0x40021800)
#define GPIOH ((GPIO_t *)0x40021C00)