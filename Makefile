CC					= avr-g++
OBJCOPY				= avr-objcopy
CFLAGS				= -Wall					\
					  -Wno-reorder			\
						-I$(INCLUDEDIR)		\
						-O$(OPTIMIZATION)	\
						-mmcu=$(DEVICE)		\
						-std=c++11			\
						-DF_CPU=$(F_CPU)	\
						-DF_USB=$(F_USB)	\
#						-DNO_PORTMAP -DNO_PINMAP
FLASH_COPY_FLAGS	= -R .eeprom
EEPROM_COPY_FLAGS	= -j .eeprom --change-section-lma .eeprom=0x0
AVRDUDE				= avrdude -p $(DEVICE) -c $(PROGRAMMER) -V -B 1
OBJDIR				= build
DUMPDIR				= dump_$(DEVICE)
SRCDIR				= src
INCLUDEDIR			= include
PRODUCT_BASE		= $(OBJDIR)/$(NAME)
PRODUCT_ELF			= $(PRODUCT_BASE).elf
PRODUCT_APP			= $(PRODUCT_BASE).hex
PRODUCT_EEPROM		= $(PRODUCT_BASE).eeprom.hex
OBJS				= $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

NAME				= hax
DEVICE				= at90usb162
OPTIMIZATION		= s
PROGRAMMER			= usbtiny
F_CPU				= 16000000UL
F_USB				= $(F_CPU)

SOURCES				= $(wildcard $(SRCDIR)/*.cpp)

.PHONY: all
all: app eeprom

.PHONY: app eeprom
app: $(PRODUCT_APP)
eeprom: $(PRODUCT_EEPROM)

$(OBJDIR):
	mkdir $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(INCLUDEDIR)/%.h | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(PRODUCT_ELF): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

$(PRODUCT_APP): $(PRODUCT_BASE).elf
	$(OBJCOPY) $(FLASH_COPY_FLAGS) -O ihex $< $@

$(PRODUCT_EEPROM): $(PRODUCT_BASE).elf
	$(OBJCOPY) $(EEPROM_COPY_FLAGS) -O ihex $< $@

.PHONY: flashapp
FLASH_APP_ARGUMENTS=-U flash:w:$(PRODUCT_APP):i
flashapp: $(PRODUCT_APP)
	$(AVRDUDE) $(FLASH_APP_ARGUMENTS)

.PHONY: flasheeprom
FLASH_EEPROM_ARGUMENTS=-U eeprom:w:$(PRODUCT_EEPROM):i
flasheeprom: $(PRODUCT_EEPROM)
	$(AVRDUDE) $(FLASH_EEPROM_ARGUMENTS)

.PHONY: flashall
flashall: $(PRODUCT_APP) $(PRODUCT_EEPROM)
	$(AVRDUDE) $(FLASH_APP_ARGUMENTS) $(FLASH_EEPROM_ARGUMENTS)

.PHONY: dump
dump: cleandump
	mkdir dump_$(DEVICE)
	$(AVRDUDE)									\
		-U flash:r:$(DUMPDIR)/flash.bin:r 		\
		-U eeprom:r:$(DUMPDIR)/eeprom.bin:r 	\
		-U hfuse:r:$(DUMPDIR)/hfuse.bin:r 		\
		-U lfuse:r:$(DUMPDIR)/lfuse.bin:r 		\
		-U efuse:r:$(DUMPDIR)/efuse.bin:r

.PHONY: restoredump
restoredump:
	$(AVRDUDE)									\
		-U flash:w:$(DUMPDIR)/flash.bin:r 		\
		-U eeprom:w:$(DUMPDIR)/eeprom.bin:r 	\
		-U hfuse:w:$(DUMPDIR)/hfuse.bin:r 		\
		-U lfuse:w:$(DUMPDIR)/lfuse.bin:r 		\
		-U efuse:w:$(DUMPDIR)/efuse.bin:r

.PHONY: shell
shell:
	$(AVRDUDE) -t

.PHONY: erase
erase:
	$(AVRDUDE) -e

.PHONY: cleandump
cleandump:
	rm -rf $(DUMPDIR)

.PHONY: clean
clean:
	rm -f $(OBJDIR)/*

.PHONY: cleanall
cleanall: clean cleandump
