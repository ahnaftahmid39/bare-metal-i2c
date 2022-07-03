typedef struct {
	uint32_t volatile SR;
	uint32_t volatile DR;
	uint32_t volatile BRR;
	uint32_t volatile CR1;
	uint32_t volatile CR2;
	uint32_t volatile CR3;
	uint32_t volatile GTPR;
} USART_t;

#define USART1 ((USART_t *) 0x40011000)
#define USART2 ((USART_t *) 0x40004400)
#define USART3 ((USART_t *) 0x40004800)
#define USART4 ((USART_t *) 0x40004C00)
#define USART5 ((USART_t *) 0x40005000)
#define USART6 ((USART_t *) 0x40011400)

#define USART2_IRQn (uint32_t)38