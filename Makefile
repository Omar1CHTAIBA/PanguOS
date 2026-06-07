ASM=nasm
CC=gcc
LD=ld
OBJCOPY=objcopy

SRC=src
BUILD=build

# Added -I$(SRC) to allow header inclusion from the src directory
CFLAGS=-m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -nodefaultlibs -I$(SRC)
LDFLAGS=-m elf_i386

# Dynamically find all .c files in the source folder
C_SOURCES = $(wildcard $(SRC)/*.c)
# Map them to corresponding .o files in the build folder
C_OBJECTS = $(patsubst $(SRC)/%.c, $(BUILD)/%.o, $(C_SOURCES))

all: directories floppy

directories:
	mkdir -p $(BUILD)

# Build Bootloader
$(BUILD)/boot.bin: $(SRC)/boot.asm
	$(ASM) -f bin $(SRC)/boot.asm -o $(BUILD)/boot.bin

# Build Kernel Entry
$(BUILD)/kernel_entry.o: $(SRC)/kernel_entry.asm
	$(ASM) -f elf32 $(SRC)/kernel_entry.asm -o $(BUILD)/kernel_entry.o

# Build C Files
$(BUILD)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link Kernel
$(BUILD)/kernel.bin: $(BUILD)/kernel_entry.o $(C_OBJECTS) linker.ld
	$(LD) $(LDFLAGS) -T linker.ld $(BUILD)/kernel_entry.o $(C_OBJECTS) -o $(BUILD)/kernel.elf
	$(OBJCOPY) -O binary $(BUILD)/kernel.elf $(BUILD)/kernel.bin

# Create Floppy Image
floppy: $(BUILD)/boot.bin $(BUILD)/kernel.bin
	dd if=/dev/zero of=$(BUILD)/os.img bs=512 count=2880
	dd if=$(BUILD)/boot.bin of=$(BUILD)/os.img conv=notrunc
	dd if=$(BUILD)/kernel.bin of=$(BUILD)/os.img bs=512 seek=1 conv=notrunc

run: all
	qemu-system-i386 -fda $(BUILD)/os.img

clean:
	rm -rf $(BUILD)