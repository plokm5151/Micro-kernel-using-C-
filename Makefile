CC      ?= clang
LD      ?= ld.lld
CFLAGS  := -target aarch64-unknown-none -ffreestanding -fno-stack-protector -fno-exceptions -fno-rtti -O2 -g
LDFLAGS := -nostdlib -static -T boot/kernel.ld
OBJS    := build/minimal.o
ELF     := build/kernel.elf

all: $(ELF)

build/minimal.o: boot/minimal.S
	mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

$(ELF): $(OBJS) boot/kernel.ld
	$(LD) $(LDFLAGS) $(OBJS) -o $(ELF)

run: $(ELF)
	qemu-system-aarch64 -machine virt,gic-version=3 -cpu cortex-a72 \
	  -smp 1 -m 512 -nographic -serial mon:stdio \
	  -kernel $(ELF) -d guest_errors,unimp -D .qemu.log

clean:
	rm -rf build .qemu.log

.PHONY: all run clean
