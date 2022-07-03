typedef struct {
	uint32_t volatile CR1;
	uint32_t volatile CR2;
	uint32_t reserved;
	uint32_t volatile DIER;
	uint32_t volatile SR;
	uint32_t volatile EGR;
	uint32_t reserved2[3];
	uint32_t volatile CNT;
	uint32_t volatile PSC;
	uint32_t volatile ARR;
} TIM_t;

#define TIM1 ((TIM_t *)0x40010000)
#define TIM2 ((TIM_t *)0x40000000)
#define TIM3 ((TIM_t *)0x40000400)
#define TIM4 ((TIM_t *)0x40000800)
#define TIM5 ((TIM_t *)0x40000C00)
#define TIM6 ((TIM_t *)0x40001000)
#define TIM7 ((TIM_t *)0x40001400)
#define TIM8 ((TIM_t *)0x40010400)
#define TIM9 ((TIM_t *)0x40014000)
#define TIM10 ((TIM_t *)0x40014400)
#define TIM11 ((TIM_t *)0x40014800)
#define TIM12 ((TIM_t *)0x40001800)
#define TIM13 ((TIM_t *)0x40001C00)
#define TIM14 ((TIM_t *)0x40002000)
