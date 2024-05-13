# Makefile template from
#	https://github.com/cpq/bare-metal-programming-guide/blob/main/steps/step-0-minimal/Makefile

INC_DIR = -Iinclude/
INCLUDES = $(INC_DIR)
INCLUDES += -I../../CMSIS/Device/ST/STM32H7xx/Include/
INCLUDES += -I../../CMSIS/Include/
INCLUDES += -I../../kernel/include/

DEFS += -D CORE_CM7
DEFS += -D STM32H755xx

CFLAGS  ?=  -W -Wall -Wextra --pedantic -Wundef -Wshadow -Wdouble-promotion \
            -Wformat-truncation -fno-common -Wconversion  \
            -g3 -ffunction-sections -fdata-sections -I. $(INCLUDES) -mgeneral-regs-only \
            -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 $(EXTRA_CFLAGS) ${DEFS}

TESTFLAGS = -W -Wall -Wextra --pedantic -Werror $(INCLUDES)

LDFLAGS ?= -Tflash/STM32H755ZITX_FLASH.ld --specs nano.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=$@.map
SOURCES += src/*.c
SOURCES += startup/*.s
SOURCES += ../../kernel/src/*.c

TEST_SOURCES += test/*.c


all: build

test: test_build test_run
test_build: $(SOURCES) $(TEST_SOURCES)
	gcc $(TEST_SOURCES) $(TESTFLAGS) -o build/test.out

test_run:
	build/test.out

build: clean build/firmware.bin

build/firmware.elf: $(SOURCES)
	arm-none-eabi-gcc $(SOURCES) $(CFLAGS) $(LDFLAGS) -o $@

build/firmware.bin: build/firmware.elf
	arm-none-eabi-objcopy -O binary $< $@

flash: build/firmware.elf
	openocd -f interface/stlink.cfg -f target/stm32h7x_dual_bank.cfg -c "program $< verify reset exit"

clean:
	rm -rf build/firmware*.* build/test.out