
all:
	clang --target=riscv32 -march=rv32imaf -mabi=ilp32f \
		-D_stack=0x20005000 \
		-Wl,--section-start=.text=0x100 \
		-Wl,--section-start=.init=0x0 \
		-Wl,--section-start=.vector=0x04 \
		-Wl,--section-start=.bss=0x20000000 \
		-o t.elf -nostdlib ../main.c ../test.S
	llvm-objdump -d t.elf > t.lst
	llvm-objcopy t.elf -O ihex t.hex
	echo https://github.com/jnk0le/openocd-wch/releases
	sudo openocd -f ../tools/wch-riscv.cfg -c init -c halt  -c "program t.hex verify " -c reset -c wlink_reset_resume -c exit

