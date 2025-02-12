## **RED-V Open Toolchain Project**

This project demonstrates building and flashing bare-metal programs on the **SparkFun RED-V RedBoard** using a **open-source RISC-V toolchain**.

### **Toolchain**
- **Compiler**: `riscv64-elf-gcc`
- **Debugger**: `riscv64-elf-gdb`
- **Flashing**: `OpenOCD`
- **Build System**: `Make`

### **Usage**
1. **Build**
   ```bash
   make
   ```
2. **Flash to RED-V**
   ```bash
   make flash
   ```
3. **Debug with GDB**
   ```bash
   make debug
   ```

### **Hardware**
- **Processor**: SiFive FE310-G002 (RISC-V 32-bit)
- **Board**: SparkFun RED-V RedBoard
