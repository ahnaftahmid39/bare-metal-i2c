typedef struct {
	volatile uint32_t CR1;        /*!< I2C Control register 1,     Address offset: 0x00 */
	volatile uint32_t CR2;        /*!< I2C Control register 2,     Address offset: 0x04 */
	volatile uint32_t OAR1;       /*!< I2C Own address register 1, Address offset: 0x08 */
	volatile uint32_t OAR2;       /*!< I2C Own address register 2, Address offset: 0x0C */
	volatile uint32_t DR;         /*!< I2C Data register,          Address offset: 0x10 */
	volatile uint32_t SR1;        /*!< I2C Status register 1,      Address offset: 0x14 */
	volatile uint32_t SR2;        /*!< I2C Status register 2,      Address offset: 0x18 */
	volatile uint32_t CCR;        /*!< I2C Clock control register, Address offset: 0x1C */
	volatile uint32_t TRISE;      /*!< I2C TRISE register,         Address offset: 0x20 */
	volatile uint32_t FLTR;       /*!< I2C FLTR register,          Address offset: 0x24 */
} I2C_TypeDef;

#define I2C1 ((I2C_TypeDef *)0x40005400)
#define I2C2 ((I2C_TypeDef *)0x40005800)
#define I2C3 ((I2C_TypeDef *)0x40005C00)