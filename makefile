###############################################################################
# 1. Toolchain Setup
###############################################################################
RISCV_PREFIX = riscv64-elf-
CC           = $(RISCV_PREFIX)gcc
OPENOCD      = openocd

###############################################################################
# 2. Directories and Files
###############################################################################
SRC_DIR      = src
BUILD_DIR    = build
LINKER       = linker/metal.lds
OPENOCD_CFG  = target/sifive-hifive1-revb.cfg
INC_DIR      = include

# Gather C and Assembly source files
SRC_C  = $(wildcard $(SRC_DIR)/*.c)
SRC_S  = $(wildcard $(SRC_DIR)/*.s)
SRCS   = $(SRC_C) $(SRC_S)

# Convert each .c / .S to .o in build/ folder
OBJS   = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC_C)) \
         $(patsubst $(SRC_DIR)/%.s, $(BUILD_DIR)/%.o, $(SRC_S))

# Final ELF output
ELF    = $(BUILD_DIR)/firmware.elf

###############################################################################
# 3. Compiler/Linker Flags
###############################################################################
CFLAGS  = -march=rv32imac -mabi=ilp32 \
          -I$(INC_DIR) \
          -Wall -Wextra -O2 \
          -ffreestanding -nostartfiles -specs=nosys.specs
LDFLAGS = -T$(LINKER) -Wl,--gc-sections

###############################################################################
# 4. Build Rules
###############################################################################
all: $(ELF)

# Ensure the build directory exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# (a) Compile .c -> .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# (b) Compile .S -> .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.s | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# (c) Link all object files into the final ELF
$(ELF): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJS)

###############################################################################
# 5. Flash and Debug
###############################################################################
flash: $(ELF)
	$(OPENOCD) -f $(OPENOCD_CFG) -c "program $(ELF) verify reset exit"

debug: $(ELF)
	$(OPENOCD) -f $(OPENOCD_CFG) &
	sleep 1
	$(RISCV_PREFIX)gdb $(ELF) -ex "target remote localhost:3333" -ex "monitor reset halt"

###############################################################################
# 6. Clean
###############################################################################
clean:
	rm -rf $(BUILD_DIR)
