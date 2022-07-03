#define PLL_M 4
#define PLL_N 180
#define PLL_P 0  // PLLP = 2

void SysClockConfig(void) {
	/*************>>>>>>> STEPS FOLLOWED <<<<<<<<************
		1. ENABLE HSE and wait for the HSE to become Ready
		2. Set the POWER ENABLE CLOCK and VOLTAGE REGULATOR
		3. Configure the FLASH PREFETCH and the LATENCY Related Settings
		4. Configure the PRESCALARS HCLK, PCLK1, PCLK2
		5. Configure the MAIN PLL
		6. Enable the PLL and wait for it to become ready
		7. Select the Clock Source and wait for it to be set
	********************************************************/

	RCC->CR |= RCC_CR_HSEON;
	while (!(RCC->CR & RCC_CR_HSERDY));

	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	PWR->CR |= PWR_CR_VOS;

	FLASH->ACR = FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_PRFTEN | FLASH_ACR_LATENCY_5WS;
	RCC->CFGR &= ~(1 << 4);

	RCC->CFGR |= (5 << 10);

	RCC->CFGR |= (4 << 13);

	RCC->PLLCFGR = (PLL_M << 0) | (PLL_N << 6) | (PLL_P << 16) | (RCC_PLLCFGR_PLLSRC_HSE);
	RCC->CR |= RCC_CR_PLLON;
	while (!(RCC->CR & RCC_CR_PLLRDY));

	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while (!(RCC->CFGR & (2 << 2)));
}