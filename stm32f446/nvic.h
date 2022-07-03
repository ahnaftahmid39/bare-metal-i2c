typedef struct {
	volatile uint32_t ISER[8U];               /*!< Offset: 0x000 (R/W)  Interrupt Set Enable Register */
	uint32_t RESERVED0[24U];
	volatile uint32_t ICER[8U];               /*!< Offset: 0x080 (R/W)  Interrupt Clear Enable Register */
	uint32_t RSERVED1[24U];
	volatile uint32_t ISPR[8U];               /*!< Offset: 0x100 (R/W)  Interrupt Set Pending Register */
	uint32_t RESERVED2[24U];
	volatile uint32_t ICPR[8U];               /*!< Offset: 0x180 (R/W)  Interrupt Clear Pending Register */
	uint32_t RESERVED3[24U];
	volatile uint32_t IABR[8U];               /*!< Offset: 0x200 (R/W)  Interrupt Active bit Register */
	uint32_t RESERVED4[56U];
	volatile uint8_t  IP[240U];               /*!< Offset: 0x300 (R/W)  Interrupt Priority Register (8Bit wide) */
	uint32_t RESERVED5[644U];
	volatile  uint32_t STIR;                   /*!< Offset: 0xE00 ( /W)  Software Trigger Interrupt Register */
}  NVIC_Type;

#define NVIC ((NVIC_Type *)0xE000E100)

void NVIC_EnableIRQ(uint32_t IRQn) {
	if ((int32_t)(IRQn) >= 0) {
		NVIC->ISER[(IRQn >> 5UL)] = (uint32_t)(1UL << (IRQn & 0x1FUL));
	}
}

void NVIC_SetPriority(uint32_t IRQn, uint32_t priority) {
	if ((int32_t)(IRQn) >= 0) {
		NVIC->IP[((uint32_t)IRQn)] = (uint8_t)((priority << (8U - 3U)) & (uint32_t)0xFFUL);
	}
}