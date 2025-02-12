# === Toolchain Setup ===
RISCV_PREFIX   = riscv64-elf-
CC             = $(RISCV_PREFIX)gcc
OPENOCD        = openocd

# === Project Structure ===
SRC_DIR   = src
BUILD_DIR = build
LINKER    = linker/metal.default.lds
OPENOCD_CFG = target/sifive-hifive1-revb.cfg

# === Files ===
SRCS      = $(wildcard $(SRC_DIR)/*.c)
ELF       = $(BUILD_DIR)/blink.elf

# === Compilation Flags ===
CFLAGS  = -march=rv32imac -mabi=ilp32 -T$(LINKER) -nostartfiles -specs=nosys.specs

# === Targets ===
all: $(BUILD_DIR) $(ELF)

# Ensure build directory exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compile and link in one step (EXACTLY like the working manual command)
$(ELF): $(SRCS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(ELF) $(SRCS)

# === Flash the Program ===
flash: $(ELF)
	$(OPENOCD) -f $(OPENOCD_CFG) -c "program $(ELF) verify reset exit"

# === Debugging with GDB ===
debug: $(ELF)
	$(OPENOCD) -f $(OPENOCD_CFG) &
	sleep 1  # Allow OpenOCD to start
	riscv64-unknown-elf-gdb $(ELF) -ex "target remote localhost:3333" -ex "monitor reset halt"

# === Clean Build Files ===
clean:
	rm -rf $(BUILD_DIR)
