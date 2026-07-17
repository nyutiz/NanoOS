CC="riscv64-unknown-elf-gcc"
CFLAGS="-march=rv64gc -mabi=lp64d -mcmodel=medany -nostdlib -ffreestanding -fno-pic -I lib/libfdt/ -I ."
LLD="/home/nyutiz/.rustup/toolchains/stable-x86_64-unknown-linux-gnu/lib/rustlib/x86_64-unknown-linux-gnu/bin/gcc-ld/ld.lld"

cargo build --target-dir build --release

$CC $CFLAGS -c boot.S -o build/boot.o
$CC $CFLAGS -c kernel.c -o build/kernel.o
$CC $CFLAGS -c uart.c -o build/uart.o
$CC $CFLAGS -c string.c -o build/string.o
$CC $CFLAGS -c lib/libfdt/fdt.c -o build/fdt.o
$CC $CFLAGS -c lib/libfdt/fdt_ro.c -o build/fdt_ro.o
$CC $CFLAGS -c lib/libfdt/fdt_strerror.c -o build/fdt_strerror.o

$LLD -T linker.ld -o kernel.elf \
    build/boot.o build/kernel.o build/uart.o build/string.o build/fdt.o build/fdt_ro.o build/fdt_strerror.o build/riscv64gc-unknown-none-elf/release/libkernel_rust.a

echo "Build OK -> kernel.elf"


