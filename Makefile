CC=avr-g++
OBJCOPY=avr-objcopy
DEVICE=at90usb162
F_CPU=16000000UL
F_USB=$(F_CPU)
PROGRAMMER=usbtiny
OPTIMIZATION=s
CFLAGS=	-Wall \
		-I./include \
		-O$(OPTIMIZATION) \
		-mmcu=$(DEVICE) \
		-std=c++11 \
		-DF_CPU=$(F_CPU) \
		-DF_USB=$(F_USB)
NAME=hax
AVRDUDE=avrdude -p $(DEVICE) -c $(PROGRAMMER)

OBJS=build/main.o build/LCD.o build/Pin.o build/Port.o

all: build

build: $(OBJS) build/$(NAME).elf build/$(NAME).hex build/$(NAME).eeprom

build/%.elf: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $<

build/%.hex: build/%.elf
	$(OBJCOPY) -R .eeprom -O ihex $< $@

build/%.eeprom: build/%.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0x0 -O ihex $< $@

build/%.o: src/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

flash: build
	$(AVRDUDE) -U flash:w:build/$(NAME).hex:i

flasheeprom:
	$(AVRDUDE) -U eeprom:w:build/$(NAME).eeprom:i

flashall: flash flasheeprom

dump: cleandump
	mkdir dump_$(DEVICE)
	$(AVRDUDE)										\
		-U flash:r:dump_$(DEVICE)/flash.bin:r 		\
		-U eeprom:r:dump_$(DEVICE)/eeprom.bin:r 	\
		-U hfuse:r:dump_$(DEVICE)/hfuse.bin:r 		\
		-U lfuse:r:dump_$(DEVICE)/lfuse.bin:r 		\
		-U efuse:r:dump_$(DEVICE)/efuse.bin:r

restoredump:
	$(AVRDUDE)										\
		-U flash:w:dump_$(DEVICE)/flash.bin:r 		\
		-U eeprom:w:dump_$(DEVICE)/eeprom.bin:r 	\
		-U hfuse:w:dump_$(DEVICE)/hfuse.bin:r 		\
		-U lfuse:w:dump_$(DEVICE)/lfuse.bin:r 		\
		-U efuse:w:dump_$(DEVICE)/efuse.bin:r

shell:
	$(AVRDUDE) -t

erase:
	$(AVRDUDE) -e

cleandump:
	rm -rf dump_$(DEVICE)

clean:
	rm -f $(OBJS) build/{$(NAME).elf,$(NAME).hex,$(NAME).eeprom}

cleanall: clean cleandump
