#include "bmp_registers.h"
#include "string.h"
#include <stdlib.h>
#include <math.h>
// #include "../utils.h"


#define BMP_ADDRESS BMP_ADDRESS_SDO_GROUND

void BMP_reset();
void BMP_control(uint8_t mode, uint8_t osrs_p, uint8_t osrs_t);
void BMP_config(uint8_t t_standby, uint8_t t_IIR, uint8_t spi_3);

void BMP_Write(uint8_t address, uint8_t reg, uint8_t data) {
	I2C_Reset();
	I2C_Start();
	I2C_Address(address);
	I2C_Write(reg);
	I2C_Write(data);
	I2C_Stop();
}

void BMP_Read(uint8_t address, uint8_t reg, uint8_t* buffer, uint8_t size) {
	I2C_Reset();
	I2C_Start();
	I2C_Address(address);
	I2C_Write(reg);
	I2C_Start();  // repeated start
	I2C_Read(address, buffer, size);
	I2C_Stop();
}


void BMP_init() {
	// uint8_t* id = (uint8_t*)malloc((sizeof(uint8_t) * 12));
	uint8_t id;
	BMP_Read(BMP_ADDRESS, BMP_ID_REG, &id, 1);
	if (id == BMP_ID) {
		// uart_log("BMP init: Success init id-received: ");
		// uart_logln(intToString(id));
		uart_logln("BMP: Resetting...");
		BMP_reset();
		uart_logln("BMP: Reset done [x]");
		uart_logln("BMP: Setting controls...");
		// normal mode = 3, pressure oversampling = 5, temperature oversampling = 5
		BMP_control(3, 5, 5);
		uart_logln("BMP: Controls done [x]");

	}
	else {
		uart_log("BMP init: Failed to init. Retrying.., id-received: ");
		uart_logln(intToString(id));
		Delay_ms(1000);
		BMP_init();
	}
}

uint8_t calib_loaded = 0;
uint16_t dig_T1;
int16_t dig_T2;
int16_t dig_T3;
uint16_t dig_P1;
int16_t dig_P2;
int16_t dig_P3;
int16_t dig_P4;
int16_t dig_P5;
int16_t dig_P6;
int16_t dig_P7;
int16_t dig_P8;
int16_t dig_P9;
int16_t dig_reserved;

int32_t t_fine;


void BMP_load_trimming_params() {
	uint8_t calib[30];
	BMP_Read(BMP_ADDRESS, BMP_CALIB_ADDR, calib, 26);

	dig_T1 = (uint16_t)(calib[1] << 8 | calib[0]);
	dig_T2 = (int16_t)(calib[3] << 8 | calib[2]);
	dig_T3 = (int16_t)(calib[5] << 8 | calib[4]);
	dig_P1 = (uint16_t)(calib[7] << 8 | calib[6]);
	dig_P2 = (int16_t)(calib[9] << 8 | calib[8]);
	dig_P3 = (int16_t)(calib[11] << 8 | calib[10]);
	dig_P4 = (int16_t)(calib[13] << 8 | calib[12]);
	dig_P5 = (int16_t)(calib[15] << 8 | calib[14]);
	dig_P6 = (int16_t)(calib[17] << 8 | calib[16]);
	dig_P7 = (int16_t)(calib[19] << 8 | calib[18]);
	dig_P8 = (int16_t)(calib[21] << 8 | calib[20]);
	dig_P9 = (int16_t)(calib[23] << 8 | calib[22]);

	// for debug purpose
	// dig_P4 = 2855;

	calib_loaded = 1;
}

/*
	If the value 0xB6 is written to the register,
	the device is reset using the complete power - on - reset procedure.Writing other values than 0xB6 has
	no effect.
*/
void BMP_reset() {
	BMP_Write(BMP_ADDRESS, BMP_RESET, (uint8_t)0xB6);
	Delay_ms(10);
}

/*
	set mode, oversampling of pressure and temperature
	[7, 6, 5] -> mode
	[4, 3, 2] -> oversampling of pressure
	[1, 0] 		-> oversampling of temperature
*/
void BMP_control(uint8_t mode, uint8_t osrs_p, uint8_t osrs_t) {
	uint8_t byte = 0;
	byte |= osrs_t << 5;
	byte |= osrs_p << 2;
	byte |= mode << 0;
	BMP_Write(BMP_ADDRESS, BMP_CTRL_MEAS, byte);
	Delay_ms(10);
}

/*
	sets the rate, filter and interface options of the device
	[7, 6, 5] -> Controls inactive duration t_standby in normal mode.
	[4, 3, 2] -> Controls the time constant of the IIR filter
	[0] 			-> Enables 3-wire SPI interface when set to ‘1’.
*/
void BMP_config(uint8_t t_standby, uint8_t t_IIR, uint8_t spi_3) {
	uint8_t byte = 0x00;
	byte |= t_standby << 5;
	byte |= t_IIR << 2;
	byte |= spi_3 << 0;
	BMP_Write(BMP_ADDRESS, BMP_CONFIG, byte);
	Delay_ms(10);
}

//TODO
/*
	[3] -> Automatically set to ‘1’ whenever a conversion is
running and back to ‘0’ when the results have been
transferred to the data registers.
	[0] -> Automatically set to ‘1’ when the NVM data are being
copied to image registers and back to ‘0’ when the
copying is done. The data are copied at power-on-
reset and before every conversion.
*/
void BMP_get_status() {

}



double BMP_Temp_Compensation(int32_t adc_T) {
	double var1, var2, T;
	var1 = (((double)adc_T) / 16384.0 - ((double)dig_T1) / 1024.0) * ((double)dig_T2);
	var2 = ((((double)adc_T) / 131072.0 - ((double)dig_T1) / 8192.0) *
		(((double)adc_T) / 131072.0 - ((double)dig_T1) / 8192.0)) * ((double)dig_T3);
	t_fine = (int32_t)(var1 + var2);
	T = (var1 + var2) / 5120.0;
	return T;
}

// Returns pressure in Pa as double. Output value of “96386.2” equals 96386.2 Pa = 963.862 hPa
double BMP_Press_Compensation(int32_t adc_P) {
	double var1, var2, p;
	var1 = ((double)t_fine / 2.0) - 64000.0;
	var2 = var1 * var1 * ((double)dig_P6) / 32768.0;
	var2 = var2 + var1 * ((double)dig_P5) * 2.0;
	var2 = (var2 / 4.0) + (((double)dig_P4) * 65536.0);
	var1 = (((double)dig_P3) * var1 * var1 / 524288.0 + ((double)dig_P2) * var1) / 524288.0;
	var1 = (1.0 + var1 / 32768.0) * ((double)dig_P1);
	if (var1 == 0.0) {
		return 0; // avoid exception caused by division by zero
	}
	p = 1048576.0 - (double)adc_P;
	p = (p - (var2 / 4096.0)) * 6250.0 / var1;
	var1 = ((double)dig_P9) * p * p / 2147483648.0;
	var2 = p * ((double)dig_P8) / 32768.0;
	p = p + (var1 + var2 + ((double)dig_P7)) / 16.0;
	return p;
}

int32_t* BMP_get_sample_measurements() {
	int32_t* adc = (int32_t*)malloc((sizeof(int32_t) * 3));
	uint8_t data[10];

	BMP_Read(BMP_ADDRESS, BMP_PRESS_MSB, data, 6);

	adc[0] = (int32_t)(data[0] << 12 | data[1] << 4 | data[2] >> 4);
	adc[1] = (int32_t)(data[3] << 12 | data[4] << 4 | data[5] >> 4);

	return adc;
}

void print_calibration_data() {
	if (calib_loaded) {
		uart_log("digT1: ");
		uart_logln(intToString((int)dig_T1));
		uart_log("digT2: ");
		uart_logln(intToString((int)dig_T2));
		uart_log("digT3: ");
		uart_logln(intToString((int)dig_T3));
		uart_log("digP1: ");
		uart_logln(intToString((int)dig_P1));
		uart_log("digP2: ");
		uart_logln(intToString((int)dig_P2));
		uart_log("digP3: ");
		uart_logln(intToString((int)dig_P3));
		uart_log("digP4: ");
		uart_logln(intToString((int)dig_P4));
		uart_log("digP5: ");
		uart_logln(intToString((int)dig_P5));
		uart_log("digP6: ");
		uart_logln(intToString((int)dig_P6));
		uart_log("digP7: ");
		uart_logln(intToString((int)dig_P7));
		uart_log("digP8: ");
		uart_logln(intToString((int)dig_P8));
		uart_log("digP9: ");
		uart_logln(intToString((int)dig_P9));
	}
}

void BMP_get_actual_measurements() {
	if (calib_loaded == 0) {
		BMP_load_trimming_params();
		uart_logln("BMP: Loaded calibration data");
		print_calibration_data();
	}
	// int32_t* adc = BMP_get_sample_measurements();
	// uart_logln("BMP: Loaded sample measure data");

	// int32_t adc_P = adc[0];
	// int32_t adc_T = adc[1];
	// int32_t temperature = BMP_compensate_T_int32(adc_T);
	// uint32_t pressure = BMP_compensate_P_int64(adc_P);

	uint8_t data[6];
	int32_t adc_P, adc_T;
	BMP_Read(BMP_ADDRESS, BMP_PRESS_MSB, data, 6);
	uart_logln("BMP: Loaded sample measure data");

	adc_P = data[0] << 12 | data[1] << 4 | data[2] >> 4;
	adc_T = data[3] << 12 | data[4] << 4 | data[5] >> 4;

	uart_log("ADC_P: ");
	uart_log(intToString(adc_P));
	uart_log(", ADC_T: ");
	uart_logln(intToString(adc_T));

	int ref_pressure = 101325;

	double temperature = BMP_Temp_Compensation(adc_T);
	double pressure = BMP_Press_Compensation(adc_P);
	double altitude = (1 - (double)pow(((double)pressure / (double)ref_pressure), (1 / 5.257))) * (double)44330;
	// double temperature = (double)(BMP_Temp_Compensation(adc_T) / 100);
	// double pressure = (double)(BMP_Press_Compensation(adc_P) / 256.0);
	// double altitude = (1 - (double)pow(((double)pressure / (double)ref_pressure), (1 / 5.257))) * (double)44330;

	uart_log("Temperature: ");
	uart_log(doubleToString(temperature));

	uart_log(", Pressure: ");
	uart_log(doubleToString(pressure));

	uart_log(", Altitude: ");
	uart_logln(doubleToString(altitude));
}