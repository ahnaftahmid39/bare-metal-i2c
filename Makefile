CC=arm-none-eabi-gcc
MACH=cortex-m4
# CFLAGS= -c -mcpu=$(MACH) -mthumb -mfloat-abi=soft -std=gnu11 -Wall -O0
CFLAGS= -c -mcpu=$(MACH) -mthumb -mfloat-abi=soft -std=gnu11 -Wno-unused-variable -O0

LDFLAGS= -mcpu=$(MACH) -mthumb -mfloat-abi=soft -u _printf_float  --specs=nano.specs -T stm32_ls.ld -Wl,-Map=final.map

all:main.o stm32_startup.o syscalls.o final.elf
allclean: clean all
main.o: main.c
	$(CC) $(CFLAGS) -o $@ $^

stm32_startup.o:stm32_startup.c
	$(CC) $(CFLAGS) -o $@ $^

syscalls.o:syscalls.c
	$(CC) $(CFLAGS) -o $@ $^

final.elf: main.o stm32_startup.o syscalls.o
	$(CC) $(LDFLAGS) -o $@ $^  -lm -lc

clean:
	rm -rf *.o *.elf *.map

load:
	openocd -f board/stm32f4discovery.cfg
