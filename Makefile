CC					= avr-g++
OBJCOPY				= avr-objcopy
CFLAGS				= -Wall					\
						-I./include			\
						-O$(OPTIMIZATION)	\
						-mmcu=$(DEVICE)		\
						-std=c++11			\
						-DF_CPU=$(F_CPU)	\
						-DF_USB=$(F_USB)
FLASH_COPY_FLAGS	= -R .eeprom
EEPROM_COPY_FLAGS	= -j .eeprom --change-section-lma .eeprom=0x0
AVRDUDE				= avrdude -p $(DEVICE) -c $(PROGRAMMER)
OBJDIR				= build
DUMPDIR				= dump_$(DEVICE)
SRCDIR				= src

NAME				= hax
DEVICE				= at90usb162
OPTIMIZATION		= s
PROGRAMMER			= usbtiny
F_CPU				= 16000000UL
F_USB				= $(F_CPU)

PRODUCT_BASE		= $(OBJDIR)/$(NAME)
SOURCES				= main.cpp LCD.cpp Pin.cpp Port.cpp
OBJS				= $(SOURCES:%.cpp=$(OBJDIR)/%.o)

all: $(PRODUCT_BASE).hex $(PRODUCT_BASE).eeprom

$(OBJDIR):
	mkdir $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(PRODUCT_BASE).elf: $(OBJS) $(wildcard include/*.h)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

$(PRODUCT_BASE).hex: $(PRODUCT_BASE).elf
	$(OBJCOPY) $(FLASH_COPY_FLAGS) -O ihex $< $@

$(PRODUCT_BASE).eeprom: $(PRODUCT_BASE).elf
	$(OBJCOPY) $(EEPROM_COPY_FLAGS) -O ihex $< $@

flash: build
	$(AVRDUDE) -U flash:w:build/$(NAME).hex:i

flasheeprom:
	$(AVRDUDE) -U eeprom:w:build/$(NAME).eeprom:i

flashall: flash flasheeprom

dump: cleandump
	mkdir dump_$(DEVICE)
	$(AVRDUDE)									\
		-U flash:r:$(DUMPDIR)/flash.bin:r 		\
		-U eeprom:r:$(DUMPDIR)/eeprom.bin:r 	\
		-U hfuse:r:$(DUMPDIR)/hfuse.bin:r 		\
		-U lfuse:r:$(DUMPDIR)/lfuse.bin:r 		\
		-U efuse:r:$(DUMPDIR)/efuse.bin:r

restoredump:
	$(AVRDUDE)									\
		-U flash:w:$(DUMPDIR)/flash.bin:r 		\
		-U eeprom:w:$(DUMPDIR)/eeprom.bin:r 	\
		-U hfuse:w:$(DUMPDIR)/hfuse.bin:r 		\
		-U lfuse:w:$(DUMPDIR)/lfuse.bin:r 		\
		-U efuse:w:$(DUMPDIR)/efuse.bin:r

shell:
	$(AVRDUDE) -t

erase:
	$(AVRDUDE) -e

cleandump:
	rm -rf $(DUMPDIR)

clean:
	rm -f $(OBJDIR)/*

cleanall: clean cleandump
