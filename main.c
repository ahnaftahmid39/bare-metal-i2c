#include <stdio.h>

#include "stm32f446/stm32f446re.h"
#include "configure/clock.h"
#include "configure/usart.h"
#include "utils.h"
#include "configure/timer.h"
#include "configure/i2c.h"
#include "bmp/bmp_funcs.h"

#define PLL_M 4
#define PLL_N 180
#define PLL_P 0  // PLLP = 2


int main(void) {
  SysClockConfig();
  Uart2Config();
  uart_logln("USART2 initialized");
  TIM6Config();
  uart_logln("TIM6 initialized");
  I2C_Config();
  uart_logln("I2C initialized");
  BMP_init();
  uart_logln("BMP initialized");

  int readcnt = 10;

  double powerd = pow(10, 3);
  uart_logln(doubleToString(powerd));

  while (1) {
    Delay_ms(2000);
    if (readcnt-- > 0) BMP_get_actual_measurements();
  }
}

void USART2_IRQHandler() {
  uint8_t data = UART2_GetChar();
  UART2_SendChar(data);
}

// 2. implement the fault handlers
void HardFault_Handler(void) {
  printf("Exception : Hardfault\n");
  while (1);
}

void MemManage_Handler(void) {
  printf("Exception : MemManage\n");
  while (1);
}

void BusFault_Handler(void) {
  printf("Exception : BusFault\n");
  while (1);
}
